#ifndef GAME_STATE_GAME_HH_
#define GAME_STATE_GAME_HH_

#include "GameState.hh"
#include <vector>
#include <random>

class Camera;
class Image;
class Level;
class Mesh;
class Scene;
class ScoreReel;
class Starfield;
class WidgetPlayerShip;


class GameStateGame : public GameState
{
public:
  GameStateGame();
  ~GameStateGame() override;

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;

private:
  std::mt19937_64                       _random;
  std::uniform_real_distribution<float> _rdist;
  double      _fov;
  Camera *    _camera;
  Scene *     _scene;
  ScoreReel * _score_reel;
  Starfield * _starfield;
  Mesh *      _space;
  std::vector<Level *> _levels;
  unsigned int         _current_level;
  
  unsigned int          _lives;
  std::vector<Widget *> _lives_widgets;

  Widget * _playership_status_widget;

  void OnPlayerDies();
  void OnLivesUpdated();
  void OnLevelChanged();
};

#endif
