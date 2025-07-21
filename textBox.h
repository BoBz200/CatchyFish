#pragma once

#include <vector>
#include <string>

#include "window.h"

class TextBox : public Window {
public:
  TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color = 0);
  TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x);

  static void draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, bool is_boxed, int color);
  static void draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, int color = 0);
  static void draw(const std::vector<std::string>& text, int start_y, int start_x, int color = 0);

  virtual void draw() const;

protected:
  std::vector<std::string> text;
  int color;
};

class TextBoxCentered: public TextBox {
public:
  TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color = 0);
  TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x);

  static void draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, bool is_boxed, int color);
  static void draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, int color = 0);

  void draw() const override;
};
