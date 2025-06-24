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

ProgressBar::ProgressBar(float progress, bool isVerticle, int height, int width, int start_y, int start_x) :
ProgressBar(height, width, start_y, start_x) {
  this->progress = progress;
  this->isVerticle = isVerticle;
}

ProgressBar::ProgressBar(int height, int width, int start_y, int start_x) :
Overlay(height, width, start_y, start_x) {
  progress = 0;
  isVerticle = true;
  box(win, 0, 0);
  if (has_colors() == false) {
    emptyChar = ' ';
    fullChar = '#';
  }
  else {
    emptyChar = ' ';
    fullChar = ' ' | COLOR_PAIR(1);
  }
}

float ProgressBar::getProgress() {
  return progress;
}
bool ProgressBar::getIsVerticle() {
  return isVerticle;
}
void ProgressBar::setProgress(float progress) {
  if (progress > 1) {
    this->progress = 1;
    return;
  }
  if (progress < 0) {
    this->progress = 0;
    return;
  }
  this-> progress = progress;
}
void ProgressBar::setIsVerticle(bool isVerticle) {
  this->isVerticle = isVerticle;
}

void ProgressBar::refresh() {
  if (isVerticle)
    verticleRefresh();
  else
    horizontalRefresh();

}

void ProgressBar::verticleRefresh() {
  int top = (1 - progress) * (height - 2);
  for (int i = 1; i <  height - 1; i++) {
    wmove(win, i, 1);
    for (int j = 1; j < width - 1; j++) {
      if (i <= top)
        waddch(win, emptyChar);
      else
        waddch(win, fullChar);
    }
  }
  wrefresh(win);
}

void ProgressBar::horizontalRefresh() {
  int top = progress * (width - 2);
  for (int i = 1; i <  width - 1; i++) {
    wmove(win, 1, i);
    for (int j = 1; j < height - 1; j++) {
      if (i <= top)
        mvwaddch(win, j, i, fullChar);
      else
        mvwaddch(win, j, i, emptyChar);
    }
  }
  wrefresh(win);
}
