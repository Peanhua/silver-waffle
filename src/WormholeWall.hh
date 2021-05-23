#ifndef WORMHOLE_WALL_HH_
#define WORMHOLE_WALL_HH_

#include "glm.hh"

class Mesh;


class WormholeWall
{
public:
  WormholeWall(double length, double height);

  void Tick(double deltatime);
  void Draw(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & mvp) const;
  
private:
  Mesh * _mesh;
};

#endif
