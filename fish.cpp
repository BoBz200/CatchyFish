#include "fish.h"
#include <string>

Fish::Fish(FishVariety name, float fishing_power, float fish_strength,
           int fish_delay, int random_fish_delay, Rarity rarity, float min_size,
           float max_size) {
  this->name = name;
  this->fishing_power = fishing_power;
  this->fish_strength = fish_strength;
  this->fish_delay = fish_delay;
  this->random_fish_delay = random_fish_delay;
  this->rarity = rarity;
  this->min_size = min_size;
  this->max_size = max_size;
  this->size = 0;
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

Fish::Fish(FishVariety name) {
  switch (name) {
    case Catfish:
      this->name = Catfish;
      init_rarity(Common);
      init_size(0.12, 2.5);
      break;
    case Bass:
      this->name = Bass;
      init_rarity(Common);
      init_size(0.3, 0.66);
      break;
    case Cod:
      this->name = Cod;
      init_rarity(Common);
      init_size(1, 2);
      break;
    case Trout:
      this->name = Trout;
      init_rarity(Common);
      init_size(0.12, 0.99);
      break;
    case Bluegill:
      this->name = Bluegill;
      init_rarity(Common);
      init_size(0.10, 0.30);
      break;
    case Salmon:
      this->name = Salmon;
      init_rarity(Uncommon);
      init_size(0.50, 0.71);
      break;
    case Crawfish:
      this->name = Crawfish;
      init_rarity(Uncommon);
      init_size(0.10, 0.17);
      break;
    case Eel:
      this->name = Eel;
      init_rarity(Rare);
      init_size(0.05, 4);
      break;
    case Octopus:
      this->name = Octopus;
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
