#ifndef GAME_STATE_GAME_HH_
#define GAME_STATE_GAME_HH_

#include "GameState.hh"
#include <vector>
#include <random>

class AdditiveBlending;
class Camera;
class GaussianBlur;
class Image;
class Level;
class Mesh;
class Scene;
class ScoreReel;
class TextureRenderer;
class UpgradeMaterial;
class WidgetPlayerShip;
class WidgetSpaceshipStatus;
class WidgetSpaceshipUpgradeStatus;


class GameStateGame : public GameState
{
public:
  GameStateGame();
  ~GameStateGame() override;

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;

private:
  enum class State
    {
      RUNNING,
      DEATH_PAUSE,
      FULL_PAUSE
    };
  
  State _state;
  bool  _state_death_pause_key_eaten;
  std::mt19937_64                       _random;
  std::uniform_real_distribution<float> _rdist;
  double      _fov;
  Camera *    _camera;
  Scene *     _scene;
  TextureRenderer * _texture_renderer;
  Mesh *      _overlay_mesh;
  GaussianBlur * _blur;
  AdditiveBlending * _blender;
  ScoreReel * _score_reel;
  std::vector<Level *> _levels;
  unsigned int         _current_level;
  unsigned int _score_multiplier;
  double       _score_multiplier_timer;
  std::vector<UpgradeMaterial *> _upgradematerials;
  
  unsigned int          _lives;
  std::vector<Widget *> _lives_widgets;
  std::vector<WidgetSpaceshipStatus *> _player_status_widgets;
  std::vector<WidgetSpaceshipUpgradeStatus *> _player_upgrade_status_widgets;
  std::vector<Widget *> _active_bonus_widgets;
  Widget * _pausebutton;
  std::vector<Widget *> _upgradematerial_widgets;

  void ChangeState(State new_state);
  void OnPlayerDies();
  void OnLivesUpdated();
  void OnLevelChanged();
  void NextLifeOrQuit();
};

#endif
