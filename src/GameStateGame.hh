#ifndef GAME_STATE_GAME_HH_
#define GAME_STATE_GAME_HH_

#include "GameState.hh"
#include "ShaderProgram.hh"
#include "Camera.hh"
#include "GameLevel.hh"

class GameStateGame : public GameState
{
public:
  GameStateGame();
  ~GameStateGame() override;

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;

private:
  double          _fov;
  ShaderProgram * _shaderprog;
  Camera *        _camera;
  GameLevel *     _level;
};

#endif
