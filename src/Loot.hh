#ifndef LOOT_HH_
#define LOOT_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "ObjectCollectible.hh"
#include <json11.hpp>


class Loot
{
public:
  
  class Item
  {
  public:
    ObjectCollectible::Type _type;
    double                  _bonus;
  };


  Loot(json11::Json * config);

  Item * CreateLootItem(double type_random, double bonus_random);

private:
  class ItemDef
  {
  public:
    ObjectCollectible::Type _type;
    double                  _random_chance;
    double                  _bonus_min;
    double                  _bonus_max;
  };
  std::vector<ItemDef *> _definitions;
};

#endif
