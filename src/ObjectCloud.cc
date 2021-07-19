#include "ObjectCloud.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


ObjectCloud::ObjectCloud(Scene * scene, unsigned int random_seed)
  : ObjectMovable(scene, random_seed)
{
  SetIsAffectedByGravity(false);
  SetMesh(new Mesh(*AssetLoader->LoadMesh("Cloud1")));
  GetMesh()->SetShaderProgram(AssetLoader->LoadShaderProgram("Cloud"), true);
  glm::vec3 scale {
    5.0f + 5.0f * GetRand(),
    5.0f + 5.0f * GetRand(),
    5.0f + 5.0f * GetRand()
  };
  float angle = (GetRand() - 0.5f) * 30.0f;
  if(GetRand() < 0.5f)
    angle += 180.0f;
  glm::mat4 t = glm::toMat4(glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1)));
  t = glm::scale(t, scale);
  GetMesh()->ApplyTransform(t);
  GetMesh()->UpdateGPU();
  GetMesh()->CalculateBoundingVolumes();
  SetCollisionChannels(0);
  SetCollidesWithChannels(0);
  SetUseHealth(false);
  AddImpulse({10.0f * GetRand(), 0, 0});
}


void ObjectCloud::Tick(double deltatime)
{
  auto vel = GetVelocity();
  if(vel.x < 0.1f)
    vel.x += 0.0001f;
  else if(vel.x > 0.1f)
    vel.x -= 0.0001f;
  
  ObjectMovable::Tick(deltatime);
}
