#include <ncurses.h>

void draw_rectangle(int height, int width, int y, int x) {
    mvhline(y, x + 1, 0, width - 2);
    mvhline(y + height - 1, x + 1, 0, width - 2);
    mvvline(y + 1, x, 0, height - 2);
    mvvline(y + 1, x + width - 1, 0, height - 2);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y + height - 1, x, ACS_LLCORNER);
    mvaddch(y, x + width - 1, ACS_URCORNER);
    mvaddch(y + height - 1, x + width - 1, ACS_LRCORNER);
}

void clear_rectangle(int height, int width, int y, int x) {
  for (int i = 0; i < height; i++) {
    move(y + i, x);
    for (int j = 0; j < width; j++) {
      addch(' ');
    }
  }
}
