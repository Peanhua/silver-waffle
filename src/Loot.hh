#ifndef LOOT_HH_
#define LOOT_HH_

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
