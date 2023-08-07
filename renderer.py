def chunks(l, n):
    for i in range(0, len(l), n):
        yield l[i : i + n]


class Screen:
    def __init__(self, width=128, height=32):
        self.data = [0] * ((height // 8) * width)
        self.width = width
        self.height = height

    def render(self):
        # 127 255 383 511
        # 126 254 382 510
        # ...
        #   0 128 256 384

        with_borders = lambda row: f"│{row}│"
        upper_border = "┌" + ("─" * self.height) + "┐"
        lower_border = "└" + ("─" * self.height) + "┘"

        print(upper_border)
        for i in range(self.width - 1, -1, -2):
            row_str_buf = []
            for j in range(0, len(self.data), self.width):
                b1 = self.data[i + j]
                b2 = self.data[i - 1 + j]
                for k in range(8):
                    b = 1 << k
                    match b1 & b, b2 & b:
                        case (0, 0):
                            row_str_buf.append(" ")
                        case (0, _):
                            row_str_buf.append("▄")
                        case (_, 0):
                            row_str_buf.append("▀")
                        case (_, _):
                            row_str_buf.append("█")
            print(with_borders("".join(row_str_buf)), i - 1)
        print(lower_border)

    def clear(self):
        for i in range(len(self.data)):
            self.data[i] = 0

    def write_byte(self, byte, index):
        self.data[index] = byte


class Tetris:
    def __init__(self, screen, rows=20, cols=10, cell_size=3):
        self.screen = screen
        self.playfield = [[False for _ in range(cols)] for _ in range(rows + 1)]
        self.rows = rows
        self.cols = cols
        self.cell_size = cell_size

    def placeholder(self):
        for i in range(1, self.rows + 1):
            self.playfield[i][(i - 1) % self.cols] = True

    def render(self):
        self.screen.clear()
        self.render_next()
        self.render_playfield()
        self.screen.render()

    def render_next(self):
        border_width = 1
        horizontal_padding = 1
        vertical_padding = 3

        width = 4 * self.cell_size
        total_width = width + 2 * border_width + 2 * horizontal_padding
        height = 2 * self.cell_size
        total_height = height + 2 * border_width + 2 * vertical_padding

        bottom_row = 1 + self.rows * self.cell_size + 1 + self.cell_size
        top_row = bottom_row + total_height - border_width
        left_col = (self.screen.height - total_width) // 2
        right_col = left_col + total_width - border_width
        piece_left_col = left_col + border_width + horizontal_padding
        piece_right_col = right_col - border_width - horizontal_padding
        piece_bottom_row = bottom_row + border_width + vertical_padding
        piece_top_row = top_row - border_width - vertical_padding

        for i in range(bottom_row + 1, top_row):
            row_pix = [0x00, 0x01, 0x80, 0x00]
            if i == piece_bottom_row or i == piece_top_row:
                row_pix[piece_left_col // 8] |= 1 << (piece_left_col % 8)
                row_pix[piece_right_col // 8] |= 1 << (piece_right_col % 8)
            for j in range(4):
                self.screen.write_byte(row_pix[j], i + j * self.screen.width)

        row_pix = [0x00, 0xFF, 0xFF, 0x00]
        for j in range(4):
            self.screen.write_byte(row_pix[j], bottom_row + j * self.screen.width)
            self.screen.write_byte(row_pix[j], top_row + j * self.screen.width)

    def render_playfield(self):
        default_row_pix = [0x01, 0x00, 0x00, 0x80]
        for row in range(1, self.rows + 1):
            row_pix = default_row_pix.copy()
            for col in range(self.cols):
                if self.playfield[row][col]:
                    for pix_col_offset in range(self.cell_size):
                        pix_col = col * self.cell_size + pix_col_offset + 1
                        row_pix[pix_col // 8] |= 1 << (pix_col % 8)
                for pix_row_offset in range(self.cell_size):
                    pix_row = (self.rows - row) * self.cell_size + pix_row_offset + 1
                    for pix_col in range(4):
                        idx = pix_col * self.screen.width + pix_row
                        self.screen.write_byte(row_pix[pix_col], idx)
        for i in range(0, len(self.screen.data), self.screen.width):
            self.screen.write_byte(0xFF, i)
            self.screen.write_byte(0xFF, self.rows * self.cell_size + 1 + i)
