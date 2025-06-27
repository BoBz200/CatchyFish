#pragma once

#include "overlay.h"

enum state {
  MainMenu,
  Game,
  Paused,
  Quit,
};

class Menu;

struct GameState {
  enum state current_state;
  Overlay* overlay;
  Menu* menu;
};

