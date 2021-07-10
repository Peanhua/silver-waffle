#ifndef SCENE_PLANET_HH_
#define SCENE_PLANET_HH_

#include "Scene.hh"

class Camera;


class ScenePlanet : public Scene
{
public:
  ScenePlanet();

  void      Draw(const Camera & camera) const override;
  void      SetupPlayer() override;
  glm::vec3 GetRandomSpawnPosition() override;

protected:
  void      SetupSceneObject(Object * object, bool destroy_on_block) override;
};

#endif
