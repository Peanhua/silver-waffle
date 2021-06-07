#include "WidgetUpgradeMaterial.hh"
#include "SubsystemAssetLoader.hh"


WidgetUpgradeMaterial::WidgetUpgradeMaterial(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size,
                                             UpgradeMaterial::Type type)
  : Widget(parent, position, size),
    _type(type)
{
  SetImage("UpgradeMaterial");
  SetTextFont(AssetLoader->LoadFont(10));
  SetTextFontWeight(0.5);
  SetIsFocusable(false);

  switch(type)
    {
    case UpgradeMaterial::Type::ATTACK:
      SetImageColor(glm::vec4(1, 0, 0, 1));
      SetTextColor(glm::vec4(0, 1, 1, 1));
      break;
    case UpgradeMaterial::Type::DEFENSE:
      SetImageColor(glm::vec4(0, 0, 1, 1));
      SetTextColor(glm::vec4(1, 1, 0, 1));
      break;
    case UpgradeMaterial::Type::PHYSICAL:
      SetImageColor(glm::vec4(1, 0.612, 0, 1));
      SetTextColor(glm::vec4(0, 0.388, 1, 1));
      break;
    }
}


void WidgetUpgradeMaterial::SetUpgradeMaterialAmount(unsigned int amount)
{
  SetText(std::to_string(amount));
  SetTextPaddingCentered(true, true);
}


UpgradeMaterial::Type WidgetUpgradeMaterial::GetUpgradeMaterialType() const
{
  return _type;
}

