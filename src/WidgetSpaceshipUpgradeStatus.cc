/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetSpaceshipUpgradeStatus.hh"
#include "MeshProgressBar.hh"
#include "SpaceshipUpgrade.hh"


WidgetSpaceshipUpgradeStatus::WidgetSpaceshipUpgradeStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size, SpaceshipUpgrade * upgrade)
  : Widget(parent, position, size),
    _upgrade(upgrade)
{
  _meter = new MeshProgressBar(static_cast<float>(GetSize().x), static_cast<float>(GetSize().y), glm::vec3(0, 1, 0));
}


void WidgetSpaceshipUpgradeStatus::Draw() const
{
  double value = 0.0;
  switch(_upgrade->GetType())
    {
    case SpaceshipUpgrade::Type::WARP_ENGINE:
      if(_upgrade->GetTimerMax() > 0.0)
        value = _upgrade->GetTimer() / _upgrade->GetTimerMax();
      break;
    default:
      if(_upgrade->IsActive())
        value = 1.0 - _upgrade->GetTimer() / _upgrade->GetTimerMax();
      else
        value = _upgrade->GetCooldownRemaining() / _upgrade->GetCooldownMax();
      break;
    }
  value = std::clamp(value, 0.0, 1.0);
  _meter->SetValue(static_cast<float>(value));

  const glm::vec3 low_colors[] =
    {
      glm::vec3(0.78, 0, 1),
      glm::vec3(   1, 0, 0),
    };
  const glm::vec3 hi_colors[] =
    {
      glm::vec3(0.78, 0, 1),
      glm::vec3(   1, 1, 0),
    };
  unsigned int ind = _upgrade->IsActive() ? 1 : 0;
  _meter->SetColor(glm::mix(low_colors[ind], hi_colors[ind], value));
  
  _meter->Draw(GetModel(), GetMVP());
  
  Widget::Draw();
}


void WidgetSpaceshipUpgradeStatus::SetUpgrade(SpaceshipUpgrade * upgrade)
{
  _upgrade = upgrade;
}


SpaceshipUpgrade * WidgetSpaceshipUpgradeStatus::GetUpgrade() const
{
  return _upgrade;
}

