#include <cstdio>
#include <functional>
#include <ncurses.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>

#include "overlay.h"
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
  set_escdelay(0);

  GameState program_state;
  program_state.current_state = MainMenu;
  program_state.overlay = NULL;
  program_state.menu = NULL;

  refresh();

  VerticleProgressBar* bar = new VerticleProgressBar(27, 5, (y / 2) - (27 / 2), x / 13 * 11);

  int button_width = 40;
  Menu* main_menu = new Menu(y, x, 0, 0,
    new std::vector<MenuButton*>({
      new MenuButton(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Game;
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

  Menu* pause_menu = new Menu(y / 3 * 2, x / 3 * 2, (y / 6), (x / 6),
    new std::vector<MenuButton*>({
      new MenuButton(8, button_width, y / 6 + 2, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Game;
        }, 'r'),
      new MenuButton(8, button_width, y / 6 * 4 - 1, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Quit;
        }, 'q'),
    }),
    new std::vector<MenuText*>({
      new MenuText(8, button_width, y / 6 + 2, (x / 2) - (button_width / 2), resume_text, true),
      new MenuText(8, button_width, y / 6 * 4 - 1, (x / 2) - (button_width / 2), quit_text, true),
    })
  );

  pause_menu->set_is_boxed(true);

  program_state.menu = main_menu;
  program_state.menu->refresh();
  program_state.menu->refresh();

  char alternating_game_key = ' ';
  time_t fish_encounter_time = -1;

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
      if (ch == 27) {
        program_state.current_state = Paused;
        program_state.menu = pause_menu;
        mousemask(BUTTON1_CLICKED, NULL);
        program_state.menu->refresh();
        program_state.menu->refresh();
        nocbreak();
        cbreak();
        break;
      }
      if (program_state.overlay != NULL) {
          program_state.overlay->refresh();
      }

      if (fish_encounter_time == -1) {
        fish_encounter_time = time(NULL) + (int)(rand() % 30);
        printw("waiting");
        refresh();
      }
      else if (fish_encounter_time == 0) {
        float fishing_power = 0.03;
        float dropping_power = 0.005;
        if (ch == 'b' && alternating_game_key != 'b') {
          bar->set_progress(bar->get_progress() + fishing_power);
          alternating_game_key = 'b';
        }
        else if (ch == 'v' && alternating_game_key != 'v') {
          bar->set_progress(bar->get_progress() + fishing_power);
          alternating_game_key = 'v';
        }
        else if (ch == ERR) {
          bar->set_progress(bar->get_progress() - dropping_power);
        }

        if (bar->get_progress() >= 1) {
          printw("you did it");
          program_state.overlay = NULL;
          fish_encounter_time = -1;
        }
        refresh();
      }
      else if (fish_encounter_time <= time(NULL)) {
        fish_encounter_time = 0; 
        program_state.overlay = bar;
        printw("catch the fish");
        refresh();
      }


      if (program_state.current_state != Game) {
          nocbreak();
          cbreak();
      }
      break;
    }

    case Paused:
      ch = getch();
      if (program_state.menu->handle_input(ch, program_state)) {
          program_state.menu->refresh();
      }
      if (ch == 27) {
        program_state.current_state = Game;
      }
      if (program_state.current_state != Paused) {
          mousemask(0, NULL);
          program_state.menu->clear_menu();
          program_state.menu = NULL;
          halfdelay(1);
      }
      break;

    case Quit:
      delete main_menu;
      delete pause_menu;
      delete bar;
      refresh();
      loop = false;
      break;

    default:
      break;
    }
  }


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
