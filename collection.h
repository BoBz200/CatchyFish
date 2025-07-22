#pragma once

#include <vector>

#include "fish.h"

class Collection {
public:
  Collection();

  void load_from_file();
  void save_to_file();

  void add(Fish fish);
  bool is_fish_inside(FishVariety fish);
  bool is_fish_bigger(Fish fish);

  float get_fish_size(FishVariety fish_variety);

private:
  std::vector<Fish> collection;
};
