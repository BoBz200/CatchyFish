#include <string>

#include "fish.h"

Fish::Fish(FishVariety name, float fishing_power, float fish_strength,
           int fish_delay, int random_fish_delay, Rarity rarity, float min_size,
           float max_size) :
name(name), fishing_power(fishing_power), fish_strength(fish_strength), rarity(rarity),
min_size(min_size), max_size(max_size), size(0) {
}

void Fish::init_rarity(Rarity rarity) {
  switch (rarity) {
    case Common:
      this->rarity = Common;
      fishing_power = 0.04;
      fish_strength = 0.007;
      fish_delay = 5;
      random_fish_delay = 25;
      break;
    case Uncommon:
      this->rarity = Uncommon;
      fishing_power = 0.03;
      fish_strength = 0.007;
      fish_delay = 5;
      random_fish_delay = 25;
      break;
    case Rare:
      this->rarity = Rare;
      fishing_power = 0.03;
      fish_strength = 0.005;
      fish_delay = 5;
      random_fish_delay = 30;
      break;
    case Legendary:
      this->rarity = Legendary;
      fishing_power = 0.025;
      fish_strength = 0.01;
      fish_delay = 10;
      random_fish_delay = 25;
      break;
  }
}

void Fish::init_size(float min_size, float max_size) {
  this->min_size = min_size;
  this->max_size = max_size;
  size = min_size +  (rand() % (int)(100 * (max_size - min_size))) / 100.0;
}

Fish::Fish(FishVariety name) :
name(name) {
  switch (name) {
    case Catfish:
      init_rarity(Common);
      init_size(0.12, 2.5);
      break;
    case Bass:
      init_rarity(Common);
      init_size(0.3, 0.66);
      break;
    case Cod:
      init_rarity(Common);
      init_size(1, 2);
      break;
    case Trout:
      init_rarity(Common);
      init_size(0.12, 0.99);
      break;
    case Bluegill:
      init_rarity(Common);
      init_size(0.10, 0.30);
      break;
    case Salmon:
      init_rarity(Uncommon);
      init_size(0.50, 0.71);
      break;
    case Crawfish:
      init_rarity(Uncommon);
      init_size(0.10, 0.17);
      break;
    case Eel:
      init_rarity(Rare);
      init_size(0.05, 4);
      break;
    case Octopus:
      init_rarity(Legendary);
      init_size(2.1, 4.8);
      break;
  }
}

Fish::Fish(FishVariety name, float size) :
Fish(name) {
  this->size = size;
};

FishVariety Fish::get_variety() const {
  return name;
}
float Fish::get_fishing_power() const {
  return fishing_power;
}
float Fish::get_fish_strength() const {
  return fish_strength;
}
int Fish::get_fish_delay() const {
  return fish_delay;
}
int Fish::get_random_fish_delay() const {
  return random_fish_delay;
}
Rarity Fish::get_rarity() const {
  return rarity;
}
float Fish::get_min_size() const {
  return min_size;
}
float Fish::get_max_size() const {
  return max_size;
}
float Fish::get_size() const {
  return size;
}

// return the variety as a string
std::string Fish::get_name() const {
  switch (name) {
    case Catfish:
      return "Catfish";
    case Bass:
      return "Bass";
    case Cod:
        return "Cod";
    case Trout:
      return "Trout";
    case Bluegill:
      return "Bluegill";
    case Salmon:
      return "Salmon";
    case Crawfish:
      return "Crawfish";
    case Eel:
      return "Eel";
    case Octopus:
      return "Octopus";
    default:
      return "";
  }
}
