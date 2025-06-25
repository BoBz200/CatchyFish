#pragma once

#include <ncurses.h>

class Overlay {
public:
  Overlay(int height, int width, int start_y, int start_x);
  virtual ~Overlay();

  virtual void refresh() = 0;

protected:
  WINDOW* win;
  int height;
  int width;
  int start_y;
  int start_x;

};

class ProgressBar: public Overlay {
public:
  ProgressBar(float progress, int height, int width, int start_y, int start_x);
  ProgressBar(int height, int width, int start_y, int start_x);

  float get_progress() const;
  void set_progress(float progress);

  virtual void refresh() = 0;

protected:
  float progress;
  int empty_char;
  int full_char;
};

class VerticleProgressBar: public ProgressBar {
public:
  VerticleProgressBar(float progress, int height, int width, int start_y, int start_x);
  VerticleProgressBar(int height, int width, int start_y, int start_x);

  void refresh() override;
};

class HorizontalProgressBar: public ProgressBar {
public:
  HorizontalProgressBar(float progress, int height, int width, int start_y, int start_x);
  HorizontalProgressBar(int height, int width, int start_y, int start_x);

  void refresh() override;
};
