#pragma once

class Window {
public:
  Window(int height, int width, int start_y, int start_x, bool is_boxed = true);

  virtual void clear() const;
  virtual void draw() const = 0;

  void set_is_boxed(bool is_boxed);
  bool get_is_boxed() const;

  static void box(int height, int width, int start_y, int start_x, int color = 0);

protected:
  int height;
  int width;
  int start_y;
  int start_x;

  void box(int color = 0) const;

private:
  bool is_boxed;

};
