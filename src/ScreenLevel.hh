#ifndef SCREEN_LEVEL_HH_
#define SCREEN_LEVEL_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "Screen.hh"
#include "GameStats.hh"
#include "Weapon.hh"
#include <vector>
#include <random>

class AdditiveBlending;
class Camera;
class GaussianBlur;
class Image;
class Level;
class Mesh;
class QuickTimeEvent;
class Scene;
class ScoreReel;
class TextureRenderer;
class UpgradeMaterial;
class WidgetMeshRenderer;
class WidgetSpaceshipStatus;
class WidgetSpaceshipUpgradeStatus;
class WidgetTeletyper;
class WidgetWeaponAmmo;


class ScreenLevel : public Screen
{
public:
  ScreenLevel(const std::string & music_category, ScreenLevel * parent);
  ~ScreenLevel() override;

  virtual void SetupLevels();

  void Tick(double deltatime) override;
  void Draw()           const override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;
  void OnQuit() override;

  Camera *    GetCamera()       const;
  GameStats * GetGameStats()    const;
  Scene *     GetScene()        const;
  Level *     GetCurrentLevel() const;
  void TransitionToScreen(Screen * new_screen, const std::string & message);

  void OnLivesUpdated();
  void OnHumanCountUpdated();

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
      GAMEOVER_PAUSE,
      SCREEN_TRANSITION
    };

  ScreenLevel * _parent;
  State _state;
  bool  _state_death_pause_key_eaten;
  QuickTimeEvent * _current_quicktimeevent;
  std::mt19937_64                       _random;
  std::uniform_real_distribution<float> _rdist;
  TextureRenderer *  _texture_renderer;
  TextureRenderer *  _texture_renderer_warpspeed;
  Mesh *             _overlay_mesh;
  GaussianBlur *     _blur;
  GaussianBlur *     _blurcolor;
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
  WidgetMeshRenderer *  _human_widget;
  std::vector<glm::ivec2> _human_offsets;
  Widget *              _weapon_type_widget;
  Widget *              _weapon_boost_widget;
  WidgetWeaponAmmo *    _bomb_ammo_widget;
  
  bool _demo_lander_activated;
  bool _demo_ending_activated;
  
  void ChangeState(State new_state);
  void OnPlayerDies();
  void NextLifeOrQuit();

  void OpenSpaceshipMaintenanceUI();
  void OpenPauseUI();

  void GameOver(bool game_was_completed);

  void CopyPlayerData(ScreenLevel * src);
  void SetAmmoType(Weapon::AmmoType type);

  Weapon::AmmoType _weapon_ammo_type;
};

#endif
