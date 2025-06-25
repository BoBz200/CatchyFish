#include <ncurses.h>
#include "overlay.h"


Overlay::Overlay(int height, int width, int start_y, int start_x) {
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;
  win = newwin(height, width, start_y, start_x);
}

Overlay::~Overlay() {
  delwin(win);
}


ProgressBar::ProgressBar(float progress, int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {
  this->progress = progress;
}

ProgressBar::ProgressBar(int height, int width, int start_y, int start_x) :
Overlay(height, width, start_y, start_x) {
  progress = 0;
  box(win, 0, 0);
  if (has_colors() == false) {
    empty_char = ' ';
    full_char = '#';
  }
  else {
    empty_char = ' ';
    full_char = ' ' | COLOR_PAIR(1);
  }
}

float ProgressBar::get_progress() const {
  return progress;
}
void ProgressBar::set_progress(float progress) {
  if (progress > 1.0) {
    this->progress = 1;
    return;
  }
  if (progress < 0.0) {
    this->progress = 0;
    return;
  }
  this-> progress = progress;
}

VerticleProgressBar::VerticleProgressBar(float progress, int height, int width, int start_y, int start_x) :
ProgressBar(progress, height, width, start_y, start_x) {}

VerticleProgressBar::VerticleProgressBar(int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {}

void VerticleProgressBar::refresh() {
  int top = (1 - progress) * (height - 2);
  for (int i = 1; i <  height - 1; i++) {
    wmove(win, i, 1);
    for (int j = 1; j < width - 1; j++) {
      if (i <= top)
        waddch(win, empty_char);
      else
        waddch(win, full_char);
    }
  }
  wrefresh(win);
}

HorizontalProgressBar::HorizontalProgressBar(float progress, int height, int width, int start_y, int start_x) :
ProgressBar(progress, height, width, start_y, start_x) {}

HorizontalProgressBar::HorizontalProgressBar(int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {}

void HorizontalProgressBar::refresh() {
  int top = progress * (width - 2);
  for (int i = 1; i <  width - 1; i++) {
    wmove(win, 1, i);
    for (int j = 1; j < height - 1; j++) {
      if (i <= top)
        mvwaddch(win, j, i, full_char);
      else
        mvwaddch(win, j, i, empty_char);
    }
  }
  wrefresh(win);
}
