#include <cstdio>
#include <functional>
#include <ncurses.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>

#include "overlay.h"
#include "menu.h"
#include "textBox.h"
#include "globalState.h"
#include "overlay.h"
#include "textAssets.h"

bool prepare_color();

int main() {
  initscr();
  srand(time(NULL));

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
  program_state.previous_state = MainMenu;
  program_state.overlay = NULL;
  program_state.menu = NULL;

  refresh();

  VerticleProgressBar* bar = new VerticleProgressBar(27, 5, (y / 2) - (27 / 2), x / 13 * 11);

  int button_width = 40;
  Menu* main_menu = new Menu(y, x, 0, 0,
    new std::vector<MenuButton*>({
      new MenuButton(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Waiting;
        }, 'p'),
      new MenuButton(9, button_width, y / 9 * 5, (x / 2) - (button_width / 2),
        [&](GameState& state) {
        }, 't'),
      new MenuButton(9, button_width, y / 9 * 7, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Quit;
        }, 'q'),
    }),
    new std::vector<TextBox*>({
      new TextBoxCentered(play_text, 9, button_width, y / 9 * 3 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(tutorial_text, 9, button_width, y / 9 * 5 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(quit_text, 9, button_width, y / 9 * 7 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(title_text2x, 18, 105, 1, (x / 2) - (105 / 2), false, COLOR_PAIR(3)),
      new TextBoxCentered(navigation_tip_text, 11, 35, y / 9 * 7, (x / 13 * 10), true, COLOR_PAIR(2)),
    })
  );

  Menu* pause_menu = new Menu(y / 3 * 2, x / 3 * 2, (y / 6), (x / 6),
    new std::vector<MenuButton*>({
      new MenuButton(8, button_width, y / 6 + 2, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = state.previous_state;
        }, 'r'),
      new MenuButton(8, button_width, y / 6 * 4 - 1, (x / 2) - (button_width / 2),
        [&](GameState& state) {
          state.current_state = Quit;
        }, 'q'),
    }),
    new std::vector<TextBox*>({
      new TextBoxCentered(resume_text, 8, button_width, y / 6 + 2 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(quit_text, 8, button_width, y / 6 * 4 - 1 + 1, (x / 2) - (button_width / 2)),
    })
  );

  pause_menu->set_is_boxed(true);

  program_state.menu = main_menu;
  program_state.menu->draw();
  refresh();

  char alternating_game_key = ' ';
  time_t fish_encounter_time = -1;
  float fishing_power = 0.03;
  float dropping_power = 0.005;

  int ch;
  bool loop = true;
  while (loop) {

    switch (program_state.current_state) {
    case MainMenu:
      ch = getch();
      if (program_state.menu->handle_input(ch, program_state)) {
          program_state.menu->draw();
      }
      if (program_state.current_state != MainMenu) {
          mousemask(0, NULL);
          program_state.menu->clear();
          program_state.menu = NULL;

          if (program_state.current_state == Waiting) {
            fish_encounter_time = time(NULL) + (int)(rand() % 30);
            mvprintw(0, 0, "waiting");
            halfdelay(1);
          }
      }
      break;

    case Waiting:
      ch = getch();
      if (ch == 27) {
        program_state.current_state = Paused;
        program_state.previous_state = Waiting;
        program_state.menu = pause_menu;
        mousemask(BUTTON1_CLICKED, NULL);
        program_state.menu->clear();
        program_state.menu->draw();
        nocbreak();
        cbreak();
        break;
      }

      if (fish_encounter_time <= time(NULL)) {
        fish_encounter_time = -1; 
        program_state.overlay = bar;
        bar->set_progress(0);
        program_state.current_state = Catching;
        program_state.previous_state = Waiting;
        mvprintw(0, 0, "catch the fish");
      }
      break;

    case Catching:
      ch = getch();
      if (ch == 27) {
        program_state.current_state = Paused;
        program_state.previous_state = Catching;
        program_state.menu = pause_menu;
        mousemask(BUTTON1_CLICKED, NULL);
        program_state.menu->clear();
        program_state.menu->draw();
        nocbreak();
        cbreak();
        break;
      }

      if (program_state.overlay != NULL) {
          program_state.overlay->draw();
      }

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
        mvprintw(0, 0, "you did it");
        clear();
        program_state.overlay = NULL;
        program_state.current_state = Waiting;
        program_state.previous_state = Catching;
        fish_encounter_time = time(NULL) + (int)(rand() % 30);
        mvprintw(0, 0, "waiting");
      }
      break;

    case Paused:
      ch = getch();
      if (program_state.menu->handle_input(ch, program_state)) {
          program_state.menu->draw();
      }
      if (ch == 27) {
        program_state.current_state = program_state.previous_state;
      }
      if (program_state.current_state != Paused) {
          if (program_state.current_state == Waiting || program_state.current_state == Catching)
            halfdelay(1);
          mousemask(0, NULL);
          program_state.previous_state = Paused;
          program_state.menu->clear();
          program_state.menu = NULL;
      }
      break;

    case Quit:
      delete main_menu;
      delete pause_menu;
      delete bar;
      loop = false;
      break;

    default:
      break;
    }

    refresh();
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
