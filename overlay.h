#pragma once

#include <ncurses.h>

class Overlay {
public:
  Overlay(int height, int width, int start_y, int start_x);
  ~Overlay();

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
  ProgressBar(float progress, bool isVerticle, int height, int width, int start_y, int start_x);
  ProgressBar(int height, int width, int start_y, int start_x);

  float getProgress();
  bool getIsVerticle();
  void setProgress(float progress);
  void setIsVerticle(bool isVerticle);

  void refresh();

private:
  float progress;
  bool isVerticle;
  int emptyChar;
  int fullChar;

  void verticleRefresh();
  void horizontalRefresh();
};
