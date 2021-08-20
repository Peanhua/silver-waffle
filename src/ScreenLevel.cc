/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#ifdef HAVE_CONFIG_H
# include "../config.h"
#endif
#include "ScreenLevel.hh"
#include "AdditiveBlending.hh"
#include "Camera.hh"
#include "Controller.hh"
#include "Font.hh"
#include "GaussianBlur.hh"
#include "HighscoreEntry.hh"
#include "Image.hh"
#include "Level.hh"
#include "MeshOverlay.hh"
#include "MusicPlayer.hh"
#include "ObjectPlanet.hh"
#include "ObjectSpaceship.hh"
#include "QuickTimeEventLaunchToSpace.hh"
#include "Scene.hh"
#include "ScoreReel.hh"
#include "SolarSystemObject.hh"
#include "SpaceshipControlProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemHighscores.hh"
#include "SubsystemSettings.hh"
#include "SubsystemSfx.hh"
#include "TextureRenderer.hh"
#include "UpgradeMaterial.hh"
#include "Weapon.hh"
#include "WidgetButton.hh"
#include "WidgetHighscores.hh"
#include "WidgetMeshRenderer.hh"
#include "WidgetScoreReel.hh"
#include "WidgetSpaceshipMaintenance.hh"
#include "WidgetSpaceshipStatus.hh"
#include "WidgetSpaceshipUpgradeStatus.hh"
#include "WidgetTeletyper.hh"
#include "WidgetUpgradeMaterial.hh"
#include "WidgetWeaponStatus.hh"
#ifdef WITH_VALGRIND
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wuseless-cast"
# include <callgrind.h>
# pragma GCC diagnostic pop
#endif
#include <iostream>


ScreenLevel::ScreenLevel(const std::string & music_category, ScreenLevel * parent)
  : Screen(music_category),
    _parent(parent),
    _state(State::RUNNING),
    _current_quicktimeevent(nullptr),
    _random(0),
    _rdist(0, 1),
    _current_level(0),
    _gamestats(nullptr),
    _pausebutton(nullptr),
    _human_widget(nullptr),
    _weapon_type_widget(nullptr),
    _weapon_boost_widget(nullptr),
    _demo_lander_activated(false),
    _demo_ending_activated(false),
    _weapon_ammo_type(Weapon::AmmoType::KINETIC)
{
}


