#ifndef GAME_STATE_GAME_HH_
#define GAME_STATE_GAME_HH_

#include "GameState.hh"
#include "Camera.hh"
#include "Scene.hh"

class GameStateGame : public GameState
{
public:
  GameStateGame();
  ~GameStateGame() override;

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;

private:
  double   _fov;
  Camera * _camera;
  Scene *  _scene;
  bool     _player_control_left;
  bool     _player_control_right;
};

#endif
