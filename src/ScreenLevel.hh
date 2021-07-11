#ifndef SCREEN_LEVEL_HH_
#define SCREEN_LEVEL_HH_

#include "Screen.hh"
#include "GameStats.hh"
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
class WidgetTeletyper;


class ScreenLevel : public Screen
{
public:
  ScreenLevel(ScreenLevel * parent);
  ~ScreenLevel() override;

  virtual void SetupLevels();

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;
  void OnQuit() override;

  GameStats * GetGameStats() const;
  Scene *     GetScene()     const;
  void TransitionToScreen(Screen * new_screen, const std::string & message);

  void OnLivesUpdated();
  
protected:
  Camera *             _camera;
  Scene *              _scene;
  std::vector<Level *> _levels;

  void Initialize();
  void OnLevelChanged();
  void RefreshUI();
  
private:
  enum class State
    {
      RUNNING,
      DEATH_PAUSE,
      FULL_PAUSE,
      SCREEN_TRANSITION
    };

  ScreenLevel * _parent;
  State _state;
  bool  _state_death_pause_key_eaten;
  std::mt19937_64                       _random;
  std::uniform_real_distribution<float> _rdist;
  TextureRenderer *  _texture_renderer;
  Mesh *             _overlay_mesh;
  GaussianBlur *     _blur;
  AdditiveBlending * _blender;
  ScoreReel *        _score_reel;
  unsigned int       _current_level;
  GameStats *        _gamestats;

  Screen * _screentransition_new_screen;
  double      _screentransition_timer;
  std::string _screentransition_text;
  
  std::vector<Widget *> _lives_widgets;
  std::vector<WidgetSpaceshipStatus *> _player_status_widgets;
  std::vector<WidgetSpaceshipUpgradeStatus *> _player_upgrade_status_widgets;
  std::vector<Widget *> _active_bonus_widgets;
  Widget *              _pausebutton;
  Widget *              _levelinfo_widget;
  WidgetTeletyper *     _teletyper;

  void ChangeState(State new_state);
  void OnPlayerDies();
  void NextLifeOrQuit();

  void OpenSpaceshipMaintenanceUI();
  void OpenPauseUI();

  void GameOver(bool game_was_completed);

  void CopyPlayerData(ScreenLevel * src);
};

#endif