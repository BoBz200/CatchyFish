#pragma once

#include <vector>
#include <string>

enum Rarity {
  Common,
  Uncommon,
  Rare,
  Legendary,
};

enum FishVariety {
  Catfish,
  Bass,
  Cod,
  Trout,
  Bluegill,
  Salmon,
  Crawfish,
  Eel,
  Octopus,
};

class Fish {
public:
  Fish(FishVariety name, float fishing_power, float fish_strength,
       int fish_delay, int random_fish_delay, Rarity rarity, float min_size,
       float max_size);

  Fish(FishVariety name);
  Fish(FishVariety name, float size);

  FishVariety get_variety() const;
  float get_fishing_power() const;
  float get_fish_strength() const;
  int get_fish_delay() const;
  int get_random_fish_delay() const;
  Rarity get_rarity() const;
  float get_min_size() const;
  float get_max_size() const;
  float get_size() const;

  // return the variety as a string
  std::string get_name() const;

  static int get_rarity_color(Rarity rarity);
  static const std::vector<std::string>& get_variety_text(FishVariety name);
  static const std::vector<std::string>& get_rarity_text(Rarity rarity);

private:
  void init_rarity(Rarity rarity);
  void init_size(float min_size, float max_size);

  FishVariety name;

  // amount bar increases on click
  float fishing_power;
  // amount bar decrease per 1/10 second
  float fish_strength;

  // the delay for waiting to catch fish
  // timer = time(NULL) + fish_delay + (int)(rand() % random_fish_delay)
  int fish_delay;
  int random_fish_delay;

  // number of fish added to the selection pool
  Rarity rarity;

  // bounds of fishes size
  float min_size;
  float max_size;

  float size;
};
