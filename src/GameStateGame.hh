#ifndef GAME_STATE_GAME_HH_
#define GAME_STATE_GAME_HH_

#include "GameState.hh"
#include <vector>
#include <random>

class Camera;
class Image;
class Level;
class Mesh;
class Milkyway;
class Scene;
class ScoreReel;
class SpaceParticles;
class WidgetPlayerShip;
class WidgetSpaceshipStatus;


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
  ScoreReel * _score_reel;
  SpaceParticles * _particles;
  Milkyway *       _milkyway;
  std::vector<Level *> _levels;
  unsigned int         _current_level;
  unsigned int _score_multiplier;
  double       _score_multiplier_timer;
  unsigned int _upgradematerial_a;
  unsigned int _upgradematerial_d;
  unsigned int _upgradematerial_p;
  
  unsigned int          _lives;
  std::vector<Widget *> _lives_widgets;
  std::vector<WidgetSpaceshipStatus *> _player_status_widgets;
  std::vector<Widget *> _active_bonus_widgets;
  Widget * _pausebutton;
  Widget * _upgradematerial_a_widget;
  Widget * _upgradematerial_d_widget;
  Widget * _upgradematerial_p_widget;

  void ChangeState(State new_state);
  void OnPlayerDies();
  void OnLivesUpdated();
  void OnLevelChanged();
  void NextLifeOrQuit();
};

#endif
