#include <ncurses.h>

#include "bar.h"

ProgressBar::ProgressBar(float progress, int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {
  set_progress(progress);
}

ProgressBar::ProgressBar(int height, int width, int start_y, int start_x) :
Window(height, width, start_y, start_x), progress(0) {
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

VerticalProgressBar::VerticalProgressBar(float progress, int height, int width, int start_y, int start_x) :
ProgressBar(progress, height, width, start_y, start_x) {}

VerticalProgressBar::VerticalProgressBar(int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {}

void VerticalProgressBar::draw() const {
  box();

  // draw the progress bar
  int top = (1 - progress) * (height - 2);
  for (int i = 1; i <  height - 1; i++) {
    move(i + start_y, start_x + 1);

    for (int j = 1; j < width - 1; j++) {
      if (i <= top)
        addch(empty_char);
      else
        addch(full_char);
    }
  }
}

HorizontalProgressBar::HorizontalProgressBar(float progress, int height, int width, int start_y, int start_x) :
ProgressBar(progress, height, width, start_y, start_x) {}

HorizontalProgressBar::HorizontalProgressBar(int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {}

void HorizontalProgressBar::draw() const {
  box();

  // draw the progress bar
  int top = progress * (width - 2);
  for (int i = 1; i <  width - 1; i++) {
    move(start_y + 1, start_x + i);
    for (int j = 1; j < height - 1; j++) {
      if (i <= top)
        mvaddch(start_y + j, start_x + i, full_char);
      else
        mvaddch(start_y + j, start_x + i, empty_char);
    }
  }
}
