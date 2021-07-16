/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetSpaceshipStatus.hh"
#include "Mesh.hh"
#include "MeshProgressBar.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"


WidgetSpaceshipStatus::WidgetSpaceshipStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, ObjectSpaceship * spaceship)
  : Widget(parent, position, size),
    _spaceship(spaceship)
{
  _meter = new MeshProgressBar(static_cast<float>(GetSize().x), static_cast<float>(GetSize().y), glm::vec3(0, 1, 0));
}


void WidgetSpaceshipStatus::Draw() const
{
  _meter->SetValue(static_cast<float>(std::clamp(_spaceship->GetHealth() / _spaceship->GetMaxHealth(), 0.0, 1.0)));
  _meter->Draw(GetModel(), GetMVP());
  
  Widget::Draw();
}


void WidgetSpaceshipStatus::SetSpaceship(ObjectSpaceship * spaceship)
{
  _spaceship = spaceship;
}


ObjectSpaceship * WidgetSpaceshipStatus::GetSpaceship() const
{
  return _spaceship;
}

