#ifndef GAME_STATE_GAME_HH_
#define GAME_STATE_GAME_HH_

#include "GameState.hh"
#include <vector>

class Camera;
class Scene;
class ScoreReel;
class WidgetPlayerShip;


class GameStateGame : public GameState
{
public:
  GameStateGame();
  ~GameStateGame() override;

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;

private:
  double      _fov;
  Camera *    _camera;
  Scene *     _scene;
  ScoreReel * _score_reel;
  
  unsigned int          _lives;
  std::vector<Widget *> _lives_widgets;

  void OnPlayerDies();
  void OnLivesUpdated();
};

#endif
