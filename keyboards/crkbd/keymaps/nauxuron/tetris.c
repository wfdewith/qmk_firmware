#include <avr/pgmspace.h>
#include <string.h>
#include <timer.h>
#include "oled_driver.h"

#include "tetris.h"

#define TETRIS_ROWS 24
#define TETRIS_COLS 10
#define CELL_SIZE 3
#define GRAVITY_MS 500
#define INPUT_DELAY_START_MS 250
#define INPUT_DELAY_REPEAT_MS 100

#define NUM_SHAPES 7
#define NUM_ORIENTATIONS 4
#define NUM_CELLS 4

struct tetris_loc {
  int8_t row;
  int8_t col;
};

struct tetris_block {
  uint8_t type;
  uint8_t orient;
  struct tetris_loc loc;
};

enum tetris_cell {
  TETRIS_EMPTY = 0,
  TETRIS_FILLED = 1,
};

enum tetris_input_state {
  TETRIS_INPUT_NONE,
  TETRIS_INPUT_ONCE,
  TETRIS_INPUT_REPEAT,
};

static const struct tetris_loc shapes[NUM_SHAPES][NUM_ORIENTATIONS][NUM_CELLS] = {
  // I
  {
    {{1, 0}, {1, 1}, {1, 2}, {1, 3}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
    {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}}
  },
  // J
  {
    {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
    {{0, 1}, {0, 2}, {1, 1}, {2, 1}},
    {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
    {{0, 1}, {1, 1}, {2, 0}, {2, 1}}
  },
  // L
  {
    {{0, 2}, {1, 0}, {1, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
    {{1, 0}, {1, 1}, {1, 2}, {2, 0}},
    {{0, 0}, {0, 1}, {1, 1}, {2, 1}}
  },
  // O
  {
    {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
    {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
    {{0, 1}, {0, 2}, {1, 1}, {1, 2}},
    {{0, 1}, {0, 2}, {1, 1}, {1, 2}}
  },
  // S
  {
    {{0, 1}, {0, 2}, {1, 0}, {1, 1}},
    {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
    {{1, 1}, {1, 2}, {2, 0}, {2, 1}},
    {{0, 0}, {1, 0}, {1, 1}, {2, 1}}
  },
  // T
  {
    {{0, 1}, {1, 0}, {1, 1}, {1, 2}},
    {{0, 1}, {1, 1}, {1, 2}, {2, 1}},
    {{1, 0}, {1, 1}, {1, 2}, {2, 1}},
    {{0, 1}, {1, 0}, {1, 1}, {2, 1}}
  },
  // Z
  {
    {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
    {{0, 2}, {1, 1}, {1, 2}, {2, 1}},
    {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
    {{0, 1}, {1, 0}, {1, 1}, {2, 0}}
  },
};

/* static const struct tetris_loc shape_offsets[NUM_SHAPES] = { */
/*   // I */
/*   {1, 0}, */
/*   // J */
/*   {0, }, */
/*   // L */
/*   {0}, */
/*   // O */
/*   {0}, */
/*   // S */
/*   {0}, */
/*   // T */
/*   {0}, */
/*   // Z */
/*   {0}, */
/* }; */

static uint8_t playfield[TETRIS_ROWS + 1][TETRIS_COLS];
static bool running = false;

static uint16_t frame_timer;
static bool dirty;

static int16_t remaining_gravity_ms = GRAVITY_MS;
static int16_t remaining_input_start_ms = 0;
static int16_t remaining_input_repeat_ms = 0;

static struct tetris_block falling;
static struct tetris_block next;

static uint16_t random_state = 0;

static enum tetris_input input = TETRIS_NONE;
static enum tetris_input_state input_state = TETRIS_INPUT_NONE;

static uint16_t random(void) {
  random_state ^= random_state << 7;
  random_state ^= random_state >> 9;
  random_state ^= random_state << 8;
  return random_state;
}

static bool tetris_is_in_bounds(uint8_t row, uint8_t col) {
  return 0 <= row && row <= TETRIS_ROWS && 0 <= col && col < TETRIS_COLS;
}

static void tetris_put(struct tetris_block block) {
  for (int i = 0; i < NUM_CELLS; i++) {
    struct tetris_loc cell = shapes[block.type][block.orient][i];
    playfield[block.loc.row + cell.row][block.loc.col + cell.col] = TETRIS_FILLED;
  }
}

static void tetris_remove(struct tetris_block block) {
  for (int i = 0; i < NUM_CELLS; i++) {
    struct tetris_loc cell = shapes[block.type][block.orient][i];
    playfield[block.loc.row + cell.row][block.loc.col + cell.col] = TETRIS_EMPTY;
  }
}

static bool tetris_fits(struct tetris_block block) {
  for (int i = 0; i < NUM_CELLS; i++) {
    struct tetris_loc cell = shapes[block.type][block.orient][i];
    uint8_t row = block.loc.row + cell.row;
    uint8_t col = block.loc.col + cell.col;
    if (!tetris_is_in_bounds(row, col) || playfield[row][col] == TETRIS_FILLED) {
      return false;
    }
  }
  return true;
}

static void tetris_move(char direction) {
  tetris_remove(falling);
  falling.loc.col += direction;
  if (!tetris_fits(falling)) {
    falling.loc.col -= direction;
  }
  tetris_put(falling);
}

static void tetris_rotate(char rotation) {
  tetris_remove(falling);
  falling.orient = (falling.orient + rotation) % NUM_ORIENTATIONS;
  if (tetris_fits(falling)) {
    goto done;
  }
  falling.loc.col++;
  if (tetris_fits(falling)) {
    goto done;
  }
  falling.loc.col -= 2;
  if (tetris_fits(falling)) {
    goto done;
  }
  falling.loc.col++;
  falling.orient = (falling.orient - rotation) % NUM_ORIENTATIONS;
done:
  tetris_put(falling);
}

static void tetris_new_falling(void) {
  falling = next;
  next.type = random() % NUM_SHAPES;
  next.orient = 0;
  next.loc.row = 0;
  next.loc.col = TETRIS_COLS / 2 - 2;
}

static bool tetris_gravity_update(uint16_t elapsed) {
  remaining_gravity_ms -= elapsed;
  if (remaining_gravity_ms > 0) {
    return false;
  }

  tetris_remove(falling);
  falling.loc.row++;
  if (!tetris_fits(falling)) {
    falling.loc.row--;
    tetris_put(falling);
    tetris_new_falling();
  }
  remaining_gravity_ms = GRAVITY_MS;
  tetris_put(falling);
  return true;
}

static bool tetris_handle_input(uint16_t elapsed) {
  switch (input_state) {
    case TETRIS_INPUT_NONE:
      remaining_input_start_ms = INPUT_DELAY_START_MS;
      input_state = TETRIS_INPUT_ONCE;
      break;
    case TETRIS_INPUT_ONCE:
      remaining_input_start_ms -= elapsed;
      if (remaining_input_start_ms > 0) {
        return false;
      }
      remaining_input_repeat_ms = 0;
      input_state = TETRIS_INPUT_REPEAT;
      break;
    case TETRIS_INPUT_REPEAT:
      remaining_input_repeat_ms -= elapsed;
      if (remaining_input_repeat_ms > 0) {
        return false;
      }
      remaining_input_repeat_ms = INPUT_DELAY_REPEAT_MS;
      break;
  }

  switch (input) {
    case TETRIS_NONE:
      return false;
    case TETRIS_LEFT:
      tetris_move(-1);
      return true;
    case TETRIS_RIGHT:
      tetris_move(1);
      return true;
    case TETRIS_CW:
      tetris_rotate(1);
      return true;
    case TETRIS_CCW:
      tetris_rotate(-1);
      return true;
  }
  return false;
}

static bool tetris_is_row_full(int row) {
  for (int col = 0; col < TETRIS_COLS; col++) {
    if (playfield[row][col] == TETRIS_EMPTY) {
      return false;
    }
  }
  return true;
}

static void tetris_clear_row(int row) {
  for (int r = row - 1; r > 0; r--) {
    for (int col = 0; col < TETRIS_COLS; col++) {
      playfield[r + 1][col] = playfield[r][col];
      playfield[r][col] = TETRIS_EMPTY;
    }
  }
}

static int tetris_check_filled_rows(void) {
  int rows = 0;
  tetris_remove(falling);
  for (int row = TETRIS_ROWS; row >= 0; row--) {
    if (tetris_is_row_full(row)) {
      tetris_clear_row(row);
      row++;
      rows++;
    }
  }
  tetris_put(falling);
  return rows;
}

static bool tetris_is_game_over(void) {
  bool game_over = false;
  tetris_remove(falling);
  for (int col = 0; col < TETRIS_COLS; col++) {
    if (playfield[0][col] == TETRIS_FILLED) {
      game_over = true;
      break;
    }
  }
  tetris_put(falling);
  return game_over;
}

static void tetris_update(void) {
  uint16_t elapsed = timer_elapsed(frame_timer);

  dirty = tetris_gravity_update(elapsed) || dirty;

  dirty = tetris_handle_input(elapsed) || dirty;

  tetris_check_filled_rows();

  running = !tetris_is_game_over();

  frame_timer = timer_read();
}

static void tetris_render_playfield(void) {
  uint8_t default_row_pix[4] = {0x01, 0x00, 0x00, 0x80};
  uint8_t row_pix[4];

  for (int row = 1; row <= TETRIS_ROWS; row++) {
    memcpy(row_pix, default_row_pix, sizeof(row_pix));
    for (int col = 0; col < TETRIS_COLS; col++) {
      if (playfield[row][col] == TETRIS_FILLED) {
        for (uint8_t pix_col_offset = 0; pix_col_offset < CELL_SIZE; pix_col_offset++) {
          uint8_t pix_col = col * CELL_SIZE + pix_col_offset + 1;
          row_pix[pix_col / 8] |= 1 << (pix_col % 8);
        }
      }
      for (uint8_t pix_row_offset = 0; pix_row_offset < CELL_SIZE; pix_row_offset++) {
        uint8_t pix_row = (TETRIS_ROWS - row) * CELL_SIZE + pix_row_offset + 1;
        for (uint8_t pix_col = 0; pix_col < 4; pix_col++) {
          uint16_t idx = pix_col * OLED_DISPLAY_WIDTH + pix_row;
          oled_write_raw_byte(row_pix[pix_col], idx);
        }
      }
    }
  }

  for (uint16_t idx = 0; idx < OLED_MATRIX_SIZE; idx += OLED_DISPLAY_WIDTH) {
    oled_write_raw_byte(0xFF, idx);
    oled_write_raw_byte(0xFF, TETRIS_ROWS * CELL_SIZE + 1 + idx);
  }
  dirty = false;
}

static void tetris_render_next(void) {
  /* uint8_t bottom_row = TETRIS_ROWS * CELL_SIZE + 10; */
}

void tetris_render(void) {
  if (running) {
    tetris_update();
  }
  if (dirty) {
    oled_clear();
    tetris_render_playfield();
    tetris_render_next();
  }
}

void tetris_init(void) {
  memset(playfield, 0, sizeof(playfield));
  frame_timer = timer_read();
  random_state = TCNT0 + TCNT1 + TCNT3 + TCNT4;
  dirty = true;
  running = true;
  input = TETRIS_NONE;
  input_state = TETRIS_INPUT_NONE;

  next.type = random() % NUM_SHAPES;
  next.orient = 0;
  next.loc.row = 0;
  next.loc.col = TETRIS_COLS / 2 - 2;

  tetris_new_falling();
  tetris_put(falling);
}

void tetris_input(enum tetris_input in, bool pressed) {
  if (pressed) {
    input = in;
  } 
  if (!pressed && input == in) {
    input = TETRIS_NONE;
  }
  input_state = TETRIS_INPUT_NONE;
}

