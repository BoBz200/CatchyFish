#include <ncurses.h>
#include <vector>

#include "overlay.h"
#include "menu.h"
#include "globalState.h"
#include "textAssets.h"

bool prepare_color();

int main() {

  initscr();
  prepare_color();
  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();

  GameState program_state;
  program_state.current_state = MainMenu;
  program_state.overlay = NULL;
  program_state.menu = NULL;

  printw("Gamestarted");
  refresh();

  int y, x;
  getmaxyx(stdscr, y, x);

  int button_width = 40;
  Menu* main_menu = new Menu(y, x, 0, 0,
    new std::vector<MenuButton*>({
      new MenuButton(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Game;
        }, 'p'),
      new MenuButton(9, button_width, y / 9 * 5, (x / 2) - (button_width / 2),
        [&](GameState& state) {
        }, 't'),
      new MenuButton(9, button_width, y / 9 * 7, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Quit;
        }, 'q'),
    }),
    new std::vector<MenuText*>({
      new MenuText(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2), play_text, true),
      new MenuText(9, button_width, y / 9 * 5, (x / 2) - (button_width / 2), tutorial_text, true),
      new MenuText(9, button_width, y / 9 * 7, (x / 2) - (button_width / 2), quit_text, true),
      new MenuText(18, 105, 1, (x / 2) - (105 / 2), title_text2x, true),
    })
  );

  program_state.menu = main_menu;
  program_state.menu->refresh();
  program_state.menu->refresh();

  int ch;
  bool loop = true;
  while (loop) {

    switch (program_state.current_state) {
    case MainMenu:
      ch = getch();
      if (program_state.menu->handle_input(ch, program_state)) {
          program_state.menu->refresh();
      }
      if (program_state.current_state != MainMenu) {
          program_state.menu->clear_menu();
          program_state.menu = NULL;
        }
      break;

    case Game:
      halfdelay(1);
      ch = getch();
      if (ch == 'q') {
          program_state.current_state = Quit;
          nocbreak();
          cbreak();
      }
      printw("Gaming ");
      refresh();
      break;

    case Quit:
      delete main_menu;
      refresh();
      loop = false;
      break;

    default:
      break;
    }
  }

  getch();

  endwin();

  return 0;
}

bool prepare_color() {
  if (has_colors() == false)
    return false;

  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  return true;
}
