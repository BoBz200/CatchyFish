#pragma once

#include <ncurses.h>
#include <string>
#include <vector>
#include <functional>
#include "globalState.h"

class MenuItem {
public:
  MenuItem(int height, int width, int start_y, int start_x);
  virtual ~MenuItem();

  virtual void refresh() = 0;
  virtual void clear_menu_item();

protected:
  WINDOW* win;
  int height;
  int width;
  int start_y;
  int start_x;
};

class MenuButton: public MenuItem {
public:
  MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f, char key);
  MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f);

  int get_key() const;
  bool get_is_selected() const;
  void set_is_selected(bool is_selected);

  void refresh() override;
  void action(GameState& state);

private:
  char key;
  bool is_selected;
  std::function<void(GameState&)> f;
};

class MenuText: public MenuItem {
public:
  MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text);
  MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text, bool is_text_centered);
  MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text, bool is_text_centered, bool is_boxed);
  MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text, bool is_text_centered, bool is_boxed, int color);

  std::vector<std::string> get_text() const;
  bool get_is_boxed() const;
  bool get_is_text_centered() const;
  void set_text(std::vector<std::string>& text);
  void set_is_boxed(bool is_boxed);
  void set_is_text_centered(bool is_text_centered);

  void refresh() override;

private:
  std::vector<std::string> text;
  bool is_boxed;
  bool is_text_centered;
  int color;

  void refresh_centered();
  void refresh_not_centered();
};

class Menu {
public:
  Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton*>* menu_buttons, std::vector<MenuText*>* menu_texts);
  ~Menu();

  bool get_is_boxed() const;
  void set_is_boxed(bool is_boxed);

  void refresh();
  void clear_menu();
  bool handle_input(int ch, GameState& state);

private:
  WINDOW* win;
  int height;
  int width;
  int start_y;
  int start_x;

  bool is_boxed;

  int selected_button_index;
  std::vector<MenuButton*>* menu_buttons;
  std::vector<MenuText*>* menu_texts;
};
