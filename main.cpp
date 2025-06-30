#include <ncurses.h>
#include <vector>
#include <iostream>

// #include "overlay.h"
#include "menu.h"
#include "globalState.h"
#include "overlay.h"
#include "textAssets.h"

bool prepare_color();

int main() {
  initscr();

  int y, x;
  getmaxyx(stdscr, y, x);

  if (y < 45 || x < 170) {
    endwin();
    std::cout << "Your terminal is not big enough.\nPlease increase the window or reduce the text size." << std::endl;
    return -1;
  }

  prepare_color();
  keypad(stdscr, TRUE);
  mousemask(BUTTON1_CLICKED, NULL);
  curs_set(0);
  noecho();

  GameState program_state;
  program_state.current_state = MainMenu;
  program_state.overlay = NULL;
  program_state.menu = NULL;

  printw("Gamestarted");
  refresh();

  VerticleProgressBar* bar = new VerticleProgressBar(27, 5, (y / 2) - (27 / 2), x / 13 * 11);

  int button_width = 40;
  Menu* main_menu = new Menu(y, x, 0, 0,
    new std::vector<MenuButton*>({
      new MenuButton(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Game;
          state.overlay = bar;
          halfdelay(1);
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
      new MenuText(18, 105, 1, (x / 2) - (105 / 2), title_text2x, true, false, COLOR_PAIR(3)),
      new MenuText(11, 35, y / 9 * 7, (x / 13 * 10), navigation_tip_text, true, true, COLOR_PAIR(2)),
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
          mousemask(0, NULL);
          program_state.menu->clear_menu();
          program_state.menu = NULL;
      }
      break;

    case Game: {
      ch = getch();
      if (ch == 'q') {
          program_state.current_state = Quit;
          program_state.overlay = NULL;
      }
      if (program_state.overlay != NULL) {
          program_state.overlay->refresh();
      }
      float fishing_power = 0.03;
      float dropping_power = 0.01;
      if (ch == 'b') {
        bar->set_progress(bar->get_progress() + fishing_power);
      }
      else if (ch == ERR) {
        bar->set_progress(bar->get_progress() - dropping_power);
      }

      refresh();

      if (program_state.current_state != Game) {
          nocbreak();
          cbreak();
      }
      break;
    }

    case Quit:
      delete main_menu;
      delete bar;
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
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLACK);
  return true;
}
