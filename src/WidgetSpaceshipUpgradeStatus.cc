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
  double value;
  if(_upgrade->IsActive())
    value = 1.0 - _upgrade->GetTimer() / _upgrade->GetTimerMax();
  else
    value = _upgrade->GetCooldownRemaining() / _upgrade->GetCooldownMax();
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
  
  _meter->Draw(GetModel(), GetView(), GetProjection(), GetMVP());
  
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