void ScreenLevel::Initialize()
{
  _texture_renderer = new TextureRenderer(static_cast<unsigned int>(Settings->GetInt("screen_width")),
                                          static_cast<unsigned int>(Settings->GetInt("screen_height")),
                                          2);
  _overlay_mesh = new MeshOverlay();
  _overlay_mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));

  _blur = new GaussianBlur();
  _blender = new AdditiveBlending();
    
  if(_parent)
    {
      CopyPlayerData(_parent);
      SetAmmoType(_parent->_weapon_ammo_type);
      _gamestats = _parent->GetGameStats();
      _scene->EnableTutorialMessages(false);
    }

  if(!_gamestats)
    _gamestats = new GameStats();
  
  _scene->GetPlayer()->SetOwnerGameStats(_gamestats);

  _scene->GetPlayer()->SetOnDestroyed([this]([[maybe_unused]] Object * destroyer)
  {
    OnPlayerDies();
  });

  auto splayer = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
  if(splayer)
    splayer->SetOnHumanCountChanged([this]()
    {
      OnHumanCountUpdated();
    });

 
  // UI:
  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(width, height));
  assert(root);
  root->SetIsFocusable(false);
  SetRootWidget(root);

  _score_reel = new ScoreReel(6);
  new WidgetScoreReel(root, glm::ivec2(0, 30), glm::ivec2(350, 50), _score_reel);

  {
    auto mesh = AssetLoader->LoadMesh("Player");
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    glm::mat4 model(1);
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
    model = glm::translate(model, glm::vec3(0.1, 0, 0));
    
    for(int i = 0; i < 5; i++)
      {
        auto w = new WidgetMeshRenderer(root, glm::ivec2(width - 64 - i * 50, 10), glm::ivec2(50, 50), mesh, model, view);
        _lives_widgets.push_back(w);
      }
    OnLivesUpdated();
  }

  {
    glm::mat4 model(1);
    model = glm::translate(model, {0, 0, -0.25f});
    glm::mat4 view = glm::lookAt(glm::vec3(0, -1.4f, 0.2f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    auto mesh = AssetLoader->LoadMesh("Human");
    _human_widget = new WidgetMeshRenderer(root, {0, 0}, glm::ivec2(100, 100), mesh, model, view);
  }
  OnHumanCountUpdated();

  if(splayer)
    {
      int x = static_cast<int>(width) - 8;
      int y = 70;
      int wtw_x;
      const glm::ivec2 size(20, 100);
      std::vector<Widget *>    widgets;
      std::vector<std::string> labels;
      {
        x -= size.x + 4;
        auto w = new WidgetSpaceshipStatus(root, {x, y}, size, splayer);
        _player_status_widgets.push_back(w);
        widgets.push_back(w);
        labels.push_back("H");
      }
      {
        x -= size.x + 4;
        auto w = new WidgetWeaponStatus(root, {x, y}, size, splayer);
        _player_status_widgets.push_back(w);
        widgets.push_back(w);
        labels.push_back("W");
        wtw_x = x;
      }
      {
        x -= size.x + 4;
        auto w = new WidgetSpaceshipUpgradeStatus(root, {x, y}, size, splayer->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE));
        _player_upgrade_status_widgets.push_back(w);
        widgets.push_back(w);
        labels.push_back("Z");
      }
      for(unsigned int i = 0; i < labels.size(); i++)
        {
          auto l = new Widget(root, widgets[i]->GetPosition() + glm::ivec2(0, size.y), glm::ivec2(size.x, size.x));
          l->SetText(labels[i]);
          l->SetTextFont(AssetLoader->LoadFont(12));
          l->SetTextColor(glm::vec3(0.0, 0.5, 0.0));
          l->SetTextPaddingCentered(true, true);
        }

      y += size.y + 20;

      _weapon_type_widget = new Widget(root, {wtw_x, y}, {size.x, size.x});
      y += size.x;
      _weapon_boost_widget = new Widget(root, {wtw_x, y}, {size.x, size.x});
      _weapon_boost_widget->AddImage("WeaponBoostLiquid");
      _weapon_boost_widget->AddImage("WeaponBoostLiquidActive");
      _weapon_boost_widget->SetCurrentImage(0);
      _weapon_boost_widget->SetIsVisible(false);
    }
  
  {
    std::vector<std::string> imagenames
      {
        "BonusIcon-2xScore",
        "BonusIcon-Shield",
      };
    unsigned int x = 10;
    for(auto imagename : imagenames)
      {
        auto w = new Widget(root, glm::ivec2(x, 70), glm::ivec2(48, 48));
        w->SetImage(imagename);
        w->SetIsVisible(false);
        x += 50;
        _active_bonus_widgets.push_back(w);
      }
  }
  {
    auto w = new WidgetTeletyper(root, glm::ivec2(10, 120), glm::ivec2(200, 100));
    w->SetTextFont(AssetLoader->LoadFont(10));
    w->SetCharactersPerSecond(60);
    w->SetTextColor(glm::vec3(0.0, 0.8, 0.0));
    w->SetIsFocusable(false);
    w->SetPurgingTime(4.0);
    _teletyper = w;
  }

  _levelinfo_widget = new Widget(root, glm::ivec2(10, 10), glm::ivec2(300, 25));
  _levelinfo_widget->SetTextColor(glm::vec3(1.00, 0.88, 0.00));
  _levelinfo_widget->SetTextFont(AssetLoader->LoadFont(12));

  _scene->DumpStats();
  
  SetAmmoType(_weapon_ammo_type);

  if(Settings->GetBool("cheat_unlock_all_upgrades"))
    {
      std::vector<SpaceshipUpgrade::Type> all
        {
          SpaceshipUpgrade::Type::WEAPON,
          SpaceshipUpgrade::Type::WEAPON_COOLER,
          SpaceshipUpgrade::Type::ENGINE_UPGRADE,
          SpaceshipUpgrade::Type::HULL_UPGRADE,
          SpaceshipUpgrade::Type::REPAIR_DROID,
          SpaceshipUpgrade::Type::WARP_ENGINE,
          SpaceshipUpgrade::Type::PLANET_LANDER,
        };
      for(auto u : all)
        _gamestats->UnlockSpaceshipUpgrade(u);
    }
}


