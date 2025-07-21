#pragma once

enum state {
  MainMenu,
  TutorialWaiting,
  Waiting,
  TutorialCatching,
  Catching,
  Caught,
  Paused,
  Quit,
  PreviousState,
  NextMenu,
  PreviousMenu,
};

struct GameState {
  enum state current_state;
  enum state previous_state;
};

