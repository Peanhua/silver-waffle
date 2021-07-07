#ifndef SCENE_PLANET_HH_
#define SCENE_PLANET_HH_

#include "Scene.hh"


class ScenePlanet : public Scene
{
public:
  ScenePlanet();
  
  void SetupPlayer() override;
  
};

#endif
