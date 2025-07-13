#pragma once

#include "overlay.h"

enum state {
  MainMenu,
  Waiting,
  Catching,
  Caught,
  Paused,
  Quit,
};

class Menu;

struct GameState {
  enum state current_state;
  enum state previous_state;
};

