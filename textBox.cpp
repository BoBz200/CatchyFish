#include <ncurses.h>
#include "textBox.h"
#include "myNcursesUtils.h"

TextBox::TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x) {
  this->text = text;
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;
  is_boxed = false;
  color = 0;
}

TextBox::TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed) :
TextBox(text, height, width, start_y, start_x) {
  this->is_boxed = is_boxed;
}

TextBox::TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color) :
TextBox(text, height, width, start_y, start_x, is_boxed) {
  if (has_colors() == true)
    this->color = color;
}

void TextBox::set_is_boxed(bool is_boxed) {
  this->is_boxed = is_boxed;
}

bool TextBox::get_is_boxed() const {
  return is_boxed;
}

void TextBox::draw() {
  int margin = 0;
  if (is_boxed) {
    draw_rectangle(height, width, start_y, start_x);
    margin = 1;
  }

  for (int i = 0; (i < height - (margin * 2) && i < text.size()); i++) {
    std::string line = text[i];
    move(start_y + i + margin, start_x + margin);

    for (int j = 0; (j < width - (margin * 2) && j < line.size()); j++) {
      char ch = line[j];
      if (ch == ' ') {
        move(start_y + i + margin, start_x + j + margin + 1);
        continue;
      }

      addch(ch | color);
    }
  }
}

void TextBox::clear() {
  clear_rectangle(height, width, start_y, start_x);
}

TextBoxCentered::TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x) :
TextBox(text, height, width, start_y, start_x) {}

TextBoxCentered::TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed) :
TextBox(text, height, width, start_y, start_x, is_boxed) {}

TextBoxCentered::TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color) :
TextBox(text, height, width, start_y, start_x, is_boxed, color) {}

void TextBoxCentered::draw() {
  int margin = 0;
  if (is_boxed) {
    draw_rectangle(height, width, start_y, start_x);
    margin = 1;
  }

  for (int i = 0; (i < height - (margin * 2) && i < text.size()); i++) {
    std::string line = text[i];

    int start = ((width - (margin * 2)) / 2) - (line.size() / 2);
    start = start < 0 ? 0 : start;
    move(start_y + i + margin, start_x + start + margin);

    for (int j = 0; (j < width - (margin * 2) && j < line.size()); j++) {
      char ch = line[j];
      if (ch == ' ') {
        move(start_y + i + margin, start_x + j + margin + start + 1);
        continue;
      }

      addch(ch | color);
    }
  }
}
