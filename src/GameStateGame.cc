#include "GameStateGame.hh"
#include "SubsystemAssetLoader.hh"
#include "Camera2.hh"
#include <iostream>

GameStateGame::GameStateGame()
  : GameState(),
    _player_control_left(false),
    _player_control_right(false)
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
}


GameStateGame::~GameStateGame()
{
  delete _scene;
  delete _camera;
}


void GameStateGame::Tick(double deltatime)
{
  if(_player_control_left)
    _scene->GetPlayer()->AddImpulse(glm::vec3(-20.0 * deltatime, 0, 0));
  if(_player_control_right)
    _scene->GetPlayer()->AddImpulse(glm::vec3(20.0 * deltatime, 0, 0));

  if(!_player_control_left && !_player_control_right)
    {
      const glm::vec3 vel = _scene->GetPlayer()->GetVelocity();
      if(std::abs(vel.x) < 0.1f)
        _scene->GetPlayer()->SetVelocity(glm::vec3(0, vel.y, vel.z));
      else if(vel.x < 0.0f)
        _scene->GetPlayer()->AddImpulse(glm::vec3(10.0 * deltatime, 0, 0));
      else if(vel.x > 0.0f)
        _scene->GetPlayer()->AddImpulse(glm::vec3(-10.0 * deltatime, 0, 0));
    }

  
  _scene->Tick(deltatime);
  _scene->Draw(_camera->GetViewProjection());
}


void GameStateGame::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);

  switch(key)
    {
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
      _player_control_left = pressed;
      break;
      
    case SDLK_RIGHT:
      _player_control_right = pressed;
      break;

    case SDLK_SPACE:
      _scene->AddPlayerBullet(glm::vec3(0, 0, 10), 10.0);
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
          auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(10.0)), glm::vec3(1, 0, 0));
          _scene->GetPlayer()->GetMesh()->ApplyTransform(transform);
        }
      break;

    case SDLK_y:
      if(pressed)
        {
          auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(10.0)), glm::vec3(0, 1, 0));
          _scene->GetPlayer()->GetMesh()->ApplyTransform(transform);
        }
      break;
      
    case SDLK_z:
      if(pressed)
        {
          auto transform = glm::rotate(glm::mat4(1), static_cast<float>(glm::radians(10.0)), glm::vec3(0, 0, 1));
          _scene->GetPlayer()->GetMesh()->ApplyTransform(transform);
        }
      break;
    }

}
