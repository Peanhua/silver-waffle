#include "ObjectCloud.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


ObjectCloud::ObjectCloud(Scene * scene, unsigned int random_seed)
  : ObjectMovable(scene, random_seed)
{
  SetIsAffectedByGravity(false);

  int meshtype = GetRand() < 0.67f ? 1 : 2;
  auto cmesh = AssetLoader->LoadMesh("Cloud" + std::to_string(meshtype));
  SetMesh(new Mesh(0));
  GetMesh()->AddChild(cmesh);
  GetMesh()->SetShaderProgram(cmesh->GetShaderProgram());
  glm::vec3 scale {
    5.0f + 5.0f * GetRand(),
    5.0f + 5.0f * GetRand(),
    5.0f + 5.0f * GetRand()
  };
  float angle = (GetRand() - 0.5f) * 45.0f;
  if(GetRand() < 0.5f)
    angle += 180.0f;
  glm::mat4 t = glm::toMat4(glm::angleAxis(glm::radians(angle), glm::vec3(0, 0, 1)));
  t = glm::scale(t, scale);
  GetMesh()->ApplyTransform(t);
  
  auto cursize = cmesh->GetBoundingBoxHalfSize();
  cursize *= scale;
  GetMesh()->SetBoundingSphereRadius(glm::length(cursize));

  auto c = glm::vec3(0.3f + 0.7f * GetRand(),
                     0.5f,
                     0.3f + 0.7f * GetRand());
  SetColor(c);

  SetCollisionChannels(0);
  SetCollidesWithChannels(0);
  SetUseHealth(false);
  AddImpulse({10.0f * GetRand(), 0, 0});
  
  SetDrag(0.01);
}