ScreenLevel::~ScreenLevel()
{
  delete _scene;
  delete _camera;
}


void ScreenLevel::Tick(double deltatime)
{
  if(Settings->GetBool("demo"))
    {
      auto player = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
      assert(player);
      if(!_demo_lander_activated)
        if(!_parent && GetGameStats()->GetTime() > 9.4)
          {
            auto planet = dynamic_cast<ObjectPlanet *>(_scene->GetNextClosestPlanet(player->GetPosition()));
            assert(planet);
            player->DescendToPlanet(planet->GetSolarSystemObject());
            _demo_lander_activated = true;
          }
      auto t = GetGameStats()->GetTime();
      if(_parent)
        {
          if(t > 12 && t < 13)
            {
              player->GetController()->SteerBackward(true);
              player->GetController()->SteerLeft(true);
            }
          else if(t > 16 && t < 17)
            {
              player->GetController()->SteerBackward(false);
              player->GetController()->SteerLeft(false);
            }
          else if(t > 20 && t < 21)
            player->GetController()->SteerBackward(true);
          else if(t > 22 && t < 23)
            player->GetController()->SteerBackward(false);
          else if(t > 24 && t < 25)
            player->SetUseHealth(false);
        }
      if(t > 40.0)
        {
          SDL_Event e;
          e.quit.type = SDL_QUIT;
          e.quit.timestamp = SDL_GetTicks();
          SDL_PushEvent(&e);
        }
    }
      
  
  auto level = _levels[_current_level];
  auto player = _scene->GetPlayer();
  auto splayer = dynamic_cast<ObjectSpaceship *>(player);
  
  _score_reel->SetScore(_gamestats->GetScore());

  if(_state == State::RUNNING)
    {
      _gamestats->Tick(deltatime);
      if(GetGameStats()->GetTime() > 6.0)
        _scene->TutorialMessage(0, "Fire at will.\n");
    }

  if(_state == State::RUNNING || _state == State::DEATH_PAUSE || _state == State::GAMEOVER_PAUSE)
    _scene->Tick(deltatime);

  if(_state == State::RUNNING)
    {
      level->Tick(deltatime);
      if(level->IsFinished())
        {
          if(_current_level + 1 < _levels.size())
            {
              _current_level++;
              OnLevelChanged();
            }
          else
            {
              if(_parent)
                Quit();
              else
                GameOver(true);
            }
        }
    }
  else if(_state == State::SCREEN_TRANSITION)
    {
      player->Tick(deltatime);
      _screentransition_timer += deltatime;

      if(_screentransition_new_screen)
        {
          {
            auto amount = std::clamp(_screentransition_timer - 0.5, 0.0, 1.0);
            _pausebutton->SetImageColor(glm::vec4(0, 0, 0, amount));
            double v = 0;
            if(_screentransition_timer < 2.0)
              v = std::clamp(1.0 - _screentransition_timer / 2.0, 0.0, 1.0);
            _pausebutton->SetTextColor(glm::vec3(v, v, v));
            _pausebutton->SetText(_screentransition_text);
            _pausebutton->SetTextPaddingCentered(true, true);
          }
          
          if(splayer && splayer->GetActiveControlProgramCount() == 0)
            { // Enter the new screen.
              SetChild(_screentransition_new_screen);
              _screentransition_new_screen = nullptr;
              _pausebutton->SetText("");
              _screentransition_timer = 0;
              { // Set resume animation here.
                auto p = new SCP_Pitch(splayer, 180, 0.5);
                splayer->AddControlProgram(p);
              }
            }
        }
      else
        { // Resumed from the new screen.
          _scene->DestroyAllEnemies();
          if(GetGameStats()->GetLives() == 0)
            GameOver(false);
          else
            {
              if(_screentransition_timer < 0.5)
                {
                  auto amount = std::clamp(1.0 - _screentransition_timer / 0.5, 0.0, 1.0);
                  _pausebutton->SetImageColor(glm::vec4(0, 0, 0, amount));
                }
              if(splayer && splayer->GetActiveControlProgramCount() == 0)
                {
                  player->SetPosition(glm::vec3(player->GetPosition().x, 40 - 53, 0));
                  player->SetOrientation(glm::quat(1, 0, 0, 0));
                  RefreshUI();
                  OnLivesUpdated();
                  OnHumanCountUpdated();
                  ChangeState(State::RUNNING);
                }
            }
        }
    }

  {
    auto bd = splayer->GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE);
    _weapon_boost_widget->SetIsVisible(bd->GetRawValue() > 0);
    _weapon_boost_widget->SetCurrentImage(bd->IsActive() ? 1 : 0);
  }
  _active_bonus_widgets[0]->SetIsVisible(_gamestats->GetScoreMultiplier() > 1);
  _active_bonus_widgets[1]->SetIsVisible(splayer->GetUpgrade(SpaceshipUpgrade::Type::SHIELD)->GetValue() > 0);

  auto fmt = [](double v)
  {
    auto rv = std::to_string(v);
    return rv.substr(0, rv.find('.') + 2);
  };

  {
    assert(player);
    auto planet = dynamic_cast<ObjectPlanet *>(_scene->GetNextClosestPlanet(player->GetPosition()));
    if(planet)
      _levelinfo_widget->SetText(planet->GetSolarSystemObject()->GetName()
                                 + " " + fmt(std::abs(player->GetPosition().y - planet->GetPosition().y))
                                 + " (" + fmt(_gamestats->GetTime()) + ")");
    else
      _levelinfo_widget->SetText(level->GetName()
                                 + " (" + fmt(_gamestats->GetTime()) + ")");
  }

  {
    if(splayer && splayer->IsAlive())
      if(splayer->SystemlogGet().length() > 0)
        {
          _teletyper->AppendText(splayer->SystemlogGet());
          splayer->SystemlogClear();
        }
  }

  if(_current_quicktimeevent)
    {
      if(_current_quicktimeevent->IsAlive())
        {
          _current_quicktimeevent->Tick(deltatime);
        }
      else
        {
          delete _current_quicktimeevent;
          _current_quicktimeevent = nullptr;
        }
    }

  Screen::Tick(deltatime);
}


