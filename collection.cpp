#include <vector>
#include <fstream>

#include "collection.h"

Collection::Collection() :
collection(std::vector<Fish>()) {}

void Collection::add(Fish new_fish) {
  // if fish is not in collection add it
  if (!is_fish_inside(new_fish.get_variety())) {
    collection.push_back(new_fish);
    return;
  }

  // replace fish with new fish
  for (int i = 0; i < collection.size(); i++) {
    if (collection[i].get_variety() == new_fish.get_variety())
      collection[i] = new_fish;
  }
}

bool Collection::is_fish_inside(FishVariety fish_variety) {
  for (const Fish& fish : collection) {
    if (fish.get_variety() == fish_variety)
      return true;
  }
  return false;
}

// return true if fish is bigger then the fish inside
// the collecting or if there is no fish of that type
// else return false
bool Collection::is_fish_bigger(Fish fish) {
  // if fish is not in collection return true
  if (!is_fish_inside(fish.get_variety()))
    return true;

  for (const Fish& collected_fish : collection) {
    // if wrong fish continue
    if (collected_fish.get_variety() != fish.get_variety())
      continue;

    // if fish is bigger
    if (collected_fish.get_size() < fish.get_size())
      return true;
  }

  // if fish is not bigger
  return false;
}

void Collection::load_from_file() {
  std::ifstream file("collection.csv");
  // exit if cant open file
  if (!file.is_open()) {
    return;
  }

  std::string line;
  while (getline(file, line)) {
    std::string fish_variety = line.substr(0, line.find(","));
    line = line.substr(line.find(",") + 1, line.length());
    double size = std::stod(line.substr(0, line.find(",")));

    if (fish_variety == "Catfish")
      collection.push_back(Fish(Catfish, size));
    if (fish_variety == "Bass")
      collection.push_back(Fish(Bass, size));
    if (fish_variety == "Cod")
      collection.push_back(Fish(Cod, size));
    if (fish_variety == "Trout")
      collection.push_back(Fish(Trout, size));
    if (fish_variety == "Bluegill")
      collection.push_back(Fish(Bluegill, size));
    if (fish_variety == "Salmon")
      collection.push_back(Fish(Salmon, size));
    if (fish_variety == "Crawfish")
      collection.push_back(Fish(Crawfish, size));
    if (fish_variety == "Eel")
      collection.push_back(Fish(Eel, size));
    if (fish_variety == "Octopus")
      collection.push_back(Fish(Octopus, size));
  }
  file.close();
}

void Collection::save_to_file() {
  std::ofstream file("collection.csv");
  // exit if cant open file
  if (!file.is_open()) {
    return;
  }

  // save to file as {fishVariety}, {size}
  for (const Fish& fish : collection) {
    file << fish.get_name() << "," << fish.get_size() << std::endl;
  }
  file.close();
}

// return the size of the fish if it is inside else -1;
float Collection::get_fish_size(FishVariety fish_variety) {
  if (!is_fish_inside(fish_variety))
    return -1;

  for (const Fish& fish : collection) {
    if (fish.get_variety() == fish_variety)
      return fish.get_size();
  }
  return -1;
}
