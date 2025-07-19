#include <ncurses.h>

#include "window.h"

Window::Window(int height, int width, int start_y, int start_x, bool is_boxed) {
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;
  this->is_boxed = is_boxed;
}

Window::Window(int height, int width, int start_y, int start_x) :
Window(height, width, start_y, start_x, true) {}

void Window::clear() const {
  for (int i = 0; i < height; i++) {
    move(start_y + i, start_x);
    for (int j = 0; j < width; j++) {
      addch(' ');
    }
  }
}

void Window::set_is_boxed(bool is_boxed) {
  this->is_boxed = is_boxed;
}

bool Window::get_is_boxed() const {
  return is_boxed;
}

void Window::box(int height, int width, int start_y, int start_x, int color) {
  if (has_colors() != true)
    color = 0;

  mvhline(start_y, start_x + 1, ACS_HLINE | color, width - 2);
  mvhline(start_y + height - 1, start_x + 1, ACS_HLINE | color, width - 2);
  mvvline(start_y + 1, start_x, ACS_VLINE | color, height - 2);
  mvvline(start_y + 1, start_x + width - 1, ACS_VLINE | color, height - 2);
  mvaddch(start_y, start_x, ACS_ULCORNER | color);
  mvaddch(start_y + height - 1, start_x, ACS_LLCORNER | color);
  mvaddch(start_y, start_x + width - 1, ACS_URCORNER | color);
  mvaddch(start_y + height - 1, start_x + width - 1, ACS_LRCORNER | color);
}

void Window::box(int height, int width, int start_y, int start_x) {
  box(height, width, start_y, start_x, 0);
}

void Window::box(int color) const {
  box(height, width, start_y, start_x, color);
}

void Window::box() const {
  box(0);
}