void ScreenLevel::Draw() const
{
  _texture_renderer->BeginRender();
  _camera->Update();
  _camera->stats.elapsed_frames++;
  _scene->Draw(*_camera);
  _texture_renderer->EndRender();
  
  glDisable(GL_DEPTH_TEST);
  auto tid_glow = _blur->Blur(3, _texture_renderer->GetTextureId(1));
  _blender->Blend(_texture_renderer->GetTextureId(0), tid_glow);

  Screen::Draw();
}


void ScreenLevel::OnKeyboard(bool pressed, SDL_Keycode key, [[maybe_unused]] SDL_Keymod mod)
{
  if(_state == State::GAMEOVER_PAUSE)
    {
      if(pressed)
        if(_pausebutton)
          {
            SetModalWidget(nullptr);
            _pausebutton->Destroy();
            _pausebutton = nullptr;
          }
      return;
    }
  
  if(_state == State::DEATH_PAUSE)
    {
      if(key == SDLK_ESCAPE && !pressed)
        NextLifeOrQuit();
      else if(key == SDLK_SPACE)
        {
          if(pressed)
            _state_death_pause_key_eaten = true;
          else
            if(_state_death_pause_key_eaten)
              NextLifeOrQuit();
        }
      return;
    }
  
  if(_state == State::FULL_PAUSE)
    {
      if((key == SDLK_ESCAPE || key == SDLK_TAB) && !pressed)
        ChangeState(State::RUNNING);
      return;
    }

  if(_state == State::SCREEN_TRANSITION)
    return;

  assert(_state == State::RUNNING);

  auto player = _scene->GetPlayer();
  auto splayer = dynamic_cast<ObjectSpaceship *>(player);
  auto controller = player->GetController();

  bool disablecontrols = false;
  if(splayer && splayer->IsAlive())
    {
      if(_scene->IsWarpEngineStarting())
        disablecontrols = true;
    
      auto u = splayer->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
      if(u->IsActive())
        disablecontrols = true;
      else if(splayer->GetActiveControlProgramCount() > 0)
        disablecontrols = true;
    }
  else
    disablecontrols = true;

  if(!disablecontrols)
    if(_current_quicktimeevent)
      disablecontrols = _current_quicktimeevent->OnKeyboard(pressed, key, mod);
  
  switch(key)
    {
    case SDLK_ESCAPE:
      if(!pressed)
        OpenPauseUI();
      break;
      
    case SDLK_TAB:
      if(!pressed)
        OpenSpaceshipMaintenanceUI();
      break;
      
    case SDLK_LEFT:
      if(!disablecontrols)
        controller->SteerLeft(pressed);
      break;
      
    case SDLK_RIGHT:
      if(!disablecontrols)
        controller->SteerRight(pressed);
      break;

    case SDLK_DOWN:
      if(!disablecontrols)
        controller->SteerBackward(pressed);
      break;

    case SDLK_UP:
      if(!disablecontrols)
        controller->SteerForward(pressed);
      break;

    case SDLK_SPACE:
      if(!disablecontrols)
        controller->ActivateWeapon(pressed);
      break;

    case SDLK_w:
      if(!disablecontrols && pressed)
        _scene->StartWarpEngine();
      else
        _scene->StopWarpEngine();
      break;

    case SDLK_s:
      if(!disablecontrols && pressed)
        if(splayer)
          {
            auto lander = splayer->GetUpgrade(SpaceshipUpgrade::Type::PLANET_LANDER);
            if(lander->CanActivate())
              lander->Activate();
          }
      break;

    case SDLK_f:
      if(!disablecontrols && pressed)
        if(splayer)
          {
            auto dboost = splayer->GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE);
            if(dboost->CanActivate())
              dboost->Activate();
          }
      break;

    case SDLK_q:
      std::cout << player->GetPosition() << std::endl;
      break;

    case SDLK_1:
      if(!disablecontrols && pressed)
        SetAmmoType(Weapon::AmmoType::KINETIC);
      break;

    case SDLK_2:
      if(!disablecontrols && pressed)
        SetAmmoType(Weapon::AmmoType::PLASMA);
      break;
      
    case SDLK_l:
      if(!disablecontrols && pressed)
        if(splayer && splayer->IsLanded())
          {
            auto level = _levels[_current_level];
            if(!level->AreBossBuildingsAlive())
              _current_quicktimeevent = new QuickTimeEventLaunchToSpace(splayer, GetRootWidget(), 3);
            else
              splayer->SystemlogAppend("A disturbance in time-space continuity is preventing the launch!\n");
          }
      break;

    case SDLK_F1:
      if(pressed)
        {
#ifdef DEBUG_COLLISION
          auto f = Settings->GetBool("draw_collision");
          Settings->SetBool("draw_collision", !f);
#endif
        }
      break;
    }
}


