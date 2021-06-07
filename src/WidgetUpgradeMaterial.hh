#ifndef WIDGET_UPGRADE_MATERIAL_HH_
#define WIDGET_UPGRADE_MATERIAL_HH_

#include "Widget.hh"
#include "UpgradeMaterial.hh"


class WidgetUpgradeMaterial : public Widget
{
public:
  WidgetUpgradeMaterial(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                        UpgradeMaterial::Type type);

  UpgradeMaterial::Type GetUpgradeMaterialType() const;
  void SetUpgradeMaterialAmount(unsigned int amount);

private:
  UpgradeMaterial::Type _type;
};

#endif
