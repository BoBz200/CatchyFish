#pragma once

enum state {
  MainMenu,
  Waiting,
  Catching,
  Caught,
  Paused,
  Quit,
};

struct GameState {
  enum state current_state;
  enum state previous_state;
};

