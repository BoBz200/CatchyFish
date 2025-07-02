#pragma once

#include <ncurses.h>
#include <string>
#include <vector>
#include <functional>
#include "globalState.h"
#include "myNcursesUtils.h"
#include "textBox.h"

class MenuButton {
public:
  MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f, char key);
  MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f);

  int get_key() const;
  bool get_is_selected() const;
  void set_is_selected(bool is_selected);

  void draw();
  void clear();
  void action(GameState& state);

  bool is_mouse_on_button(MEVENT& event);

private:
  char key;
  bool is_selected;
  std::function<void(GameState&)> f;

protected:
  int height;
  int width;
  int start_y;
  int start_x;
};

class Menu {
public:
  Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton*>* menu_buttons, std::vector<TextBox*>* menu_texts);
  ~Menu();

  bool get_is_boxed() const;
  void set_is_boxed(bool is_boxed);

  void draw();
  void clear();
  bool handle_input(int ch, GameState& state);

private:
  int height;
  int width;
  int start_y;
  int start_x;

  bool is_boxed;

  int selected_button_index;
  std::vector<MenuButton*>* menu_buttons;
  std::vector<TextBox*>* menu_texts;
};
