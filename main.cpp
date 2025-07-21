#include <ncurses.h>
#include <random>
#include <ctime>
#include <vector>
#include <iostream>

#include "bar.h"
#include "menu.h"
#include "textBox.h"
#include "globalState.h"
#include "textAssets.h"
#include "fish.h"
#include "collection.h"

int get_fish_rarity_color(Rarity rarity);
std::vector<std::string> get_fish_variety_text(FishVariety name);
std::vector<std::string> get_fish_rarity_text(Rarity rarity);
std::vector<std::string> get_number_text(int number);
bool prepare_color();
std::vector<FishVariety> build_fishing_pool();

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

  VerticleProgressBar bar(27, 5, (y / 2) - (27 / 2), x / 13 * 2);

  int button_width = 40;
  Menu main_menu(y, x, 0, 0,
    std::vector<MenuButton>({
      MenuButton(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2), Waiting, 'p'),
      MenuButton(9, button_width, y / 9 * 5, (x / 2) - (button_width / 2), TutorialWaiting, 't'),
      MenuButton(9, button_width, y / 9 * 7, (x / 2) - (button_width / 2), Quit, 'q'),
    }),
    std::vector<TextBox*>({
      new TextBoxCentered(play_text, 9, button_width, y / 9 * 3 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(tutorial_text, 9, button_width, y / 9 * 5 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(quit_text, 9, button_width, y / 9 * 7 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(title_text2x, 18, 105, 1, (x / 2) - (105 / 2), false, COLOR_PAIR(3)),
      new TextBoxCentered(navigation_tip_text, 11, 35, y / 9 * 7, (x / 13 * 10), true, COLOR_PAIR(2)),
    })
  );

  Menu pause_menu(y / 3 * 2, x / 3 * 2, (y / 6), (x / 6),
    std::vector<MenuButton>({
      MenuButton(8, button_width, y / 6 + 2, (x / 2) - (button_width / 2), PreviousState, 'r'),
      MenuButton(8, button_width, y / 6 * 4 - 1, (x / 2) - (button_width / 2), Quit, 'q'),
    }),
    std::vector<TextBox*>({
      new TextBoxCentered(resume_text, 8, button_width, y / 6 + 2 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(quit_text, 8, button_width, y / 6 * 4 - 1 + 1, (x / 2) - (button_width / 2)),
    })
  );
  pause_menu.set_is_boxed(true);
  time_t pause_menu_timer = -1;

  Menu caught_menu(y / 3 * 2, x / 3 * 2, (y / 6), (x / 6),
    std::vector<MenuButton>({
      MenuButton(5, 12, (y / 6) + 2, (x / 6) + 3, Waiting, 'x'),
    }),
    std::vector<TextBox*>({
      new TextBoxCentered(x_text, 5, 12, (y / 6) + 2, (x / 6) + 3)
    })
  );
  caught_menu.set_is_boxed(true);

  main_menu.draw();
  refresh();

  bool waiting_animation_switch = false;
  bool can_switch = true;

  time_t fish_encounter_time = -1;
  char alternating_game_key = ' ';

  std::vector<FishVariety> fishing_pool = build_fishing_pool();
  Fish chosen_fish = Fish(fishing_pool[(int)(rand() % fishing_pool.size())]);

  Collection collection = Collection();
  collection.load_from_file();

  TextBoxCentered b_pressed(b_button_pressed, 10, 15, y / 6 * 5, x / 2);
  TextBoxCentered b(b_button, 10, 15, y / 6 * 5, x / 2);
  TextBoxCentered v_pressed(v_button_pressed, 10, 15, y / 6 * 5, x / 2 - 15);
  TextBoxCentered v(v_button, 10, 15, y / 6 * 5, x / 2 - 15);
  TextBoxCentered fishing_bobber_waiting1(bobber1, 13, 28, y / 2 - 13 / 2, x / 2 - 28 / 2);
  TextBoxCentered fishing_bobber_waiting2(bobber2, 13, 28, y / 2 - 13 / 2, x / 2 - 28 / 2);
  TextBoxCentered fishing_bobber_catching(bobber3, 13, 28, y / 2 - 13 / 2, x / 2 - 28 / 2);
  TextBoxCentered catching_fish_indicator(exclamation_mark, 10, 5, y / 4, x / 2 + 10, false, COLOR_PAIR(4));

  int ch;
  bool loop = true;
  while (loop) {

    switch (program_state.current_state) {
    case MainMenu:
      ch = getch();
      if (main_menu.handle_input(ch, program_state)) {
          main_menu.draw();
      }
      if (program_state.current_state != MainMenu) {
          mousemask(0, NULL);
          main_menu.clear();

          if (program_state.current_state == Waiting || program_state.current_state == TutorialWaiting) {
            if (program_state.current_state == TutorialWaiting)
              fish_encounter_time = time(NULL) + 15;
            else
              fish_encounter_time = time(NULL) + chosen_fish.get_fish_delay() +
                                    (int)(rand() % chosen_fish.get_random_fish_delay());
            halfdelay(1);
          }
      }
      break;

    case TutorialWaiting:
      TextBoxCentered::draw(waiting_tip_text, 11, 40, y / 9 * 7, (x / 13 * 10), true, COLOR_PAIR(2));

    case Waiting:
      ch = getch();
      if (ch == 27) {
        program_state.previous_state = program_state.current_state;
        program_state.current_state = Paused;
        pause_menu_timer = time(NULL);
        mousemask(BUTTON1_CLICKED, NULL);
        pause_menu.clear();
        pause_menu.draw();
        nocbreak();
        cbreak();
        break;
      }


      if (time(NULL) % 3 == 0 && can_switch) {
        waiting_animation_switch = !waiting_animation_switch;
        can_switch = false;
      }
      if (!can_switch && time(NULL) % 3 == 1)
        can_switch = true;

      if (waiting_animation_switch) {
        fishing_bobber_waiting2.clear();
        fishing_bobber_waiting1.draw();
      }
      else {
        fishing_bobber_waiting1.clear();
        fishing_bobber_waiting2.draw();
      }

      if (fish_encounter_time <= time(NULL)) {
        fish_encounter_time = -1; 
        bar.set_progress(0);
        if (program_state.current_state == TutorialWaiting) {
          clear();
          program_state.current_state = TutorialCatching;
        }
        else
          program_state.current_state = Catching;
        program_state.previous_state = Waiting;
        fishing_bobber_waiting1.clear();
        fishing_bobber_waiting2.clear();
        fishing_bobber_catching.draw();
        catching_fish_indicator.draw();
        v.draw();
        b.draw();
      }
      break;

    case TutorialCatching:
      TextBoxCentered::draw(catching_tip_text, 11, 40, y / 9 * 7, (x / 13 * 10), true, COLOR_PAIR(2));

    case Catching:
      ch = getch();
      if (ch == 27) {
        program_state.previous_state = program_state.current_state;
        program_state.current_state = Paused;
        mousemask(BUTTON1_CLICKED, NULL);
        pause_menu.clear();
        pause_menu.draw();
        nocbreak();
        cbreak();
        break;
      }

      if (ch == 'b' && alternating_game_key != 'b') {
        bar.set_progress(bar.get_progress() + chosen_fish.get_fishing_power());
        alternating_game_key = 'b';
      }
      else if (ch == 'v' && alternating_game_key != 'v') {
        bar.set_progress(bar.get_progress() + chosen_fish.get_fishing_power());
        alternating_game_key = 'v';
      }
      else if (ch == ERR) {
        bar.set_progress(bar.get_progress() - chosen_fish.get_fish_strength());
      }

      catching_fish_indicator.draw();
      fishing_bobber_catching.draw();
      if (alternating_game_key != ' ') {
          bar.draw();
      }
      if (alternating_game_key == 'b') {
        b.clear();
        v_pressed.clear();
        v.draw();
        b_pressed.draw();
      }
      else if (alternating_game_key == 'v') {
        v.clear();
        b_pressed.clear();
        b.draw();
        v_pressed.draw();
      }
      else {
          v_pressed.clear();
          b_pressed.clear();
          v.draw();
          b.draw();
      }

      if (bar.get_progress() >= 1) {
        clear();
        program_state.current_state = Caught;
        program_state.previous_state = Catching;
        alternating_game_key = ' ';
        fishing_bobber_catching.clear();
        catching_fish_indicator.clear();
        mousemask(BUTTON1_CLICKED, NULL);
        nocbreak();
        cbreak();
      }
      break;

    case Caught: {
      caught_menu.draw();

      TextBoxCentered::draw(get_fish_rarity_text(chosen_fish.get_rarity()),
                            6, x / 3 * 2, (y / 6) + 1, (x / 6),
                            get_fish_rarity_color(chosen_fish.get_rarity()));
      TextBoxCentered::draw(get_fish_variety_text(chosen_fish.get_variety()),
                            6, x / 3 * 2, (y / 6) + 12, (x / 6));

      if (!collection.is_fish_inside(chosen_fish.get_variety()))
        TextBoxCentered::draw(new_text, 6, 21, (y / 6) + 8, (x / 2) + 15, COLOR_PAIR(4));

      int size = (100 * chosen_fish.get_size());
      int color = collection.is_fish_bigger(chosen_fish) ? COLOR_PAIR(4) : 0;
      TextBoxCentered::draw(get_number_text(size / 100),
                            5, 8, (y / 6) + 20, (x / 2) - (38 / 2), color);
      TextBoxCentered::draw(period_text,
                            5, 3, (y / 6) + 20, (x / 2) - (38 / 2) + 8, color);
      TextBoxCentered::draw(get_number_text((size / 10) % 10),
                            5, 8, (y / 6) + 20, (x / 2) - (38 / 2) + 11, color);
      TextBoxCentered::draw(get_number_text(size % 10),
                            5, 8, (y / 6) + 20, (x / 2) - (38 / 2) + 19, color);
      TextBoxCentered::draw(m_text,
                            5, 11, (y / 6) + 20, (x / 2) - (38 / 2) + 27, color);
      // mvprintw(0, 0, "%d.%dm", size / 100, size % 100);

      ch = getch();
      if (caught_menu.handle_input(ch, program_state)) {
          caught_menu.draw();
      }
      if (ch == 27) {
        program_state.current_state = Paused;
        program_state.previous_state = Caught;
        mousemask(BUTTON1_CLICKED, NULL);
        caught_menu.clear();
        pause_menu.draw();
        break;
      }
      if (program_state.current_state != Caught) {
        mousemask(0, NULL);
        program_state.current_state = Waiting;
        program_state.previous_state = Caught;
        halfdelay(1);

        if (collection.is_fish_bigger(chosen_fish)) {
            collection.add(chosen_fish);
            collection.save_to_file();
        }

        chosen_fish = Fish(fishing_pool[(rand() % fishing_pool.size())]);
        fish_encounter_time = time(NULL) + chosen_fish.get_fish_delay() +
                              (rand() % chosen_fish.get_random_fish_delay());
        caught_menu.clear();
        fishing_bobber_waiting1.draw();
      }
      break;
    }

    case Paused:
      ch = getch();
      if (pause_menu.handle_input(ch, program_state)) {
          pause_menu.draw();
      }
      if (ch == 27) {
        program_state.current_state = PreviousState;
      }

      if (program_state.current_state != Paused) {
          if (program_state.current_state == PreviousState) {
            program_state.current_state = program_state.previous_state;

            if (program_state.current_state == TutorialWaiting || program_state.current_state == Waiting ||
                program_state.current_state == TutorialCatching || program_state.current_state == Catching) {
              if (fish_encounter_time != -1)
                fish_encounter_time += time(NULL) - pause_menu_timer;
              halfdelay(1);
            }
          }
          mousemask(0, NULL);
          program_state.previous_state = Paused;
          pause_menu.clear();
      }
      break;

    case Quit:
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
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  return true;
}

std::vector<FishVariety> build_fishing_pool() {
  std::vector<FishVariety> pool;
  for (int i = 0; i < 5; i++) {
    pool.push_back(Catfish);
    pool.push_back(Bass);
    pool.push_back(Cod);
    pool.push_back(Trout);
    pool.push_back(Bluegill);
  }

  for (int i = 0; i < 3; i++) {
    pool.push_back(Salmon);
    pool.push_back(Crawfish);
  }

  for (int i = 0; i < 2; i++) {
    pool.push_back(Eel);
  }

  for (int i = 0; i < 1; i++) {
    pool.push_back(Octopus);
  }

  return pool;
}

std::vector<std::string> get_fish_variety_text(FishVariety name) {
  switch (name) {
    case Catfish:
      return catfish_text;
    case Bass:
      return bass_text;
    case Cod:
        return cod_text;
    case Trout:
      return trout_text;
    case Bluegill:
      return bluegill_text;
    case Salmon:
      return salmon_text;
    case Crawfish:
      return crawfish_text;
    case Eel:
      return eel_text;
    case Octopus:
      return octopus_text;
    default:
      return catfish_text;
  }
}

std::vector<std::string> get_fish_rarity_text(Rarity rarity) {
  switch (rarity) {
    case Common:
      return common_text;
    case Uncommon:
      return uncommon_text;
    case Rare:
      return rare_text;
    case Legendary:
      return legendary_text;
    default:
      return common_text;
  }
}

int get_fish_rarity_color(Rarity rarity) {
  if (!has_colors())
    return 0;

  switch (rarity) {
    case Common:
      return 0;
    case Uncommon:
      return COLOR_PAIR(2);
    case Rare:
      return COLOR_PAIR(3);
    case Legendary:
      return COLOR_PAIR(5);
    default:
      return 0;
  }
}

std::vector<std::string> get_number_text(int number) {
  switch (number) {
    case 0:
      return zero_text;
    case 1:
      return one_text;
    case 2:
      return two_text;
    case 3:
      return three_text;
    case 4:
      return four_text;
    case 5:
      return five_text;
    case 6:
      return six_text;
    case 7:
      return seven_text;
    case 8:
      return eight_text;
    case 9:
      return nine_text;
    default:
      return zero_text;
  }
}
