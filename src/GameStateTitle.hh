#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_

#include "GameState.hh"

class Camera;
class Starfield;


class GameStateTitle : public GameState
{
public:
  GameStateTitle();

  void Tick(double deltatime) override;
  
private:
  Starfield * _starfield;
  double      _starfield_cameramovement_timer;
  float       _starfield_vertical_cameramovement;
  Camera *    _starfield_camera;
};

#endif
