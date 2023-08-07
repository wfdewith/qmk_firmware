#pragma once

enum tetris_input {
  TETRIS_NONE,
  TETRIS_LEFT,
  TETRIS_RIGHT,
  TETRIS_CW,
  TETRIS_CCW,
};

void tetris_render(void);
void tetris_init(void);

void tetris_input(enum tetris_input in, bool pressed);
