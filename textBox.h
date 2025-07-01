#pragma once

#include <vector>
#include <string>

class TextBox {
public:
  TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x);
  TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed);
  TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color);

  void set_is_boxed(bool is_boxed);
  bool get_is_boxed() const;

  virtual void draw();

protected:
  int height;
  int width;
  int start_y;
  int start_x;

  bool is_boxed;
  std::vector<std::string> text;
  int color;
};

class TextBoxCentered: public TextBox {
public:
  TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x);
  TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed);
  TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color);

  void draw() override;
};
