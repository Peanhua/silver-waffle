#include "WidgetWeaponStatus.hh"
#include "MeshProgressBar.hh"
#include "ObjectSpaceship.hh"


WidgetWeaponStatus::WidgetWeaponStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : WidgetSpaceshipStatus(parent, position, size)
{
  delete _meter;
  _meter = new MeshProgressBar(GetSize().x, GetSize().y, glm::vec3(1, 0, 0));
}


void WidgetWeaponStatus::Draw() const
{
  _meter->SetValue(std::clamp(_spaceship->GetWeaponHeat(0), 0.0, 1.0));
  _meter->Draw(GetModel(), GetView(), GetProjection(), GetMVP());
  
  Widget::Draw();
}

