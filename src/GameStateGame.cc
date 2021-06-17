#include "GameStateGame.hh"
#include "AdditiveBlending.hh"
#include "Camera.hh"
#include "Font.hh"
#include "GaussianBlur.hh"
#include "Image.hh"
#include "MainLevel.hh"
#include "MeshOverlay.hh"
#include "ObjectCollectible.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "ScoreReel.hh"
#include "SolarSystemObject.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "TextureRenderer.hh"
#include "UpgradeMaterial.hh"
#include "WidgetButton.hh"
#include "WidgetPlayerShip.hh"
#include "WidgetSpaceshipMaintenance.hh"
#include "WidgetSpaceshipStatus.hh"
#include "WidgetSpaceshipUpgradeStatus.hh"
#include "WidgetUpgradeMaterial.hh"
#include "WidgetWeaponStatus.hh"
#include <iostream>


GameStateGame::GameStateGame(GameStats * gamestats)
  : GameState(),
    _state(State::RUNNING),
    _random(0),
    _rdist(0, 1),
    _current_level(0),
    _gamestats(gamestats),
    _pausebutton(nullptr)
{
  _camera = new Camera();
  _scene = new Scene();
  if(!_gamestats)
    _gamestats = new GameStats();
  _texture_renderer = new TextureRenderer(static_cast<unsigned int>(Settings->GetInt("screen_width")),
                                          static_cast<unsigned int>(Settings->GetInt("screen_height")),
                                          2);
  _overlay_mesh = new MeshOverlay();
  _overlay_mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));

  _blur = new GaussianBlur();
  _blender = new AdditiveBlending();
    
  _fov = 60.0;
#define CAMERA_SPEED 0.5

  _camera->SetFOV(_fov);
  _camera->SetClippingPlanes(0.01, 10000.0);
  _camera->UpdateProjection();
  
  _scene->GetPlayer()->SetOwnerGameStats(_gamestats);
  _scene->GetPlayer()->SetOnDestroyed([this](Object * destroyer)
  {
    assert(destroyer == destroyer);
    OnPlayerDies();
  });

  _score_reel = new ScoreReel(10);

  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(width, height));
  assert(root);
  root->SetIsFocusable(false);
  SetRootWidget(root);

  for(int i = 0; i < 5; i++)
    {
      auto w = new WidgetPlayerShip(root, glm::ivec2(width - 64 - i * 50, 10), glm::ivec2(50, 50));
      _lives_widgets.push_back(w);
    }
  OnLivesUpdated();

  {
    int y = 70;
    for(auto um : _gamestats->GetUpgradeMaterials())
      {
        auto w = new WidgetUpgradeMaterial(root, glm::ivec2(width - 50, y), glm::ivec2(50, 25), *um);
        _upgradematerial_widgets.push_back(w);
        y += 25;
      }
  }
  {
    auto w = new WidgetSpaceshipStatus(root, glm::ivec2(width - 32, 160), glm::ivec2(20, 100), _scene->GetPlayer());
    _player_status_widgets.push_back(w);
    auto l = new Widget(root, w->GetPosition() + glm::ivec2(0, w->GetSize().y), glm::ivec2(20, 20));
    l->SetText("H");
    l->SetTextFont(AssetLoader->LoadFont(12));
    l->SetTextColor(glm::vec3(0.0, 0.5, 0.0));
    l->SetTextPaddingCentered(true, true);
  }
  {
    auto w = new WidgetWeaponStatus(root, glm::ivec2(width - 32 - 24, 160), glm::ivec2(20, 100), _scene->GetPlayer());
    _player_status_widgets.push_back(w);
    auto l = new Widget(root, w->GetPosition() + glm::ivec2(0, w->GetSize().y), glm::ivec2(20, 20));
    l->SetText("W");
    l->SetTextFont(AssetLoader->LoadFont(12));
    l->SetTextColor(glm::vec3(0.0, 0.5, 0.0));
    l->SetTextPaddingCentered(true, true);
  }
  {
    auto w = new WidgetSpaceshipUpgradeStatus(root, glm::ivec2(width - 32 - 24 - 24, 160), glm::ivec2(20, 100), _scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER));
    _player_upgrade_status_widgets.push_back(w);
    auto l = new Widget(root, w->GetPosition() + glm::ivec2(0, w->GetSize().y), glm::ivec2(20, 20));
    l->SetText("E");
    l->SetTextFont(AssetLoader->LoadFont(12));
    l->SetTextColor(glm::vec3(0.0, 0.5, 0.0));
    l->SetTextPaddingCentered(true, true);
  }
  {
    std::vector<std::string> imagenames
      {
        "",
        "BonusIcon-2xDamage",
        "BonusIcon-2xScore",
        "BonusIcon-Shield",
      };
    unsigned int x = 10;
    for(auto imagename : imagenames)
      {
        Widget * w = nullptr;
        if(imagename.size() > 0)
          {
            w = new Widget(root, glm::ivec2(x, 70), glm::ivec2(48, 48));
            w->SetImage(imagename);
            w->SetIsVisible(false);
            x += 50;
          }
        _active_bonus_widgets.push_back(w);
      }
  }

  _levelinfo_widget = new Widget(root, glm::ivec2(10, 10), glm::ivec2(300, 25));
  _levelinfo_widget->SetTextColor(glm::vec3(1.00, 0.88, 0.00));
  _levelinfo_widget->SetTextFont(AssetLoader->LoadFont(12));
}