void ScreenLevel::OnLivesUpdated()
{
  int used = 0;
  for(auto w : _lives_widgets)
    {
      w->SetIsVisible(used < _gamestats->GetLives());
      used++;
    }
}


void ScreenLevel::OnHumanCountUpdated()
{
  auto player = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
  assert(player);
  auto count = static_cast<unsigned int>(player->GetHumanCount());

  _human_widget->SetIsVisible(count > 0);
  if(count == 0)
    return;
  
  while(count < _human_offsets.size())
    _human_offsets.pop_back();

  auto screen_width = Settings->GetInt("screen_width");
  while(count > _human_offsets.size())
    {
      auto distance = 10.0f * static_cast<float>(_human_offsets.size());
      glm::vec2 off(_rdist(_random) - 0.5f, _rdist(_random));
      off = glm::normalize(off) * distance;

      glm::ivec2 pos(screen_width / 2, 10);
      pos += off;
      
      _human_offsets.push_back(pos);
    }
  _human_widget->SetMultiRender(_human_offsets);
}


void ScreenLevel::OnPlayerDies()
{
  if(Settings->GetBool("demo"))
    {
      SDL_Event e;
      e.quit.type = SDL_QUIT;
      e.quit.timestamp = SDL_GetTicks();
      SDL_PushEvent(&e);
    }
  
  Sounds->GetMusicPlayer()->SetMusicCategory("Death");
  assert(_state == State::RUNNING);
  _teletyper->AppendText("Offline.\n");
  ChangeState(State::DEATH_PAUSE);
}


