/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Loot.hh"
#include <cassert>


Loot::Loot(json11::Json * config)
{
  assert(config->is_object());
  assert((*config)["definitions"].is_array());
  auto defs = (*config)["definitions"].array_items();
  for(auto def : defs)
    {
      auto d = new ItemDef();
      
      assert(def["type"].is_string());
      auto stype = def["type"].string_value();
      if(stype == "SCORE_BONUS")
        d->_type = ObjectCollectible::Type::SCORE_BONUS;
      else if(stype == "DAMAGE_MULTIPLIER")
        d->_type = ObjectCollectible::Type::DAMAGE_MULTIPLIER;
      else if(stype == "SCORE_MULTIPLIER")
        d->_type = ObjectCollectible::Type::SCORE_MULTIPLIER;
      else if(stype == "SHIELD")
        d->_type = ObjectCollectible::Type::SHIELD;
      else if(stype == "UPGRADEMATERIAL_ATTACK")
        d->_type = ObjectCollectible::Type::UPGRADEMATERIAL_ATTACK;
      else if(stype == "UPGRADEMATERIAL_DEFENSE")
        d->_type = ObjectCollectible::Type::UPGRADEMATERIAL_DEFENSE;
      else if(stype == "UPGRADEMATERIAL_PHYSICAL")
        d->_type = ObjectCollectible::Type::UPGRADEMATERIAL_PHYSICAL;
      else if(stype == "WARP_FUEL")
        d->_type = ObjectCollectible::Type::WARP_FUEL;
      else if(stype == "HUMAN")
        d->_type = ObjectCollectible::Type::HUMAN;
      else
        assert(false);

      assert(def["random_chance"].is_number());
      assert(def["bonus_min"].is_number());
      assert(def["bonus_max"].is_number());
      d->_random_chance = def["random_chance"].number_value();
      d->_bonus_min = def["bonus_min"].number_value();
      d->_bonus_max = def["bonus_max"].number_value();

      _definitions.push_back(d);
    }
}


Loot::Item * Loot::CreateLootItem(double type_random, double bonus_random)
{
  for(auto def : _definitions)
    {
      if(type_random < def->_random_chance)
        {
          auto rv = new Item();
          rv->_type = def->_type;
          rv->_bonus = def->_bonus_min + (def->_bonus_max - def->_bonus_min) * bonus_random;
          return rv;
        }
      type_random -= def->_random_chance;
    }
  return nullptr;
}

