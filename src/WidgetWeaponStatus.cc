/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetWeaponStatus.hh"
#include "MeshProgressBar.hh"
#include "ObjectSpaceship.hh"
#include "Weapon.hh"


WidgetWeaponStatus::WidgetWeaponStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship)
  : WidgetSpaceshipStatus(parent, position, size, spaceship),
    _weaponcount(0)
{
  UpdateMeter();
}


void WidgetWeaponStatus::Tick(double deltatime)
{
  Widget::Tick(deltatime);
  UpdateMeter();
}


void WidgetWeaponStatus::Draw() const
{
  float i = 0;
  float count = static_cast<float>(_spaceship->GetWeaponCount(0));
  float width = static_cast<float>(GetSize().x);
  for(auto weapon : _spaceship->GetWeapons(0))
    {
      _meter->SetValue(static_cast<float>(std::clamp(weapon->GetHeat(), 0.0, 1.0)));
      auto model = glm::translate(GetModel(), {i * width / count, 0.0f, 0.0f});
      _meter->Draw(model, GetProjection() * GetView() * model);
      i += 1.0f;
    }

  Widget::Draw();
}


void WidgetWeaponStatus::SetSpaceship(ObjectSpaceship * spaceship)
{
  WidgetSpaceshipStatus::SetSpaceship(spaceship);
  UpdateMeter(true);
}


void WidgetWeaponStatus::UpdateMeter(bool force)
{
  const auto count = _spaceship->GetWeaponCount(0);
  if(_weaponcount == count && !force)
    return;
  _weaponcount = count;
  
  delete _meter;

  _meter = new MeshProgressBar(static_cast<float>(GetSize().x) / static_cast<float>(count), static_cast<float>(GetSize().y), glm::vec3(1, 0, 0));
}