void ScreenLevel::NextLifeOrQuit()
{
  assert(_state == State::DEATH_PAUSE);
  assert(_pausebutton);

  _gamestats->AdjustLives(-1);
  OnLivesUpdated();
  
  if(_gamestats->GetLives() > 0)
    {
      _teletyper->AppendText("\n\n\n");
      _scene->CreatePlayer();
      _scene->SetupPlayer();
      _scene->GetPlayer()->SetOwnerGameStats(_gamestats);
      _scene->GetPlayer()->SetOnDestroyed([this]([[maybe_unused]] Object * destroyer)
      {
        OnPlayerDies();
      });

      auto splayer = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
      if(splayer)
        splayer->SetOnHumanCountChanged([this]()
        {
          OnHumanCountUpdated();
        });
      OnHumanCountUpdated();

      SetAmmoType(_weapon_ammo_type);

      delete _current_quicktimeevent;
      _current_quicktimeevent = nullptr;
      
      PlayMusic();
      RefreshUI();

      ChangeState(State::RUNNING);
    }
  else
    GameOver(false);
}


void ScreenLevel::RefreshUI()
{ // Todo: Fix the widgets to listen for changes and get rid of this.
  auto player = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
  if(!player)
    return;
  
  for(auto w : _player_status_widgets)
    w->SetSpaceship(player);
  for(auto w : _player_upgrade_status_widgets)
    w->SetUpgrade(player->GetUpgrade(w->GetUpgrade()->GetType()));

  switch(player->GetWeapon(0)->GetAmmo())
    {
    case Weapon::AmmoType::KINETIC:
      _weapon_type_widget->SetImage("Projectile");
      break;
    case Weapon::AmmoType::PLASMA:
      _weapon_type_widget->SetImage("Plasma");
      break;
    }
}


void ScreenLevel::OnLevelChanged()
{
  auto level = _levels[_current_level];
  _teletyper->AppendText("Destination: " + level->GetName() + "\n");
  level->Start();

#if defined(WITH_VALGRIND) && true
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wuseless-cast"
  std::cout << "CALLGRIND_ZERO_STATS()" << std::endl;
  CALLGRIND_ZERO_STATS;
# pragma GCC diagnostic pop
#endif
}


void ScreenLevel::SetupLevels()
{
  assert(_levels.size() > 0);
  OnLevelChanged();
}


