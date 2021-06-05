#include "GameStateGame.hh"
#include "Camera.hh"
#include "Font.hh"
#include "Level.hh"
#include "Mesh.hh"
#include "Milkyway.hh"
#include "ObjectCollectible.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "ScoreReel.hh"
#include "SolarSystemObject.hh"
#include "SpaceParticles.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "WidgetPlayerShip.hh"
#include "WidgetSpaceshipStatus.hh"
#include "WidgetWeaponStatus.hh"
#include <iostream>


GameStateGame::GameStateGame()
  : GameState(),
    _random(0),
    _rdist(0, 1),
    _paused(false),
    _current_level(0),
    _score_multiplier_timer(0),
    _upgradematerial_a(0),
    _upgradematerial_d(0),
    _upgradematerial_p(0),
    _lives(3),
    _pausebutton(nullptr)
{
#if 1
  _camera = new Camera();
  _scene = new Scene();
  _fov = 60.0;
# define CAMERA_SPEED 0.5
#else
  _camera = new Camera2();
  _scene = new Scene2();
  _fov = 120.0;
# define CAMERA_SPEED 1
#endif
  
  _camera->SetFOV(_fov);
  _camera->SetClippingPlanes(0.01, 10000.0);
  _camera->UpdateProjection();
  
  _scene->Initialize(1.0);
  
  _scene->SetOnDestroyed([this](Object * destroyer, Object * target)
  {
    if(destroyer == _scene->GetPlayer() && target != _scene->GetPlayer())
      {
        {
          unsigned int score = 1;
          if(_score_multiplier_timer > 0.0)
            score *= _score_multiplier;
          _score_reel->SetScore(_score_reel->GetScore() + score);
        }

        ObjectCollectible::Type bonustype = ObjectCollectible::TYPE_NONE;
        double bonus;
        bool dorandomrotation = false;

        auto r = _rdist(_random);
        if(r < 0.10f)
          {
            bonustype = ObjectCollectible::TYPE_SCORE_BONUS;
            bonus = 5.0f + _rdist(_random) * 5.0f;
            dorandomrotation = true;
          }
        else if(r < 0.15f)
          {
            bonustype = ObjectCollectible::TYPE_DAMAGE_MULTIPLIER;
            bonus = 2.0;
          }
        else if(r < 0.20f)
          {
            bonustype = ObjectCollectible::TYPE_SCORE_MULTIPLIER;
            bonus = 2.0;
          }
        else if(r < 0.30f)
          {
            bonustype = ObjectCollectible::TYPE_SHIELD;
            bonus = 100.0;
          }
        else if(r < 0.40f)
          {
            bonustype = ObjectCollectible::TYPE_UPGRADEMATERIAL_ATTACK;
            bonus = 1.0;
            dorandomrotation = true;
          }
        else if(r < 0.50f)
          {
            bonustype = ObjectCollectible::TYPE_UPGRADEMATERIAL_DEFENSE;
            bonus = 1.0;
            dorandomrotation = true;
          }
        else if(r < 0.60f)
          {
            bonustype = ObjectCollectible::TYPE_UPGRADEMATERIAL_PHYSICAL;
            bonus = 1.0;
            dorandomrotation = true;
          }

        if(bonustype != ObjectCollectible::TYPE_NONE)
          {
            auto c = AssetLoader->LoadObjectCollectible(bonustype);
            assert(c);
            auto coll = new ObjectCollectible(*c);
            coll->SetBonus(bonustype, bonus);
            _scene->AddCollectible(coll, target->GetPosition(), glm::vec3(0, -5, 0));
            if(dorandomrotation)
              {
                auto rotangle = glm::normalize(glm::vec3(_rdist(_random) * 2.0f - 1.0f,
                                                         _rdist(_random) * 2.0f - 1.0f,
                                                         _rdist(_random) * 2.0f - 1.0f));
                coll->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + static_cast<double>(_rdist(_random)) * 10.0);
              }
          }
      }
    else if(target == _scene->GetPlayer())
      OnPlayerDies();
  });

  _scene->SetOnCollectibleCollected([this](ObjectCollectible * collectible)
  {
    _scene->GetPlayer()->UpgradeFromCollectible(collectible);

    if(collectible->HasBonus(ObjectCollectible::TYPE_SCORE_BONUS))
      {
        auto score = static_cast<unsigned int>(collectible->GetBonus(ObjectCollectible::TYPE_SCORE_BONUS));
        if(_score_multiplier_timer > 0.0)
          score *= _score_multiplier;
        _score_reel->SetScore(_score_reel->GetScore() + score);
      }
    if(collectible->HasBonus(ObjectCollectible::TYPE_SCORE_MULTIPLIER))
      {
        _score_multiplier = static_cast<unsigned int>(collectible->GetBonus(ObjectCollectible::TYPE_SCORE_MULTIPLIER));
        _score_multiplier_timer = 30.0;
      }
    if(collectible->HasBonus(ObjectCollectible::TYPE_UPGRADEMATERIAL_ATTACK))
      {
        _upgradematerial_a += static_cast<unsigned int>(collectible->GetBonus(ObjectCollectible::TYPE_UPGRADEMATERIAL_ATTACK));
        _upgradematerial_a_widget->SetText(std::to_string(_upgradematerial_a));
      }
    if(collectible->HasBonus(ObjectCollectible::TYPE_UPGRADEMATERIAL_DEFENSE))
      {
        _upgradematerial_d += static_cast<unsigned int>(collectible->GetBonus(ObjectCollectible::TYPE_UPGRADEMATERIAL_DEFENSE));
        _upgradematerial_d_widget->SetText(std::to_string(_upgradematerial_d));
      }
    if(collectible->HasBonus(ObjectCollectible::TYPE_UPGRADEMATERIAL_PHYSICAL))
      {
        _upgradematerial_p += static_cast<unsigned int>(collectible->GetBonus(ObjectCollectible::TYPE_UPGRADEMATERIAL_PHYSICAL));
        _upgradematerial_p_widget->SetText(std::to_string(_upgradematerial_p));
      }
  });
  
  {
    bool done = false;
    for(unsigned int i = 0; !done; i++)
      {
        auto sobj = AssetLoader->LoadSolarSystemObject(SolarSystemObject::TYPE_PLANET, i);
        if(sobj)
          _levels.push_back(new Level(_scene, sobj));
        else
          done = true;
      }
  }

  _score_reel = new ScoreReel(10);

  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(width, height));
  assert(root);
  SetRootWidget(root);

  for(int i = 0; i < 5; i++)
    {
      auto w = new WidgetPlayerShip(root, glm::ivec2(width - 64 - i * 50, 10), glm::ivec2(50, 50));
      _lives_widgets.push_back(w);
    }
  OnLivesUpdated();

  {
    auto w = new Widget(root, glm::ivec2(width - 100, 70), glm::ivec2(100, 30));
    w->SetTextColor(glm::vec3(1, 0, 0));
    _upgradematerial_a_widget = w;
  }
  {
    auto w = new Widget(root, glm::ivec2(width - 100, 100), glm::ivec2(100, 30));
    w->SetTextColor(glm::vec3(0, 0, 1));
    _upgradematerial_d_widget = w;
  }
  {
    auto w = new Widget(root, glm::ivec2(width - 100, 130), glm::ivec2(100, 30));
    w->SetTextColor(glm::vec3(1, 0.612, 0));
    _upgradematerial_p_widget = w;
  }
  
  {
    auto w = new WidgetSpaceshipStatus(root, glm::ivec2(width - 32, 160), glm::ivec2(20, 100));
    w->SetSpaceship(_scene->GetPlayer());
    _player_status_widgets.push_back(w);
  }
  {
    auto w = new WidgetWeaponStatus(root, glm::ivec2(width - 32 - 24, 160), glm::ivec2(20, 100));
    w->SetSpaceship(_scene->GetPlayer());
    _player_status_widgets.push_back(w);
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
    
  _particles = new SpaceParticles(5.0, 50.0, 0);

  _milkyway = new Milkyway();

  OnLevelChanged();
}


