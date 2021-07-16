#ifndef UPGRADE_MATERIAL_HH_
#define UPGRADE_MATERIAL_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Observer.hh"
#include <string>


class UpgradeMaterial : public Subject<UpgradeMaterial>
{
public:
  enum class Type
    {
      ATTACK,
      DEFENSE,
      PHYSICAL
    };
  
  UpgradeMaterial(Type type, const std::string & name);

  Type              GetType()   const;
  const std::string GetName()   const;
  unsigned int      GetAmount() const;

  void Add(unsigned int amount);
  void Use(unsigned int amount);
  
  
private:
  Type         _type;
  std::string  _name;
  unsigned int _amount;
};

#endif
