#pragma once

#include <ncurses.h>

#include "window.h"

class ProgressBar: public Window {
public:
  ProgressBar(float progress, int height, int width, int start_y, int start_x);
  ProgressBar(int height, int width, int start_y, int start_x);

  float get_progress() const;
  void set_progress(float progress);

protected:
  float progress;

  int empty_char;
  int full_char;
};

class VerticleProgressBar: public ProgressBar {
public:
  VerticleProgressBar(float progress, int height, int width, int start_y, int start_x);
  VerticleProgressBar(int height, int width, int start_y, int start_x);

  void draw() const override;
};

class HorizontalProgressBar: public ProgressBar {
public:
  HorizontalProgressBar(float progress, int height, int width, int start_y, int start_x);
  HorizontalProgressBar(int height, int width, int start_y, int start_x);

  void draw() const override;
};
