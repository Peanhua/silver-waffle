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
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "ScoreReel.hh"
#include "SolarSystemObject.hh"
#include "SpaceshipControlProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemHighscores.hh"
#include "SubsystemSettings.hh"
#include "TextureRenderer.hh"
#include "UpgradeMaterial.hh"
#include "WidgetButton.hh"
#include "WidgetPlayerShip.hh"
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


ScreenLevel::ScreenLevel(ScreenLevel * parent)
  : Screen(),
    _parent(parent),
    _state(State::RUNNING),
    _random(0),
    _rdist(0, 1),
    _current_level(0),
    _gamestats(nullptr),
    _pausebutton(nullptr),
    _demo_lander_activated(false),
    _demo_ending_activated(false)
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
      _gamestats = _parent->GetGameStats();
      _scene->EnableTutorialMessages(false);
    }

  if(!_gamestats)
    _gamestats = new GameStats();
  
  _scene->GetPlayer()->SetOwnerGameStats(_gamestats);
  _scene->GetPlayer()->SetOnDestroyed([this](Object * destroyer)
  {
    assert(destroyer == destroyer);
    OnPlayerDies();
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

  for(int i = 0; i < 5; i++)
    {
      auto w = new WidgetPlayerShip(root, glm::ivec2(width - 64 - i * 50, 10), glm::ivec2(50, 50));
      _lives_widgets.push_back(w);
    }
  OnLivesUpdated();

  {
    int x = static_cast<int>(width) - 8;
    int y = 70;
    const glm::ivec2 size(20, 100);
    std::vector<Widget *>    widgets;
    std::vector<std::string> labels;
    {
      x -= size.x + 4;
      auto w = new WidgetSpaceshipStatus(root, {x, y}, size, _scene->GetPlayer());
      _player_status_widgets.push_back(w);
      widgets.push_back(w);
      labels.push_back("H");
    }
    {
      x -= size.x + 4;
      auto w = new WidgetWeaponStatus(root, {x, y}, size, _scene->GetPlayer());
      _player_status_widgets.push_back(w);
      widgets.push_back(w);
      labels.push_back("W");
    }
    {
      x -= size.x + 4;
      auto w = new WidgetSpaceshipUpgradeStatus(root, {x, y}, size, _scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER));
      _player_upgrade_status_widgets.push_back(w);
      widgets.push_back(w);
      labels.push_back("E");
    }
    {
      x -= size.x + 4;
      auto w = new WidgetSpaceshipUpgradeStatus(root, {x, y}, size, _scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE));
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
  }
  
  {
    std::vector<std::string> imagenames
      {
        "BonusIcon-2xDamage",
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

#ifdef WITH_VALGRIND && true
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wold-style-cast"
# pragma GCC diagnostic ignored "-Wuseless-cast"
  CALLGRIND_ZERO_STATS;
# pragma GCC diagnostic pop
#endif
  if(Settings->GetBool("demo"))
    _scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::PLANET_LANDER)->Install();
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
      if(!_demo_lander_activated)
        if(!_parent && GetGameStats()->GetTime() > 9.75)
          { // Activates only if "cheat_planet_lander_disable_distance_check" settings is true.
            _scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::PLANET_LANDER)->Activate();
            _demo_lander_activated = true;
          }
      if(!_demo_ending_activated && _parent && GetGameStats()->GetTime() > 9.0)
        {
          _demo_ending_activated = true;
          _scene->GetPlayer()->GetController()->SteerBackward(true);
        }
      if(GetGameStats()->GetTime() > 20.0)
        {
          SDL_Event e;
          e.quit.type = SDL_QUIT;
          e.quit.timestamp = SDL_GetTicks();
          SDL_PushEvent(&e);
        }
    }
      
  
  auto level = _levels[_current_level];
  
  _score_reel->SetScore(_gamestats->GetScore());

  if(_state == State::RUNNING)
    {
      _gamestats->Tick(deltatime);
      if(GetGameStats()->GetTime() > 6.0)
        _scene->TutorialMessage(0, "Fire at will.\n");
    }

  if(_state == State::RUNNING || _state == State::DEATH_PAUSE)
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
      _scene->GetPlayer()->Tick(deltatime);
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
          
          if(_scene->GetPlayer()->GetActiveControlProgramCount() == 0)
            { // Enter the new screen.
              SetChild(_screentransition_new_screen);
              _screentransition_new_screen = nullptr;
              _pausebutton->SetText("");
              _screentransition_timer = 0;
              { // Set resume animation here.
                auto player = _scene->GetPlayer();
                
                auto p = new SCP_Pitch(player, 180, 0.5);
                player->AddControlProgram(p);
              }
            }
        }
      else
        { // Resumed from the new screen.
          if(GetGameStats()->GetLives() == 0)
            GameOver(false);
          else
            {
              if(_screentransition_timer < 0.5)
                {
                  auto amount = std::clamp(1.0 - _screentransition_timer / 0.5, 0.0, 1.0);
                  _pausebutton->SetImageColor(glm::vec4(0, 0, 0, amount));
                }
              if(_scene->GetPlayer()->GetActiveControlProgramCount() == 0)
                {
                  _scene->GetPlayer()->SetPosition(glm::vec3(_scene->GetPlayer()->GetPosition().x, 40 - 53, 0));
                  _scene->GetPlayer()->SetOrientation(glm::quat(1, 0, 0, 0));
                  RefreshUI();
                  OnLivesUpdated();
                  ChangeState(State::RUNNING);
                }
            }
        }
    }
  
  _active_bonus_widgets[0]->SetIsVisible(_scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE)->IsActive());
  _active_bonus_widgets[1]->SetIsVisible(_gamestats->GetScoreMultiplier() > 1);
  _active_bonus_widgets[2]->SetIsVisible(_scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::SHIELD)->IsActive());

  auto fmt = [](double v)
  {
    auto rv = std::to_string(v);
    return rv.substr(0, rv.find('.') + 2);
  };
  
  _levelinfo_widget->SetText(level->GetName() + "   " + fmt(level->GetTime()) + " (" + fmt(_gamestats->GetTime()) + ")");

  {
    auto player = _scene->GetPlayer();
    if(player && player->IsAlive())
      if(player->SystemlogGet().length() > 0)
        {
          _teletyper->AppendText(player->SystemlogGet());
          player->SystemlogClear();
        }
  }
  _texture_renderer->BeginRender();
  _camera->Update();
  _camera->stats.elapsed_frames++;
  _scene->Draw(*_camera);
  _texture_renderer->EndRender();

  glDisable(GL_DEPTH_TEST);
  auto tid_glow = _blur->Blur(3, _texture_renderer->GetTextureId(1));
  _blender->Blend(_texture_renderer->GetTextureId(0), tid_glow);
  glEnable(GL_DEPTH_TEST);
  
  Screen::Tick(deltatime);
}


