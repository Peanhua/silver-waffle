#include "GameStateGame.hh"
#include "Starfield.hh"
#include "Camera2.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "ScoreReel.hh"
#include "SubsystemAssetLoader.hh"
#include "WidgetPlayerShip.hh"
#include "WidgetSpaceshipStatus.hh"
#include <iostream>
#include <GL/glew.h>


GameStateGame::GameStateGame()
  : GameState(),
    _lives(3)
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
  _camera->UpdateViewProjection();
  
  _scene->Initialize(1.0);
  _scene->SetOnDestroyed([this](Object * destroyer, Object * target)
  {
    if(destroyer == _scene->GetPlayer() && target != _scene->GetPlayer())
      _score_reel->SetScore(_score_reel->GetScore() + 1);
    else if(target == _scene->GetPlayer())
      OnPlayerDies();
  });

  _score_reel = new ScoreReel(10);

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(1024, 768));
  assert(root);
  SetRootWidget(root);

  for(int i = 0; i < 5; i++)
    {
      auto w = new WidgetPlayerShip(root, glm::ivec2(960 - i * 50, 10), glm::ivec2(50, 50));
      _lives_widgets.push_back(w);
    }
  OnLivesUpdated();

  auto w = new WidgetSpaceshipStatus(root, glm::ivec2(990, 70), glm::ivec2(20, 100));
  w->SetSpaceship(_scene->GetPlayer());
  _playership_status_widget = w;

  _starfield = new Starfield(5.0, 50.0, 0);
}


GameStateGame::~GameStateGame()
{
  delete _scene;
  delete _camera;
}


void GameStateGame::Tick(double deltatime)
{
  glEnable(GL_DEPTH_TEST);

  _starfield->Tick(deltatime);
  _scene->Tick(deltatime);
  _score_reel->Tick(deltatime);

  _starfield->Draw(_camera->GetView(), _camera->GetProjection(), _camera->GetViewProjection());
  _scene->Draw(_camera->GetView(), _camera->GetProjection(), _camera->GetViewProjection());
  _score_reel->Draw();

  GameState::Tick(deltatime);
}


void GameStateGame::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

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
      _scene->GetPlayer()->FireWeapon(0);
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
          _camera->UpdateViewProjection();
        }
      break;
      
    case SDLK_MINUS:
      if(pressed)
        {
          _fov--;
          std::cout << "fov:" << _fov << std::endl;
          _camera->SetFOV(_fov);
          _camera->UpdateViewProjection();
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
  _lives--;
  OnLivesUpdated();
  
  if(_lives > 0)
    {
      auto player = _scene->GetPlayer();
      player->SetHealth(100);
      player->SetPosition(glm::vec3(0, player->GetPosition().yz()));
    }
  else
    Quit();
}
