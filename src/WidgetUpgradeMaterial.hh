#ifndef WIDGET_UPGRADE_MATERIAL_HH_
#define WIDGET_UPGRADE_MATERIAL_HH_
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
#include "Widget.hh"
#include "UpgradeMaterial.hh"


class WidgetUpgradeMaterial : public Widget, Observer<UpgradeMaterial>
{
public:
  WidgetUpgradeMaterial(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                        UpgradeMaterial::Type type);
  WidgetUpgradeMaterial(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                        UpgradeMaterial & upgradematerial);

  UpgradeMaterial::Type GetUpgradeMaterialType() const;
  void SetUpgradeMaterialAmount(unsigned int amount);

private:
  UpgradeMaterial::Type _type;
  UpgradeMaterial *     _upgradematerial;

  void OnNotifyUpdate(UpgradeMaterial * material);
};

#endif
