#include "WidgetSpaceshipStatus.hh"
#include "Mesh.hh"
#include "MeshProgressBar.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"


WidgetSpaceshipStatus::WidgetSpaceshipStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size),
    _spaceship(nullptr)
{
  _meter = new MeshProgressBar(static_cast<float>(GetSize().x), static_cast<float>(GetSize().y), glm::vec3(0, 1, 0));
}


void WidgetSpaceshipStatus::Draw() const
{
  _meter->SetValue(static_cast<float>(glm::clamp(_spaceship->GetHealth() / _spaceship->GetMaxHealth(), 0.0, 1.0)));
  _meter->Draw(GetModel(), GetView(), GetProjection(), GetMVP());
  
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

