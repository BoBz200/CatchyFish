#pragma once

#include <ncurses.h>
#include <string>
#include <vector>
#include <functional>

#include "globalState.h"
#include "textBox.h"

class MenuButton : public Window {
public:
  MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f, char key);
  MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f);

  int get_key() const;
  bool get_is_selected() const;
  void set_is_selected(bool is_selected);

  void draw() override;
  void action(GameState& state);

  bool is_mouse_on_button(MEVENT& event);

private:
  char key;
  bool is_selected;
  std::function<void(GameState&)> f;
};

class Menu : public Window {
public:
  Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton> menu_buttons, std::vector<TextBox> menu_texts);

  void draw() override;
  bool handle_input(int ch, GameState& state);

private:
  int selected_button_index;
  std::vector<MenuButton> menu_buttons;
  std::vector<TextBox> menu_texts;
};
