#ifndef SCENE_SPACE_HH_
#define SCENE_SPACE_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Scene.hh"

class Milkyway;
class SpaceParticles;
class WormholeWall;


class SceneSpace : public Scene
{
public:
  SceneSpace();

  void      Draw(const Camera & camera) const override;
  void      Tick(double deltatime) override;
  void      SetupPlayer() override;
  glm::vec3 GetRandomSpawnPosition() override;
  bool      AreInSameCollisionPartition(Object * a, Object * b) const override;

protected:
  void      SetupSceneObject(Object * object, bool destroy_on_block) override;
  
private:
  Milkyway *     _milkyway;
  WormholeWall * _wall;
};

#endif
