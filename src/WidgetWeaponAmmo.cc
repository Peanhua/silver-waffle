/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "WidgetWeaponAmmo.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include "Weapon.hh"


WidgetWeaponAmmo::WidgetWeaponAmmo(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship)
  : WidgetMeshRenderer(parent, position, size),
    _last_ammo(0)
{
  _draw_empty = false;
  SetMesh(AssetLoader->LoadMesh("Bomb"));
  SetModel(glm::translate(glm::mat4(1), {0, 0, 0.1f}));
  SetView(glm::lookAt(glm::vec3(0.0f, -1.0f, 0.1f), {0, 0, 0}, {0, 0, 1}));
  SetSpaceship(spaceship);
  Graphics->QueueUpdateGPU(this);
  Update();
}


void WidgetWeaponAmmo::SetSpaceship(ObjectSpaceship * spaceship)
{
  _spaceship = spaceship;
  Update();
}


void WidgetWeaponAmmo::Tick(double deltatime)
{
  WidgetMeshRenderer::Tick(deltatime);

  auto weapon = GetWeapon();
  if(weapon)
    if(weapon->GetAmmoAmount() != _last_ammo)
      Update();
}


void WidgetWeaponAmmo::Update()
{
  _offsets.clear();
  
  auto weapon = GetWeapon();
  if(weapon)
    {
      _last_ammo = weapon->GetAmmoAmount();
      for(int i = 0; i < static_cast<int>(_last_ammo); i++)
        _offsets.emplace_back(0, i * GetSize().y / 4);
    }
  else
    _last_ammo = 0;

  SetMultiRender(_offsets);
}


Weapon * WidgetWeaponAmmo::GetWeapon() const
{
  auto weapons = _spaceship->GetWeapons(1);
  if(weapons.size() == 0)
    return nullptr;

  return weapons[0];
}
