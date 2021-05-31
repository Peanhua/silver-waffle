#include "GameStateTitle.hh"
#include "Camera.hh"
#include "GameStateGame.hh"
#include "Milkyway.hh"
#include "SpaceParticles.hh"
#include "SubsystemSettings.hh"
#include "Widget.hh"
#include <iostream>
#include <GL/glew.h>


GameStateTitle::GameStateTitle()
  : GameState(),
    _particles_cameramovement_timer(0.0),
    _particles_vertical_cameramovement(180.0f)
{
  _milkyway = new Milkyway();

  _particles = new SpaceParticles(15.0, 90.0, 0);

  _camera = new Camera();
  _camera->SetFOV(90.0);
  _camera->SetClippingPlanes(0.001, 300.0);
  _camera->UpdateProjection();

  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(width, height));
  assert(root);
  SetRootWidget(root);

  auto w = new Widget(root, glm::ivec2(width / 2 - 100, height / 2 - 100), glm::ivec2(200, 100));
  w->SetImage("Button-Play-Up");
  w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      SetChildState(new GameStateGame());
  });
  
  w = new Widget(root, glm::ivec2(width / 2 - 100, height / 2 + 100), glm::ivec2(200, 100));
  w->SetImage("Button-Quit-Up");
  w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      Quit();
  });
}


void GameStateTitle::Tick(double deltatime)
{
  const auto cammove_start = 30.0;
  const auto cammove_rise = 60.0;
  const auto cammove_magnitude = 4.0;
  
  _particles_cameramovement_timer += deltatime;

  auto t = _particles_cameramovement_timer - cammove_start;
  if(t > 0.0)
    {
      auto amount = 120.0 * deltatime;
      if(t < cammove_rise)
        amount *= t / cammove_rise;
      
      _particles_vertical_cameramovement += static_cast<float>(amount);
      
      if(_particles_vertical_cameramovement > 360.0f)
        _particles_vertical_cameramovement -= 360.0f;
    }

  _particles->Tick(deltatime);
  _camera->SetPosition(glm::vec3(0.0f,
                                 -20.0f,
                                 static_cast<float>(cammove_magnitude) * std::sin(glm::radians(_particles_vertical_cameramovement))));

  
  glDisable(GL_DEPTH_TEST);
  _milkyway->Draw(*_camera);
  
  glEnable(GL_DEPTH_TEST);

  _camera->UpdateView();
  _particles->Draw(*_camera);
  
  GameState::Tick(deltatime);
}