void ScreenLevel::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

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
  auto controller = player->GetController();
  bool disablecontrols = _scene->IsWarpEngineStarting();

  if(player && player->IsAlive())
    {
      auto u = player->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE);
      if(u->IsActive())
        disablecontrols = true;
    }
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

    case SDLK_c:
      if(!disablecontrols && pressed)
        {
          auto em = player->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER);
          if(em->CanActivate())
            em->Activate(4.0);
        }
      break;

    case SDLK_w:
      if(pressed)
        _scene->StartWarpEngine();
      else
        _scene->StopWarpEngine();
      break;

    case SDLK_s:
      if(pressed)
        {
          auto lander = player->GetUpgrade(SpaceshipUpgrade::Type::PLANET_LANDER);
          if(lander->CanActivate())
            lander->Activate();
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


void ScreenLevel::OnPlayerDies()
{
  if(Settings->GetBool("demo"))
    {
      SDL_Event e;
      e.quit.type = SDL_QUIT;
      e.quit.timestamp = SDL_GetTicks();
      SDL_PushEvent(&e);
    }
  
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
      _scene->GetPlayer()->SetOnDestroyed([this](Object * destroyer)
      {
        assert(destroyer == destroyer);
        OnPlayerDies();
      });
      RefreshUI();
    }
  else
    GameOver(false);

  ChangeState(State::RUNNING);
}


void ScreenLevel::RefreshUI()
{ // Todo: Fix the widgets to listen for changes and get rid of this.
  for(auto w : _player_status_widgets)
    w->SetSpaceship(_scene->GetPlayer());
  for(auto w : _player_upgrade_status_widgets)
    w->SetUpgrade(_scene->GetPlayer()->GetUpgrade(w->GetUpgrade()->GetType()));
}


