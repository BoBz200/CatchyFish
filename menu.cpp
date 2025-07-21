#include <ncurses.h>
#include <vector>

#include "menu.h"
#include "globalState.h"
#include "textBox.h"

MenuButton::MenuButton(int height, int width, int start_y, int start_x, state button_action, char key) :
Window(height, width, start_y, start_x), button_action(button_action), key(key), is_selected(false) {}

int MenuButton::get_key() const {
  return key;
}
bool MenuButton::get_is_selected() const {
  return key;
}

void MenuButton::set_is_selected(bool is_selected) {
  this->is_selected = is_selected;
}

// draws button bigger if selected
void MenuButton::draw() const {
  int increase = is_selected ? 1 : 0;
  box(height + (increase * 2), width + (increase * 2), start_y - increase, start_x - increase);
}

// returns the state the game should change to
state MenuButton::get_action() const {
  return button_action;
}

bool MenuButton::is_mouse_on_button(MEVENT& event) const {
  if (event.x >= start_x && event.x < start_x + width && event.y >= start_y && event.y < start_y + height)
    return true;
  return false;
}

Menu::Menu(int height, int width, int start_y, int start_x, std::vector<MenuButton> menu_buttons, std::vector<TextBox*> menu_texts) :
Window(height, width, start_y, start_x, false), menu_buttons(menu_buttons), menu_texts(menu_texts) {
  if (menu_buttons.size() > 0) {
    selected_button_index = 0;
    // select the first button
    this->menu_buttons[selected_button_index].set_is_selected(true);
  }
  else // if the menu has no buttons
    selected_button_index = -1;
    // -1 means there are no buttons so dont try to access one
}

Menu::~Menu() {
  for (TextBox* menu_text : menu_texts) {
    delete menu_text;
  }
}

void Menu::draw() const {
  if (get_is_boxed())
    box();

  for (const MenuButton& menu_button : menu_buttons) {
    menu_button.draw();
  }

  for (const TextBox* menu_text : menu_texts) {
    menu_text->draw();
  }
}

// checks if the keypress does something and return true if something happend else false
bool Menu::handle_input(int input_key, GameState& state) {

  if (input_key == KEY_MOUSE) {
    MEVENT event;
    // if user clicked
    if (getmouse(&event) == OK && event.bstate & BUTTON1_CLICKED) {
      for (MenuButton& menu_button : menu_buttons) {
        // check if user clicked on button then change state
        if (menu_button.is_mouse_on_button(event)) {
          menu_buttons[selected_button_index].set_is_selected(false);
          menu_button.set_is_selected(true);
          draw();
          state.current_state = menu_button.get_action();
          return true;
        }
      }
    }
  }
  else if (selected_button_index != -1 && (input_key == 'j' || input_key == KEY_DOWN || input_key == '\t')) {
    // select next button
    menu_buttons[selected_button_index].set_is_selected(false);
    selected_button_index = ++selected_button_index % menu_buttons.size();
    menu_buttons[selected_button_index].set_is_selected(true);
    clear();
    return true;
  }
  else if (selected_button_index != -1 && (input_key == 'k' || input_key == KEY_UP)) {
    // select prev button
    menu_buttons[selected_button_index].set_is_selected(false);
    selected_button_index = (menu_buttons.size() - 1 + selected_button_index) % menu_buttons.size();
    menu_buttons[selected_button_index].set_is_selected(true);
    clear();
    return true;
  }
  else if (selected_button_index != -1 && input_key == '\n') {
    // do button action
    state.current_state = menu_buttons[selected_button_index].get_action();
    return true;
  }
  else {
    // check if key corresponds with button then do button action
    for (const MenuButton& menu_button : menu_buttons) {
      if (menu_button.get_key() == input_key) {
        state.current_state = menu_button.get_action();
        return true;
      }
    }
  }

  return false;
}
