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

bool prepare_color();
std::vector<FishVariety> build_fishing_pool();
const std::vector<std::string>& get_number_text(int number);
void draw_fish_size(float size, int start_y, int start_x, int color = 0);


int main() {
  using namespace Assets;

  // init screen
  initscr();
  srand(time(NULL));

  int y, x;
  getmaxyx(stdscr, y, x);

  // if terminal is too small for text assests
  if (y < 45 || x < 170) {
    endwin();
    std::cout << "Your terminal is not big enough.\nPlease increase the window or reduce the text size." << std::endl;
    return -1;
  }

  prepare_color();
  keypad(stdscr, TRUE); // allow reading arrow keys
  mousemask(BUTTON1_CLICKED, NULL); // allow mouse click
  curs_set(0); // hide cursor
  noecho(); // keypersses are not echoed
  set_escdelay(0); // remove escape delay
  const int ESC = 27;

  GameState program_state;
  program_state.current_state = MainMenu;
  program_state.previous_state = MainMenu;

  //catching progress bar
  VerticalProgressBar bar(27, 5, (y / 2) - (27 / 2), x / 13 * 2);

  // init menus
  int button_width = 48;
  Menu main_menu(y, x, 0, 0,
    std::vector<MenuButton>({
      MenuButton(9, button_width, y / 9 * 3, (x / 2) - (button_width / 2), Waiting),
      MenuButton(9, button_width, y / 9 * 5, (x / 2) - (button_width / 2), TutorialWaiting),
      MenuButton(9, button_width, y / 9 * 7, (x / 2) - (button_width / 2), Quit),
    }),
    std::vector<TextBox*>({
      new TextBoxCentered(play_text, 9, button_width, y / 9 * 3 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(tutorial_text, 9, button_width, y / 9 * 5 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(quit_text, 9, button_width, y / 9 * 7 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(title_text2x, 18, 105, 1, (x / 2) - (105 / 2), false, COLOR_PAIR(3)),
      new TextBoxCentered(navigation_tip_text, 11, 35, y / 9 * 7, (x / 13 * 10), true, COLOR_PAIR(2)),
    })
  );

  Menu escape_menu(y / 3 * 2, x / 3 * 2, (y / 6), (x / 6),
    std::vector<MenuButton>({
      MenuButton(8, button_width, y / 6 + 2, (x / 2) - (button_width / 2), PreviousState),
      MenuButton(8, button_width, y / 6 * 2 + 4, (x / 2) - (button_width / 2), ViewCollection),
      MenuButton(8, button_width, y / 6 * 4 - 1, (x / 2) - (button_width / 2), Quit),
    }),
    std::vector<TextBox*>({
      new TextBoxCentered(resume_text, 8, button_width, y / 6 + 2 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(collection_text, 8, button_width, y / 6 * 2 + 4 + 1, (x / 2) - (button_width / 2)),
      new TextBoxCentered(quit_text, 8, button_width, y / 6 * 4 - 1 + 1, (x / 2) - (button_width / 2)),
    })
  );
  escape_menu.set_is_boxed(true);

  MenuCollection collection_menu(std::vector<Menu*>({
      new Menu(y, x, 0, 0,
        std::vector<MenuButton>({
          MenuButton(7, 12, 1, 3, PreviousState, 'x'),
          MenuButton(6, 10, y - 7, x - 13, NextMenu),
        }),
        std::vector<TextBox*>({
          new TextBoxCentered(x_text, 5, 12, 2, 3),
          new TextBoxCentered(Fish::get_rarity_text(Common), 8, 50,
                              2, x / 2 - (50 / 2), true, Fish::get_rarity_color(Common)),
          new TextBoxCentered(next_arrow_text, 4, 8, y - 6, x - 12),
        })
      ),
      new Menu(y, x, 0, 0,
        std::vector<MenuButton>({
          MenuButton(7, 12, 1, 3, PreviousState, 'x'),
          MenuButton(6, 10, y - 7, 3, PreviousMenu),
          MenuButton(6, 10, y - 7, x - 13, NextMenu),
        }),
        std::vector<TextBox*>({
          new TextBoxCentered(x_text, 5, 12, 2, 3),
          new TextBoxCentered(Fish::get_rarity_text(Uncommon), 8, 60,
                              2, x / 2 - (60 / 2), true, Fish::get_rarity_color(Uncommon)),
          new TextBoxCentered(prev_arrow_text, 4, 8, y - 6, 4),
          new TextBoxCentered(next_arrow_text, 4, 8, y - 6, x - 12),
        })
      ),
      new Menu(y, x, 0, 0,
        std::vector<MenuButton>({
          MenuButton(7, 12, 1, 3, PreviousState, 'x'),
          MenuButton(6, 10, y - 7, 3, PreviousMenu),
          MenuButton(6, 10, y - 7, x - 13, NextMenu),
        }),
        std::vector<TextBox*>({
          new TextBoxCentered(x_text, 5, 12, 2, 3),
          new TextBoxCentered(Fish::get_rarity_text(Rare), 8, 40,
                              2, x / 2 - (40 / 2), true, Fish::get_rarity_color(Rare)),
          new TextBoxCentered(prev_arrow_text, 4, 8, y - 6, 4),
          new TextBoxCentered(next_arrow_text, 4, 8, y - 6, x - 12),
        })
      ),
      new Menu(y, x, 0, 0,
        std::vector<MenuButton>({
          MenuButton(7, 12, 1, 3, PreviousState, 'x'),
          MenuButton(6, 10, y - 7, 3, PreviousMenu),
        }),
        std::vector<TextBox*>({
          new TextBoxCentered(x_text, 5, 12, 2, 3),
          new TextBoxCentered(Fish::get_rarity_text(Legendary), 8, 70,
                              2, x / 2 - (70 / 2), true, Fish::get_rarity_color(Legendary)),
          new TextBoxCentered(prev_arrow_text, 4, 8, y - 6, 4),
        })
      ),
    })
  );
  Menu* pause_menu = &escape_menu;
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

  // draw first thing on screen
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

  // init text boxes
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
      ch = getch(); // read key pressed
      // if menu changed
      if (main_menu.handle_input(ch, program_state)) {
          main_menu.draw();
      }

      // if state has changed
      if (program_state.current_state != MainMenu) {
          mousemask(0, NULL); // disable mouse reading
          main_menu.clear();

          if (program_state.current_state == Waiting || program_state.current_state == TutorialWaiting) {
            if (program_state.current_state == TutorialWaiting)
              // tutorial has a set fish encounter time
              fish_encounter_time = time(NULL) + 15;
            else
              fish_encounter_time = time(NULL) + chosen_fish.get_fish_delay() +
                                    (int)(rand() % chosen_fish.get_random_fish_delay());
            halfdelay(1); // make getch() wait 1/10sec if nothing happens return ERR
          }
      }
      break;

    case TutorialWaiting:
      TextBoxCentered::draw(waiting_tip_text, 11, 40, y / 9 * 7, (x / 13 * 10), true, COLOR_PAIR(2));

    case Waiting:
      ch = getch();
      if (ch == ESC) {
        program_state.previous_state = program_state.current_state;
        program_state.current_state = Paused;
        pause_menu_timer = time(NULL);
        mousemask(BUTTON1_CLICKED, NULL); // enable mouse reading
        pause_menu->clear();
        pause_menu->draw();
        nocbreak(); // disable halfdelay(1)
        cbreak();   //
        break;
      }


      // switch animation frames
      if (time(NULL) % 3 == 0 && can_switch) {
        waiting_animation_switch = !waiting_animation_switch;
        can_switch = false;
      }
      if (!can_switch && time(NULL) % 3 == 1)
        can_switch = true;

      // draw frame
      if (waiting_animation_switch) {
        fishing_bobber_waiting2.clear();
        fishing_bobber_waiting1.draw();
      }
      else {
        fishing_bobber_waiting1.clear();
        fishing_bobber_waiting2.draw();
      }

      // if fish encounter time is up
      if (fish_encounter_time <= time(NULL)) {
        fish_encounter_time = -1; // -1 means catching fish or caught
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
      if (ch == ESC) {
        program_state.previous_state = program_state.current_state;
        program_state.current_state = Paused;
        mousemask(BUTTON1_CLICKED, NULL);
        pause_menu->clear();
        pause_menu->draw();
        nocbreak();
        cbreak();
        break;
      }

      // update bar
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

      // draw
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

      // if fish is caught
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

    case Caught:
      // draw
      caught_menu.draw();
      TextBoxCentered::draw(Fish::get_rarity_text(chosen_fish.get_rarity()),
                            6, x / 3 * 2, (y / 6) + 1, (x / 6),
                            Fish::get_rarity_color(chosen_fish.get_rarity()));
      TextBoxCentered::draw(Fish::get_variety_text(chosen_fish.get_variety()),
                            6, x / 3 * 2, (y / 6) + 12, (x / 6));

      if (!collection.is_fish_inside(chosen_fish.get_variety()))
        TextBoxCentered::draw(new_text, 6, 21, (y / 6) + 8, (x / 2) + 15, COLOR_PAIR(4));

      draw_fish_size(chosen_fish.get_size(), (y / 6) + 20, (x / 2) - (35 / 2),
                     collection.is_fish_bigger(chosen_fish) ? COLOR_PAIR(4) : 0);

      ch = getch();
      if (caught_menu.handle_input(ch, program_state)) {
          caught_menu.draw();
      }
      if (ch == ESC) {
        program_state.current_state = Paused;
        program_state.previous_state = Caught;
        mousemask(BUTTON1_CLICKED, NULL);
        caught_menu.clear();
        pause_menu->draw();
        break;
      }

      // if state has changed
      if (program_state.current_state != Caught) {
        mousemask(0, NULL);
        program_state.current_state = Waiting;
        program_state.previous_state = Caught;
        halfdelay(1);

        if (collection.is_fish_bigger(chosen_fish)) {
            collection.add(chosen_fish);
            collection.save_to_file();
        }

        // create next fish to catch
        chosen_fish = Fish(fishing_pool[(rand() % fishing_pool.size())]);
        fish_encounter_time = time(NULL) + chosen_fish.get_fish_delay() +
                              (rand() % chosen_fish.get_random_fish_delay());
        caught_menu.clear();
        fishing_bobber_waiting1.draw();
      }
      break;

    case PausedViewCollection: // draws the Collection menu
      // if viewing common fish
      if (collection_menu.get_selected_menu_index() == 0) {
        TextBoxCentered::draw(collection.is_fish_inside(Catfish) ?
                              Fish::get_variety_text(Catfish) : unknown_fish_text,
                              8, 34, (y / 6) + 12, (x / 5 * 0) + (x / 10) - (34 / 2));
        if (collection.is_fish_inside(Catfish))
          draw_fish_size(collection.get_fish_size(Catfish), (y / 6) + 22, (x / 5 * 0) + (x / 10) - (35 / 2));

        TextBoxCentered::draw(collection.is_fish_inside(Bass) ?
                              Fish::get_variety_text(Bass) : unknown_fish_text,
                              8, 24, (y / 6) + 12, (x / 5 * 1) + (x / 10) - (24 / 2));
        if (collection.is_fish_inside(Bass))
          draw_fish_size(collection.get_fish_size(Bass) , (y / 6) + 22, (x / 5 * 1) + (x / 10) - (35 / 2));

        TextBoxCentered::draw(collection.is_fish_inside(Cod) ?
                              Fish::get_variety_text(Cod) : unknown_fish_text,
                              8, 24, (y / 6) + 12, (x / 5 * 2) + (x / 10) - (24 / 2));
        if (collection.is_fish_inside(Cod))
          draw_fish_size(collection.get_fish_size(Cod), (y / 6) + 22, (x / 5 * 2) + (x / 10) - (35 / 2));

        TextBoxCentered::draw(collection.is_fish_inside(Trout) ?
                              Fish::get_variety_text(Trout) : unknown_fish_text,
                              8, 28, (y / 6) + 12, (x / 5 * 3) + (x / 10) - (28 / 2));
        if (collection.is_fish_inside(Trout))
          draw_fish_size(collection.get_fish_size(Trout), (y / 6) + 22, (x / 5 * 3) + (x / 10) - (35 / 2));

        TextBoxCentered::draw(collection.is_fish_inside(Bluegill) ?
                              Fish::get_variety_text(Bluegill) : unknown_fish_text,
                              8, 32, (y / 6) + 12, (x / 5 * 4) + (x / 10) - (32 / 2));
        if (collection.is_fish_inside(Bluegill))
          draw_fish_size(collection.get_fish_size(Bluegill), (y / 6) + 22, (x / 5 * 4) + (x / 10) - (35 / 2));

      }

      // if viewing uncommon fish
      if (collection_menu.get_selected_menu_index() == 1) {
        TextBoxCentered::draw(collection.is_fish_inside(Salmon) ?
                              Fish::get_variety_text(Salmon) : unknown_fish_text,
                              8, 38, (y / 6) + 12, (x / 2 * 0) + (x / 4) - (38 / 2));
          if (collection.is_fish_inside(Salmon))
            draw_fish_size(collection.get_fish_size(Salmon), (y / 6) + 22, (x / 2 * 0) + (x / 4) - (35 / 2));

        TextBoxCentered::draw(collection.is_fish_inside(Crawfish) ?
                              Fish::get_variety_text(Crawfish) : unknown_fish_text,
                              8, 42, (y / 6) + 12, (x / 2 * 1) + (x / 4) - (42 / 2));
          if (collection.is_fish_inside(Crawfish))
            draw_fish_size(collection.get_fish_size(Crawfish), (y / 6) + 22, (x / 2 * 1) + (x / 4) - (35 / 2));

      }

      // if rare
      if (collection_menu.get_selected_menu_index() == 2) {
        TextBoxCentered::draw(collection.is_fish_inside(Eel) ?
                              Fish::Fish::get_variety_text(Eel) : unknown_fish_text,
                              8, 20, (y / 6) + 12, (x / 1 * 0) + (x / 2) - (20 / 2));
          if (collection.is_fish_inside(Eel))
            draw_fish_size(collection.get_fish_size(Eel), (y / 6) + 22, (x / 1 * 0) + (x / 2) - (35 / 2));

      }

      // if legendary
      if (collection_menu.get_selected_menu_index() == 3) {
        TextBoxCentered::draw(collection.is_fish_inside(Octopus) ?
                              Fish::Fish::get_variety_text(Octopus) : unknown_fish_text,
                              8, 38, (y / 6) + 12, (x / 1 * 0) + (x / 2) - (38 / 2));
          if (collection.is_fish_inside(Octopus))
            draw_fish_size(collection.get_fish_size(Octopus), (y / 6) + 22, (x / 1 * 0) + (x / 2) - (35 / 2));

      }

    case Paused:
      ch = getch();
      if (pause_menu->handle_input(ch, program_state)) {
          pause_menu->draw();
      }
      // if user clicked collection button
      if (program_state.current_state == ViewCollection) {
        pause_menu->clear();
        pause_menu = &collection_menu;
        pause_menu->draw();
        program_state.current_state = PausedViewCollection;
      }

      if (ch == ESC) {
        program_state.current_state = PreviousState;
      }

      if (program_state.current_state != Paused && program_state.current_state != PausedViewCollection) {
          if (program_state.current_state == PreviousState) {
            program_state.current_state = program_state.previous_state;

            if (program_state.current_state == TutorialWaiting || program_state.current_state == Waiting ||
                program_state.current_state == TutorialCatching || program_state.current_state == Catching) {
              if (fish_encounter_time != -1)
                fish_encounter_time += time(NULL) - pause_menu_timer;
              halfdelay(1);
            }
          }
          pause_menu->clear();      //reset pause menu
          pause_menu = &escape_menu;//
          escape_menu.reset();      //
          collection_menu.reset();  //
          if (program_state.current_state != Caught)
            mousemask(0, NULL);
          program_state.previous_state = Paused;
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

// init colors for game if possible
// returns true if succeed else false
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

const std::vector<std::string>& get_number_text(int number) {
  using namespace Assets;

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

void draw_fish_size(float size, int start_y, int start_x, int color) {
  using namespace Assets;

  int size_in_cm = (100 * size);
  TextBoxCentered::draw(get_number_text(size_in_cm / 100), 5, 8, start_y, start_x, color);
  TextBoxCentered::draw(period_text, 5, 3, start_y, start_x + 8, color);
  TextBoxCentered::draw(get_number_text((size_in_cm / 10) % 10), 5, 8, start_y, start_x + 10, color);
  TextBoxCentered::draw(get_number_text(size_in_cm % 10), 5, 8, start_y, start_x + 16, color);
  TextBoxCentered::draw(m_text, 5, 11, start_y, start_x + 24, color);
  // mvprintw(0, 0, "%d.%dm", size / 100, size % 100);
}