GameStateGame::~GameStateGame()
{
  delete _scene;
  delete _camera;
}


void GameStateGame::Tick(double deltatime)
{
  auto level = _levels[_current_level];
  
  _particles->Tick(deltatime);
  _scene->Tick(deltatime);
  _score_reel->Tick(deltatime);
  if(_score_multiplier_timer > 0.0)
    _score_multiplier_timer -= deltatime;

  if(!_paused)
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
  
  _active_bonus_widgets[ObjectCollectible::TYPE_DAMAGE_MULTIPLIER]->SetIsVisible(_scene->GetPlayer()->GetUpgrade(ObjectSpaceship::Upgrade::Type::BONUS_DAMAGE)->IsActive());
  _active_bonus_widgets[ObjectCollectible::TYPE_SCORE_MULTIPLIER]->SetIsVisible(_score_multiplier_timer > 0.0);
  _active_bonus_widgets[ObjectCollectible::TYPE_SHIELD]->SetIsVisible(_scene->GetPlayer()->GetUpgrade(ObjectSpaceship::Upgrade::Type::SHIELD)->IsActive());

  glDisable(GL_DEPTH_TEST);
  _milkyway->Draw(*_camera);
  
  glEnable(GL_DEPTH_TEST);
  _particles->Draw(*_camera);
  _scene->Draw(*_camera);
  _score_reel->Draw();
  
  GameState::Tick(deltatime);
}


