#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_

#include "GameState.hh"
#include <vector>

class Camera;
class Milkyway;
class Object;
class SpaceParticles;


class GameStateTitle : public GameState
{
public:
  GameStateTitle();

  void Tick(double deltatime) override;
  
private:
  Milkyway *       _milkyway;
  SpaceParticles * _particles;
  Camera *         _camera;
  std::vector<Object *> _planets;
};

#endif