void ScreenLevel::OnLevelChanged()
{
  auto level = _levels[_current_level];
  _teletyper->AppendText("Destination: " + level->GetName() + "\n");
  level->Start();
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
        _pausebutton->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
        {
          assert(button == button);
          assert(position == position);
          
          if(!pressed)
            NextLifeOrQuit();
        });
        SetModalWidget(_pausebutton);
      }
      break;
      
    case State::FULL_PAUSE:
      break;

    case State::SCREEN_TRANSITION:
      {
        assert(!_pausebutton);
        _pausebutton = new Widget(GetRootWidget(), glm::ivec2(0, 0), glm::ivec2(Settings->GetInt("screen_width"), Settings->GetInt("screen_height")));
        _pausebutton->SetImage(AssetLoader->LoadImage("White"));
        _pausebutton->SetImageColor(glm::vec4(0, 0, 0, 0));
        _pausebutton->SetText(_screentransition_text);
        _pausebutton->SetTextPaddingCentered(true, true);
        SetModalWidget(_pausebutton);

        _teletyper->AppendText(_screentransition_text + '\n');

        {
          auto player = _scene->GetPlayer();

          player->ClearControlPrograms();

          SpaceshipControlProgram * p = new SCP_MoveForward(player, 4, 2.5);
          player->AddControlProgram(p);
          
          p = new SCP_Pitch(_scene->GetPlayer(), -90, 1);
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


void ScreenLevel::TransitionToScreen(Screen * new_screen, const std::string & message)
{
  _screentransition_new_screen = new_screen;
  _screentransition_text = message;
  
  ChangeState(State::SCREEN_TRANSITION);
}


void ScreenLevel::OpenSpaceshipMaintenanceUI()
{
  ChangeState(State::FULL_PAUSE);

  const auto width = Settings->GetInt("screen_width");
  const auto height = Settings->GetInt("screen_height");
  const glm::ivec2 panelsize(600, 500);
  
  auto panelbackground = new Widget(GetRootWidget(), glm::ivec2(0, 0), glm::ivec2(width, height));
  panelbackground->SetImage("White");
  panelbackground->SetImageColor(glm::vec4(0, 0, 0, 0.5));
  
  new WidgetSpaceshipMaintenance(panelbackground, glm::ivec2((width - panelsize.x) / 2, (height - panelsize.y) / 2), panelsize, _scene->GetPlayer(), _gamestats);
  
  assert(!_pausebutton);
  _pausebutton = panelbackground;
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
  bcont->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      ChangeState(State::RUNNING);
  });
  pos.y += siz.y + 10;

  auto bquit = new WidgetButton(panel, pos, siz);
  bquit->SetText("Quit game");
  bquit->SetTextPaddingCentered(true, true);
  bquit->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      GameOver(false);
  });
  pos.y += siz.y + 10;

  auto bexit = new WidgetButton(panel, pos, siz);
  bexit->SetText("Exit to desktop");
  bexit->SetTextPaddingCentered(true, true);
  bexit->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
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
  auto hs = new HighscoreEntry(_score_reel->GetScore(), _current_level, _scene, game_was_completed);
  Highscores->Add(hs);

  Quit();
  auto parent = _parent;
  while(parent)
    {
      parent->Quit();
      parent = parent->_parent;
    }
}


void ScreenLevel::OnQuit()
{
  if(_parent)
    _parent->CopyPlayerData(this);
}


void ScreenLevel::CopyPlayerData(ScreenLevel * src)
{
  auto myplr = _scene->GetPlayer();
  auto srcplr = src->GetScene()->GetPlayer();

  for(unsigned int i = 0; i < srcplr->GetEngineCount(); i++)
    srcplr->SetEngineThrottle(i, 0.0);
  srcplr->GetUpgrade(SpaceshipUpgrade::Type::WARP_ENGINE)->SetEnabled(false);
  for(unsigned int i = 0; i < srcplr->GetWeaponCount(); i++)
    srcplr->SetWeaponAutofire(i, false);

  myplr->CopyUpgrades(*srcplr);
  myplr->SetHealth(srcplr->GetHealth());
  _scene->SetupPlayer();
}


Camera * ScreenLevel::GetCamera() const
{
  return _camera;
}