void ScreenLevel::ChangeState(State new_state)
{
  assert(_state != new_state);
  switch(new_state)
    {
    case State::RUNNING:
      if(_pausebutton)
        {
          SetModalWidget(nullptr);
          _pausebutton->Destroy();
          _pausebutton = nullptr;
        }
      _teletyper->SetPaused(false);
      break;
      
    case State::DEATH_PAUSE:
      {
        _state_death_pause_key_eaten = false;

        const std::string t("Press the button to continue.");
        const auto width = Settings->GetInt("screen_width");
        const auto height = Settings->GetInt("screen_height");
        auto font = AssetLoader->LoadFont(20);
        double tlen = font->GetWidth(t);

        assert(!_pausebutton);
        _pausebutton = new Widget(GetRootWidget(), glm::ivec2((width - static_cast<int>(tlen)) / 2, height / 2), glm::ivec2(tlen, 27));
        _pausebutton->SetText(t);
        _pausebutton->SetOnClicked([this](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & position)
        {
          if(!pressed)
            NextLifeOrQuit();
        });
        SetModalWidget(_pausebutton);
      }
      break;
      
    case State::FULL_PAUSE:
      break;

    case State::GAMEOVER_PAUSE:
      break;
      
    case State::SCREEN_TRANSITION:
      {
        assert(!_pausebutton);
        Sounds->GetMusicPlayer()->FadeOutCurrentSong(1);
        
        _pausebutton = new Widget(GetRootWidget(), glm::ivec2(0, 0), glm::ivec2(Settings->GetInt("screen_width"), Settings->GetInt("screen_height")));
        _pausebutton->SetImage(AssetLoader->LoadImage("White"));
        _pausebutton->SetImageColor(glm::vec4(0, 0, 0, 0));
        _pausebutton->SetText(_screentransition_text);
        _pausebutton->SetTextPaddingCentered(true, true);
        SetModalWidget(_pausebutton);

        _teletyper->AppendText(_screentransition_text + '\n');

        auto player = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
        if(player)
          {
            player->ClearControlPrograms();
            
            SpaceshipControlProgram * p = new SCP_MoveForward(player, 4, 2.5);
            player->AddControlProgram(p);
            
            p = new SCP_Pitch(player, -90, 1);
            player->AddControlProgram(p);
          }
        
        _screentransition_timer = 0.0;
      }
      break;
    }
  
  _state = new_state;
}


GameStats * ScreenLevel::GetGameStats() const
{
  return _gamestats;
}


Scene * ScreenLevel::GetScene() const
{
  return _scene;
}


Level * ScreenLevel::GetCurrentLevel() const
{
  return _levels[_current_level];
}


void ScreenLevel::TransitionToScreen(Screen * new_screen, const std::string & message)
{
  _screentransition_new_screen = new_screen;
  _screentransition_text = message;
  
  ChangeState(State::SCREEN_TRANSITION);
}


void ScreenLevel::OpenSpaceshipMaintenanceUI()
{
  auto player = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
  if(!player)
    return;
  
  ChangeState(State::FULL_PAUSE);

  const auto width = Settings->GetInt("screen_width");
  const auto height = Settings->GetInt("screen_height");
  const glm::ivec2 panelsize(600, 500);
  
  auto panelbackground = new Widget(GetRootWidget(), glm::ivec2(0, 0), glm::ivec2(width, height));
  panelbackground->SetImage("White");
  panelbackground->SetImageColor(glm::vec4(0, 0, 0, 0.5));
  
  new WidgetSpaceshipMaintenance(panelbackground, glm::ivec2((width - panelsize.x) / 2, (height - panelsize.y) / 2), panelsize, player, _gamestats);
  
  assert(!_pausebutton);
  _pausebutton = panelbackground;
  _teletyper->SetPaused(true);

  SetModalWidget(_pausebutton);
}


void ScreenLevel::OpenPauseUI()
{
  ChangeState(State::FULL_PAUSE);

  const auto width = Settings->GetInt("screen_width");
  const auto height = Settings->GetInt("screen_height");
  
  auto panelbackground = new Widget(GetRootWidget(), glm::ivec2(0, 0), glm::ivec2(width, height));
  panelbackground->SetImage("White");
  panelbackground->SetImageColor(glm::vec4(0, 0, 0, 0.5));
  panelbackground->SetIsFocusable(false);
  
  auto panel = panelbackground;
  
  glm::ivec2 siz(300, 75);
  glm::ivec2 pos((width - siz.x) / 2, (height - 3 * (siz.y + 10)) / 2);
  
  auto bcont = new WidgetButton(panel, pos, siz);
  bcont->SetText("Continue");
  bcont->SetTextPaddingCentered(true, true);
  bcont->SetOnClicked([this](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & position)
  {
    if(!pressed)
      ChangeState(State::RUNNING);
  });
  pos.y += siz.y + 10;

  auto bquit = new WidgetButton(panel, pos, siz);
  bquit->SetText("Quit game");
  bquit->SetTextPaddingCentered(true, true);
  bquit->SetOnClicked([this](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & position)
  {
    if(!pressed)
      GameOver(false);
  });
  pos.y += siz.y + 10;

  auto bexit = new WidgetButton(panel, pos, siz);
  bexit->SetText("Exit to desktop");
  bexit->SetTextPaddingCentered(true, true);
  bexit->SetOnClicked([this](bool pressed, [[maybe_unused]] unsigned int button, [[maybe_unused]] const glm::ivec2 & position)
  {
    if(!pressed)
      {
        SDL_Event e;
        e.quit.type = SDL_QUIT;
        e.quit.timestamp = SDL_GetTicks();
        SDL_PushEvent(&e);
      }
  });
  pos.y += siz.y + 10;
  
  assert(!_pausebutton);
  _pausebutton = panelbackground;
  SetModalWidget(_pausebutton);
}


