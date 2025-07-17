#include <ncurses.h>

#include "textBox.h"

TextBox::TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color) :
Window(height, width, start_y, start_x, is_boxed) {
  this->text = text;

  this->color = 0;
  if (has_colors() == true)
    this->color = color;
}

TextBox::TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed) :
TextBox(text, height, width, start_y, start_x, is_boxed, 0) {}

TextBox::TextBox(std::vector<std::string> text, int height, int width, int start_y, int start_x) :
Window(height, width, start_y, start_x, false) {}

void TextBox::draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, bool is_boxed, int color) {
  int margin = 0;
  if (is_boxed) {
    Window::box(height, width, start_y, start_x, color);
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

void TextBox::draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, int color) {
  draw(text, height, width, start_y, start_x, false, color); 
}

void TextBox::draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x) {
  draw(text, height, width, start_y, start_x, 0);
}

void TextBox::draw(const std::vector<std::string>& text, int start_y, int start_x, int color) {
  for (int i = 0; i < text.size(); i++) {
    std::string line = text[i];
    move(start_y + i, start_x);

    for (int j = 0; j < line.size(); j++) {
      char ch = line[j];
      if (ch == ' ') {
        move(start_y + i, start_x + j + 1);
        continue;
      }

      addch(ch | color);
    }
  }
}

void TextBox::draw(const std::vector<std::string>& text, int start_y, int start_x) {
  draw(text, start_y, start_x, 0);
}

void TextBox::draw() const{
  TextBox::draw(text, height, start_y, start_x, get_is_boxed(), color);
}


TextBoxCentered::TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed, int color) :
TextBox(text, height, width, start_y, start_x, is_boxed, color) {}

TextBoxCentered::TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x, bool is_boxed) :
TextBoxCentered(text, height, width, start_y, start_x, is_boxed, 0) {}

TextBoxCentered::TextBoxCentered(std::vector<std::string> text, int height, int width, int start_y, int start_x) :
TextBoxCentered(text, height, width, start_y, start_x, false) {}

void TextBoxCentered::draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, bool is_boxed, int color) {
  int margin = 0;
  if (is_boxed) {
    Window::box(height, width, start_y, start_x, color);
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

void TextBoxCentered::draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x, int color) {
  draw(text, height, width, start_y, start_x, false, color);
}
void TextBoxCentered::draw(const std::vector<std::string>& text, int height, int width, int start_y, int start_x) {
  draw(text, height, width, start_y, start_x, 0);
}

void TextBoxCentered::draw() const {
  draw(text, height, width, start_y, start_x, get_is_boxed(), color);
}
