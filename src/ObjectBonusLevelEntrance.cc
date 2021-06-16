#include "ObjectBonusLevelEntrance.hh"
#include "GameStateBonusLevel.hh"
#include "Mesh.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGameState.hh"


ObjectBonusLevelEntrance::ObjectBonusLevelEntrance(Scene * scene, double enemy_difficulty, double warp_fuel_bonus)
  : ObjectMovable(scene),
    _enemy_difficulty(enemy_difficulty),
    _warp_fuel_bonus(warp_fuel_bonus),
    _drag_timer(0),
    _used(false)
{
  SetMesh(new Mesh(*AssetLoader->LoadMesh("BonusLevelEntrance")));
  GetMesh()->SetShaderProgram(AssetLoader->LoadShaderProgram("BonusLevelEntrance"), true);
  SetCollisionSphereRadius(0.5);
  AddToCollisionChannel(CollisionChannel::COLLECTIBLE);
  AddCollidesWithChannel(CollisionChannel::PLAYER);

  GetMesh()->SetAllColor(glm::vec4(enemy_difficulty, 0.0f, warp_fuel_bonus, 0.2f), true);
  GetMesh()->UpdateGPU();
}


#if 0
void ObjectBonusLevelEntrance::Tick(double deltatime)
{
  ObjectMovable::Tick(deltatime);

  _drag_timer -= deltatime;
  if(_drag_timer < 0.0)
    {
      assert(_scene);
      auto objs = _scene->GetNearbyObjects(GetPosition(), 10);
      for(auto obj : *objs)
        if(obj && obj != this)
          {
            float direction = GetPosition().x - obj->GetPosition().x;
            if(std::abs(direction) > 0.1f)
              obj->AddImpulse(glm::vec3(direction / std::abs(direction) * 1.0f, 0, 0));
          }
      delete objs;
      
      _drag_timer += 0.5;
    }
}
#endif


void ObjectBonusLevelEntrance::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  assert(hit_direction == hit_direction);

  if(_used)
    return;

  auto player = dynamic_cast<ObjectSpaceship *>(&other);
  if(!player)
    return;

  _used = true;

  auto current = dynamic_cast<GameStateGame *>(GameStateManager->GetGameState());
  if(current)
    {
      auto ns = new GameStateBonusLevel(current, _enemy_difficulty, _warp_fuel_bonus);
      ns->SetupLevels();
      GameStateManager->GetGameState()->SetChildState(ns);
    }
}