GameStateGame::~GameStateGame()
{
  delete _scene;
  delete _camera;
}


void GameStateGame::Tick(double deltatime)
{
  auto level = _levels[_current_level];
  
  _score_reel->SetScore(_gamestats->GetScore());
  _score_reel->Tick(deltatime);

  if(_state == State::RUNNING)
    _gamestats->Tick(deltatime);

  if(_state != State::FULL_PAUSE)
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
            Quit();
        }
    }
  
  _active_bonus_widgets[ObjectCollectible::Type::TYPE_DAMAGE_MULTIPLIER]->SetIsVisible(_scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::BONUS_DAMAGE)->IsActive());
  _active_bonus_widgets[ObjectCollectible::Type::TYPE_SCORE_MULTIPLIER]->SetIsVisible(_gamestats->GetScoreMultiplier() > 1);
  _active_bonus_widgets[ObjectCollectible::Type::TYPE_SHIELD]->SetIsVisible(_scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::SHIELD)->IsActive());

  auto fmt = [](double v)
  {
    auto rv = std::to_string(v);
    return rv.substr(0, rv.find('.') + 2);
  };
  
  _levelinfo_widget->SetText(level->GetName() + "   " + fmt(level->GetTime()) + " (" + fmt(_gamestats->GetTime()) + ")");

  _texture_renderer->BeginRender();
  _scene->Draw(*_camera);
  _texture_renderer->EndRender();

  glDisable(GL_DEPTH_TEST);
  auto tid_glow = _blur->Blur(3, _texture_renderer->GetTextureId(1));
  _blender->Blend(_texture_renderer->GetTextureId(0), tid_glow);
  glEnable(GL_DEPTH_TEST);
  
  _score_reel->Draw();
  
  GameState::Tick(deltatime);
}


