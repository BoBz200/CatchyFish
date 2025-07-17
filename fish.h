#pragma once

#include <string>

class Fish {
public:
  Fish(std::string name, float fishing_power, float fish_strength,
       int fish_delay, int random_fish_delay, int rarity, float min_size,
       float max_size);

  std::string get_name() const;
  float get_fishing_power() const;// = 0.03;
  float get_fish_strength() const;// = 0.005;
  int get_fish_delay() const;// = 5;
  int get_random_fish_delay() const;// = 25;
  int get_rarity() const;
  float get_min_size() const;
  float get_max_size() const;

private:
  std::string name;

  // amount bar increases on click
  float fishing_power;
  // amount bar decrease per 1/10 second
  float fish_strength;

  // the delay for waiting to catch fish
  // timer = time(NULL) + fish_delay + (int)(rand() % random_fish_delay)
  int fish_delay;
  int random_fish_delay;

  // number of fish added to the selection pool
  int rarity;

  // bounds of fishes size
  float min_size;
  float max_size;
};
