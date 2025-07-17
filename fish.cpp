#include "fish.h"


Fish::Fish(std::string name, float fishing_power, float fish_strength,
           int fish_delay, int random_fish_delay, int rarity, float min_size,
           float max_size) {
  this->name = name;
  this->fishing_power = fishing_power;
  this->fish_strength = fish_strength;
  this->fish_delay = fish_delay;
  this->random_fish_delay = random_fish_delay;
  this->rarity = rarity;
  this->min_size = min_size;
}

std::string Fish::get_name() const {
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
int Fish::get_rarity() const {
  return rarity;
}
float Fish::get_min_size() const {
  return min_size;
}
float Fish::get_max_size() const {
  return max_size;
}
