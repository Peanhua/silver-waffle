#ifndef WIDGET_UPGRADE_MATERIAL_HH_
#define WIDGET_UPGRADE_MATERIAL_HH_

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