void ScreenLevel::GameOver(bool game_was_completed)
{
  Sounds->GetMusicPlayer()->SetMusicCategory(game_was_completed ? "GameOverVictory" : "GameOverLoss");

  auto hs = new HighscoreEntry(_scene, _current_level, game_was_completed);
  Highscores->Add(hs);
  Highscores->SetLast(hs);

  auto player = _scene->GetPlayer();
  if(player && player->IsAlive())
    player->SetUseHealth(false);
  ChangeState(State::GAMEOVER_PAUSE);
  
  auto w = Settings->GetInt("screen_width");
  auto h = Settings->GetInt("screen_height");

  Widget * result;
  {
    std::string t;
    if(game_was_completed)
      t = "Congratulations! You have successfully saved everyone!";
    else
      t = "Unfortunately you lost, and the evil alien invaders won.";

    auto font = AssetLoader->LoadFont(20);
    auto tw = static_cast<int>(font->GetWidth(t));
    result = new Widget(GetRootWidget(), glm::ivec2(0, 0), glm::ivec2(w, h));
    result->SetText(t);
    result->SetTextFont(font);
    result->SetTextFontWeight(0.5);
    result->SetTextColor(glm::vec3(0.5, 1, 1));
    result->SetTextPadding(glm::vec2((w - tw) / 2, h / 5));
  }
  {
    auto wht = new WidgetHighscores(result, glm::ivec2(0, 0), glm::ivec2(w, h), true);
    wht->SetOnDestroy([this, result]([[maybe_unused]] Widget * destroyed_widget)
    {
      Quit();
      auto parent = _parent;
      while(parent)
        {
          parent->Quit();
          parent = parent->_parent;
        }
    });
  }


  SetModalWidget(result);

  if(_pausebutton)
    _pausebutton->Destroy();
  
  _pausebutton = result;
}


void ScreenLevel::OnQuit()
{
  if(_parent)
    _parent->CopyPlayerData(this);
}


void ScreenLevel::CopyPlayerData(ScreenLevel * src)
{
  auto myplr = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
  auto srcplr = dynamic_cast<ObjectSpaceship *>(src->GetScene()->GetPlayer());
  if(myplr && srcplr)
    {
      assert(myplr != srcplr);

      for(unsigned int i = 0; i < srcplr->GetEngineCount(); i++)
        srcplr->SetEngineThrottle(i, 0.0);
      for(unsigned int i = 0; i < srcplr->GetWeaponCount(); i++)
        srcplr->SetWeaponAutofire(i, false);

      myplr->CopyUpgrades(*srcplr);
      myplr->SetHealth(srcplr->GetHealth());
    }
  _scene->SetupPlayer();
}


Camera * ScreenLevel::GetCamera() const
{
  return _camera;
}


void ScreenLevel::SetAmmoType(Weapon::AmmoType type)
{
  _weapon_ammo_type = type;

  auto player = dynamic_cast<ObjectSpaceship *>(_scene->GetPlayer());
  if(player)
    {
      auto count = player->GetWeaponCount();
      for(unsigned int i = 0; i < count; i++)
        player->GetWeapon(i)->SetAmmo(type);
    }
  
  if(_weapon_type_widget)
    switch(_weapon_ammo_type)
      {
      case Weapon::AmmoType::KINETIC:
        _weapon_type_widget->SetImage("Projectile");
        break;
      case Weapon::AmmoType::PLASMA:
        _weapon_type_widget->SetImage("Plasma");
        break;
      }
}
