#include "WidgetSpaceshipStatus.hh"
#include "Mesh.hh"
#include "MeshProgressBar.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"


WidgetSpaceshipStatus::WidgetSpaceshipStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size),
    _spaceship(nullptr)
{
  _meter = new MeshProgressBar(GetSize().x, GetSize().y, glm::vec3(0, 1, 0));
}


void WidgetSpaceshipStatus::Draw() const
{
  _meter->SetValue(glm::clamp(_spaceship->GetHealth() / 100.0, 0.0, 1.0));
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

