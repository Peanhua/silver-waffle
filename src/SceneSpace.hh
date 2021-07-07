#ifndef SCENE_SPACE_HH_
#define SCENE_SPACE_HH_

#include "Scene.hh"

class Milkyway;
class SpaceParticles;
class WormholeWall;


class SceneSpace : public Scene
{
public:
  SceneSpace();

  void Draw(const Camera & camera) const override;
  void Tick(double deltatime) override;
  void SetupPlayer() override;
  
private:
  Milkyway *     _milkyway;
  WormholeWall * _wall;
};

#endif
