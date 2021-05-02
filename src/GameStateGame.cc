#include "GameStateGame.hh"
#include "SubsystemAssetLoader.hh"
#include "Camera2.hh"
#include <iostream>

GameStateGame::GameStateGame()
  : GameState()
{
  auto vs = AssetLoader->LoadText("x.vert");
  auto fs = AssetLoader->LoadText("x.frag");
  std::vector<std::string> vss({vs});
  std::vector<std::string> fss({fs});
  _shaderprog = new ShaderProgram(vss, fss);
  glUseProgram(_shaderprog->GetProgram());

  glm::vec3 pos(0, 0, 0);
#if 0
  _camera = new Camera();
  _level = new GameLevel(pos);
  _fov = 60.0;
# define CAMERA_SPEED 0.5
#else
  _camera = new Camera2();
  _level = new GameLevel2(pos);
  _fov = 120.0;
# define CAMERA_SPEED 1
#endif
  
  _camera->SetFOV(_fov);
  _camera->UpdateViewProjection();
  
  _level->Initialize(1.0);

}


GameStateGame::~GameStateGame()
{
  delete _level;
  delete _camera;
  delete _shaderprog;
}


void GameStateGame::Tick(double deltatime)
{
  assert(deltatime == deltatime);
  _level->Draw(*_camera, *_shaderprog);
}


void GameStateGame::OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod)
{
  assert(mod == mod);
    
  if(!pressed)
    return;
    
  switch(key)
    {
    case SDLK_LEFT:
      _camera->MoveRight(-CAMERA_SPEED * 2.0);
      break;
    case SDLK_RIGHT:
      _camera->MoveRight(CAMERA_SPEED * 2.0);
      break;
    case SDLK_UP:
      _camera->MoveForward(CAMERA_SPEED);
      break;
    case SDLK_DOWN:
      _camera->MoveForward(-CAMERA_SPEED);
      break;
    case SDLK_PAGEUP:
      _camera->MoveUp(CAMERA_SPEED);
      break;
    case SDLK_PAGEDOWN:
      _camera->MoveUp(-CAMERA_SPEED);
      break;
    case SDLK_PLUS:
      _fov++;
      std::cout << "fov:" << _fov << std::endl;
      _camera->SetFOV(_fov);
      _camera->UpdateViewProjection();
      break;
    case SDLK_MINUS:
      _fov--;
      std::cout << "fov:" << _fov << std::endl;
      _camera->SetFOV(_fov);
      _camera->UpdateViewProjection();
      break;
    }
}