void GameStateGame::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
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

  assert(_state == State::RUNNING);
  switch(key)
    {
    case SDLK_ESCAPE:
      if(!pressed)
        Quit();
      break;
    case SDLK_TAB:
      if(!pressed)
        ChangeState(State::FULL_PAUSE);
      break;
#if 0
    case SDLK_LEFT:
      if(pressed)
        _camera->MoveRight(-CAMERA_SPEED * 2.0);
      break;
      
    case SDLK_RIGHT:
      if(pressed)
        _camera->MoveRight(CAMERA_SPEED * 2.0);
      break;
      
    case SDLK_UP:
      if(pressed)
        _camera->MoveForward(CAMERA_SPEED);
      break;
      
    case SDLK_DOWN:
      if(pressed)
        _camera->MoveForward(-CAMERA_SPEED);
      break;
      
    case SDLK_PAGEUP:
      if(pressed)
        _camera->MoveUp(CAMERA_SPEED);
      break;
      
    case SDLK_PAGEDOWN:
      if(pressed)
        _camera->MoveUp(-CAMERA_SPEED);
      break;
      
    case SDLK_PLUS:
      if(pressed)
        {
          _fov++;
          std::cout << "fov:" << _fov << std::endl;
          _camera->SetFOV(_fov);
          _camera->UpdateProjection();
        }
      break;
      
    case SDLK_MINUS:
      if(pressed)
        {
          _fov--;
          std::cout << "fov:" << _fov << std::endl;
          _camera->SetFOV(_fov);
          _camera->UpdateProjection();
        }
      break;

#else
    case SDLK_LEFT:
      _scene->GetPlayer()->SetEngineThrottle(0, pressed ? 1.0 : 0.0);
      break;
      
    case SDLK_RIGHT:
      _scene->GetPlayer()->SetEngineThrottle(1, pressed ? 1.0 : 0.0);
      break;

    case SDLK_SPACE:
      for(unsigned int i = 0; i < _scene->GetPlayer()->GetWeaponCount(); i++)
        _scene->GetPlayer()->SetWeaponAutofire(i, pressed);
      break;

    case SDLK_c:
      if(pressed)
        {
          auto em = _scene->GetPlayer()->GetUpgrade(SpaceshipUpgrade::Type::EVASION_MANEUVER);
          if(em->CanActivate())
            em->Activate(4.0);
        }
      break;
#endif


    case SDLK_q:
      if(pressed)
        std::cout << _scene->GetPlayer()->GetPosition() << std::endl;
      break;
    }

}


void GameStateGame::OnLivesUpdated()
{
  int used = 0;
  for(auto w : _lives_widgets)
    {
      w->SetIsVisible(used < _gamestats->GetLives());
      used++;
    }
}


void GameStateGame::OnPlayerDies()
{
  assert(_state == State::RUNNING);
  ChangeState(State::DEATH_PAUSE);
}


void GameStateGame::NextLifeOrQuit()
{
  assert(_state == State::DEATH_PAUSE);
  assert(_pausebutton);

  _gamestats->AdjustLives(-1);
  OnLivesUpdated();
  
  if(_gamestats->GetLives() > 0)
    {
      _scene->CreatePlayer();
      _scene->GetPlayer()->SetOwnerGameStats(_gamestats);
      _scene->GetPlayer()->SetOnDestroyed([this](Object * destroyer)
      {
        assert(destroyer == destroyer);
        OnPlayerDies();
      });
      for(auto w : _player_status_widgets)
        w->SetSpaceship(_scene->GetPlayer());
      for(auto w : _player_upgrade_status_widgets)
        w->SetUpgrade(_scene->GetPlayer()->GetUpgrade(w->GetUpgrade()->GetType()));
    }
  else
    Quit();

  ChangeState(State::RUNNING);
}


void GameStateGame::OnLevelChanged()
{
  auto level = _levels[_current_level];
  level->Start();
}


void GameStateGame::SetupLevels()
{
  bool done = false;
  for(unsigned int i = 0; !done; i++)
    {
      auto sobj = AssetLoader->LoadSolarSystemObject(SolarSystemObject::Type::PLANET, i);
      if(sobj)
        _levels.push_back(new MainLevel(_scene, sobj));
      else
        done = true;
    }

  OnLevelChanged();
}


void GameStateGame::ChangeState(State new_state)
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

          _upgradematerial_widgets[0]->SetText(std::to_string(_gamestats->GetUpgradeMaterial(UpgradeMaterial::Type::ATTACK)->GetAmount()));
          _upgradematerial_widgets[1]->SetText(std::to_string(_gamestats->GetUpgradeMaterial(UpgradeMaterial::Type::DEFENSE)->GetAmount()));
          _upgradematerial_widgets[2]->SetText(std::to_string(_gamestats->GetUpgradeMaterial(UpgradeMaterial::Type::PHYSICAL)->GetAmount()));
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
      {
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
      break;
    }
  
  _state = new_state;
}


GameStats * GameStateGame::GetGameStats() const
{
  return _gamestats;
}


Scene * GameStateGame::GetScene() const
{
  return _scene;
}