void GameStateGame::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

  if(_paused)
    {
      if(key == SDLK_ESCAPE || key == SDLK_SPACE)
        if(!pressed)
          NextLifeOrQuit();
      return;
    }
  
  
  switch(key)
    {
    case SDLK_ESCAPE:
      if(pressed)
        Quit();
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
      
#else
    case SDLK_LEFT:
      _scene->GetPlayer()->SetEnginePower(0, pressed ? 1.0 : 0.0);
      break;
      
    case SDLK_RIGHT:
      _scene->GetPlayer()->SetEnginePower(1, pressed ? 1.0 : 0.0);
      break;

    case SDLK_SPACE:
      _scene->GetPlayer()->SetWeaponAutofire(0, pressed);
      break;
#endif
      
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

    case SDLK_x:
      if(pressed)
        {
          /*
          auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(10.0)), glm::vec3(1, 0, 0));
          _scene->GetPlayer()->GetMesh()->ApplyTransform(transform);
          */
          _scene->GetPlayer()->RotateRoll(10.0);
        }
      break;

    case SDLK_y:
      if(pressed)
        {
          /*
          auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(10.0)), glm::vec3(0, 1, 0));
          _scene->GetPlayer()->GetMesh()->ApplyTransform(transform);
          */
          _scene->GetPlayer()->RotatePitch(10.0);
        }
      break;
      
    case SDLK_z:
      if(pressed)
        {
          /*
          auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(10.0)), glm::vec3(0, 0, 1));
          _scene->GetPlayer()->GetMesh()->ApplyTransform(transform);
          */
          _scene->GetPlayer()->RotateYaw(10.0);
        }
      break;

    case SDLK_q:
      if(pressed)
        std::cout << _scene->GetPlayer()->GetPosition() << std::endl;
      break;
    }

}


void GameStateGame::OnLivesUpdated()
{
  unsigned int used = 0;
  for(auto w : _lives_widgets)
    {
      w->SetIsVisible(used < _lives);
      used++;
    }
}


void GameStateGame::OnPlayerDies()
{
  const auto width = Settings->GetInt("screen_width");
  const auto height = Settings->GetInt("screen_height");

  _paused = true;
  const std::string t("Press the button to continue.");
  auto font = AssetLoader->LoadFont(20);
  double tlen = font->GetWidth(t);
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


void GameStateGame::NextLifeOrQuit()
{
  assert(_paused);
  assert(_pausebutton);
  
  _lives--;
  OnLivesUpdated();
  
  if(_lives > 0)
    {
      _scene->CreatePlayer();
      for(auto w : _player_status_widgets)
        w->SetSpaceship(_scene->GetPlayer());
    }
  else
    Quit();
  
  SetModalWidget(nullptr);
  _pausebutton->Destroy();
  _pausebutton = nullptr;
  _paused = false;
}


void GameStateGame::OnLevelChanged()
{
  auto level = _levels[_current_level];
  level->Start();
}
