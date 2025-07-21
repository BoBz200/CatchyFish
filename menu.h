#pragma once

#include <ncurses.h>
#include <string>
#include <vector>

#include "globalState.h"
#include "textBox.h"

class MenuButton : public Window {
public:
  MenuButton(int height, int width, int start_y, int start_x, state button_action, char key = 0);

  int get_key() const;
  bool get_is_selected() const;
  void set_is_selected(bool is_selected);

  void draw() const override;
  state get_action() const;

  bool is_mouse_on_button(MEVENT& event) const;

private:
  char key;
  bool is_selected;
  state button_action;
};

class Menu : public Window {
public:
  Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton> menu_buttons, std::vector<TextBox*> menu_texts);
  ~Menu();

  void draw() const override;
  bool handle_input(int input_key, GameState& state);

private:
  int selected_button_index;
  std::vector<MenuButton> menu_buttons;
  std::vector<TextBox*> menu_texts;
};
