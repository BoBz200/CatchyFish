#pragma once

class Window {
public:
  Window(int height, int width, int start_y, int start_x, bool is_boxed);
  Window(int height, int width, int start_y, int start_x);

  virtual void clear();
  virtual void draw() = 0;

  void set_is_boxed(bool is_boxed);
  bool get_is_boxed() const;

  static void box(int height, int width, int start_y, int start_x, int color);
  static void box(int height, int width, int start_y, int start_x);

protected:
  int height;
  int width;
  int start_y;
  int start_x;

  void box(int color);
  void box();

private:
  bool is_boxed;

};
