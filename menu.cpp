#include <functional>
#include <ncurses.h>
#include <string>
#include <vector>
#include "menu.h"
#include "globalState.h"

MenuItem::MenuItem(int height, int width, int start_y, int start_x) {
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;
  win = newwin(height, width, start_y, start_x);
}

MenuItem::~MenuItem() {
  delwin(win);
}

MenuButton::MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f) :
MenuItem(height, width, start_y, start_x){
  key = 0;
  is_selected = false;
  this->f = f;
}

MenuButton::MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f, char key) :
MenuButton(height, width, start_y, start_x, f){
  this->key = key;
}

void MenuItem::clear_menu_item() {
  wclear(win);
  wrefresh(win);
}

int MenuButton::get_key() const {
  return key;
}
bool MenuButton::get_is_selected() const {
  return key;
}

void MenuButton::set_is_selected(bool is_selected) {
  if (this->is_selected == is_selected)
    return;

  if (is_selected) {
    delwin(win);
    win = newwin(height + 2, width + 2, start_y - 1, start_x - 1);
  }
  else {
    delwin(win);
    win = newwin(height, width, start_y, start_x);
  }
  this->is_selected = is_selected;
}

void MenuButton::refresh() {
  box(win, 0, 0);
  wrefresh(win);
}

void MenuButton::action(GameState& state) {
  f(state);
}

MenuText::MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text) :
MenuItem(height, width, start_y, start_x) {
  this->text = text;
  is_boxed = false;
  is_text_centered = false;
}

MenuText::MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text, bool is_text_centered) :
MenuText(height, width, start_y, start_x, text) {
  this->is_text_centered = is_text_centered;
}

MenuText::MenuText(int height, int width, int start_y, int start_x, std::vector<std::string> text, bool is_text_centered, bool is_boxed) :
MenuText(height, width, start_y, start_x, text, is_text_centered) {
  this->is_boxed = is_boxed;
}

std::vector<std::string> MenuText::get_text() const {
  return text;
}
bool MenuText::get_is_boxed() const {
  return is_boxed;
}

bool MenuText::get_is_text_centered() const {
  return is_text_centered;
}

void MenuText::set_text(std::vector<std::string>& text) {
  this->text = text;
}

void MenuText::set_is_boxed(bool is_boxed) {
  this->is_boxed = is_boxed;
}

void MenuText::set_is_text_centered(bool is_text_centered) {
  this->is_text_centered = is_text_centered;
}

void MenuText::refresh() {
  if (is_boxed)
    box(win, 0, 0);

  if (is_text_centered)
    refresh_centered();
  else
    refresh_not_centered();

  wrefresh(win);
}

void MenuText::refresh_centered() {
  for (int i = 0; (i < height - 2 && i < text.size()); i++) {
    std::string line = text[i];

    int start = ((width - 2) / 2) - (line.size() / 2);
    start = start < 0 ? 0 : start;
    wmove(win, i + 1, start + 1);

    for (int j = 0; (j < width - 2 && j < line.size()); j++) {
      char ch = line[j];
      if (ch == ' ') {
        wmove(win, i + 1, j + 2 + start);
        continue;
      }

      waddch(win, ch);
    }
  }
}

void MenuText::refresh_not_centered() {
  for (int i = 0; (i < height - 2 && i < text.size()); i++) {
    std::string line = text[i];
    wmove(win, i + 1, 1);

    for (int j = 0; (j < width - 2 && j < line.size()); j++) {
      char ch = line[j];
      if (ch == ' ') {
        wmove(win, i + 1, j + 2);
        continue;
      }

      waddch(win, ch);
    }
  }
}


  Menu::Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton*>* menu_buttons, std::vector<MenuText*>* menu_texts) {
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;
  win = newwin(height, width, start_y, start_x);

  is_boxed = false;

  this->menu_buttons = menu_buttons;
  if (menu_buttons->size() > 0) {
  selected_button_index = 0;
  (*menu_buttons)[selected_button_index]->set_is_selected(true);
  }
  else // if the menu has no buttons
    selected_button_index = -1;
    // -1 means there are no buttons so dont try to access one

  this->menu_texts = menu_texts;
}

Menu::~Menu() {
  wclear(win);
  delwin(win);

  for (MenuText* menu_text : *menu_texts) {
    delete menu_text;
  }
  delete menu_texts;
  for (MenuButton* menu_item : *menu_buttons) {
    delete menu_item;
  }
  delete menu_buttons;
}

bool Menu::get_is_boxed() const {
  return is_boxed;
}

void Menu::set_is_boxed(bool is_boxed) {
  this->is_boxed = is_boxed;
}

void Menu::refresh() {
  wclear(win);
  if (is_boxed)
    box(win, 0, 0);
  wrefresh(win);

  for (MenuButton* menu_button : *menu_buttons) {
    menu_button->refresh();
  }

  for (MenuText* menu_text : *menu_texts) {
    menu_text->refresh();
  }

}

void Menu::clear_menu() {
  wclear(win);

  for (MenuButton* menu_button : *menu_buttons) {
    menu_button->clear_menu_item();
  }
  for (MenuText* menu_text : *menu_texts) {
    menu_text->clear_menu_item();
  }

  wrefresh(win);
}

bool Menu::handle_input(int ch, GameState& state) {

  if (selected_button_index != -1 && (ch == 'j' || ch == KEY_DOWN || ch == '\t')) {
    (*menu_buttons)[selected_button_index]->set_is_selected(false);
    selected_button_index = ++selected_button_index % menu_buttons->size();
    (*menu_buttons)[selected_button_index]->set_is_selected(true);
    return true;
  }
  else if (selected_button_index != -1 && (ch == 'k' || ch == KEY_UP)) {
    (*menu_buttons)[selected_button_index]->set_is_selected(false);
    selected_button_index = (menu_buttons->size() - 1 + selected_button_index) % menu_buttons->size();
    (*menu_buttons)[selected_button_index]->set_is_selected(true);
    return true;
  }
  else if (selected_button_index != -1 && ch == '\n') {
    (*menu_buttons)[selected_button_index]->action(state);
    return true;
  }
  else {
    for (MenuButton* menu_button : *menu_buttons) {
      if (menu_button->get_key() == ch) {
        menu_button->action(state);
        return true;
      }
    }
  }

  return false;
}
