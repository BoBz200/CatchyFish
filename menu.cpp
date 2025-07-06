#include <functional>
#include <ncurses.h>
#include <vector>
#include "menu.h"
#include "globalState.h"
#include "myNcursesUtils.h"
#include "textBox.h"

MenuButton::MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f) {
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;

  this->f = f;
  key = 0;
  is_selected = false;
}

MenuButton::MenuButton(int height, int width, int start_y, int start_x, std::function<void(GameState&)> f, char key) :
MenuButton(height, width, start_y, start_x, f){
  this->key = key;
}

int MenuButton::get_key() const {
  return key;
}
bool MenuButton::get_is_selected() const {
  return key;
}

void MenuButton::set_is_selected(bool is_selected) {
  this->is_selected = is_selected;
}

void MenuButton::draw() {
  int increase = is_selected ? 1 : 0;
  draw_rectangle(height + (increase * 2), width + (increase * 2), start_y - increase, start_x - increase);
}

void MenuButton::clear() {
  clear_rectangle(height, width, start_y, start_x);
}

void MenuButton::action(GameState& state) {
  f(state);
}

bool MenuButton::is_mouse_on_button(MEVENT& event) {
  if (event.x >= start_x && event.x < start_x + width && event.y >= start_y && event.y < start_y + height)
    return true;
  return false;
}

  Menu::Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton*>* menu_buttons, std::vector<TextBox*>* menu_texts) {
  this->height = height;
  this->width = width;
  this->start_y = start_y;
  this->start_x = start_x;

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

  for (TextBox* menu_text : *menu_texts) {
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

void Menu::draw() {
  if (is_boxed)
    draw_rectangle(height, width, start_y, start_x);

  for (MenuButton* menu_button : *menu_buttons) {
    menu_button->draw();
  }

  for (TextBox* menu_text : *menu_texts) {
    menu_text->draw();
  }

}

void Menu::clear() {
  clear_rectangle(height, width, start_y, start_x);

  for (MenuButton* menu_button : *menu_buttons) {
    menu_button->clear();
  }
  for (TextBox* menu_text : *menu_texts) {
    menu_text->clear();
  }
}

bool Menu::handle_input(int ch, GameState& state) {

  if (ch == KEY_MOUSE) {
    MEVENT event;
    if (getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
      for (MenuButton* menu_button : *menu_buttons) {
        if (menu_button->is_mouse_on_button(event)) {
          (*menu_buttons)[selected_button_index]->set_is_selected(false);
          menu_button->set_is_selected(true);
          this->draw();
          menu_button->action(state);
          return true;
        }
      }
    }
  }
  else if (selected_button_index != -1 && (ch == 'j' || ch == KEY_DOWN || ch == '\t')) {
    (*menu_buttons)[selected_button_index]->set_is_selected(false);
    selected_button_index = ++selected_button_index % menu_buttons->size();
    (*menu_buttons)[selected_button_index]->set_is_selected(true);
    clear();
    return true;
  }
  else if (selected_button_index != -1 && (ch == 'k' || ch == KEY_UP)) {
    (*menu_buttons)[selected_button_index]->set_is_selected(false);
    selected_button_index = (menu_buttons->size() - 1 + selected_button_index) % menu_buttons->size();
    (*menu_buttons)[selected_button_index]->set_is_selected(true);
    clear();
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
