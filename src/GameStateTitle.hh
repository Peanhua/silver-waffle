#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_

#include "GameState.hh"

class Camera;
class SpaceParticles;


class GameStateTitle : public GameState
{
public:
  GameStateTitle();

  void Tick(double deltatime) override;
  
private:
  SpaceParticles * _particles;
  double           _particles_cameramovement_timer;
  float            _particles_vertical_cameramovement;
  Camera *         _particles_camera;
};

#endif
