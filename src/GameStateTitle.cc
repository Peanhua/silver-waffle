#include "GameStateTitle.hh"
#include "GameStateGame.hh"
#include "Starfield.hh"
#include "SubsystemSettings.hh"
#include "Widget.hh"
#include <iostream>
#include <GL/glew.h>


GameStateTitle::GameStateTitle()
  : GameState(),
    _starfield_cameramovement_timer(0.0),
    _starfield_vertical_cameramovement(180.0f)
{
  _starfield = new Starfield(15.0, 90.0, 0);

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
  
  _starfield_cameramovement_timer += deltatime;

  auto t = _starfield_cameramovement_timer - cammove_start;
  if(t > 0.0)
    {
      auto amount = 120.0 * deltatime;
      if(t < cammove_rise)
        amount *= t / cammove_rise;
      
      _starfield_vertical_cameramovement += static_cast<float>(amount);
      
      if(_starfield_vertical_cameramovement > 360.0f)
        _starfield_vertical_cameramovement -= 360.0f;
    }
  
  glEnable(GL_DEPTH_TEST);
  _starfield->Tick(deltatime);
  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");
  glm::mat4 proj = glm::perspective(glm::radians(90.0), width / height, 0.001, 300.0);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -20.0f, cammove_magnitude * std::sin(glm::radians(_starfield_vertical_cameramovement))),
                               glm::vec3(0, 0, 0),
                               glm::vec3(0, 0, 1));
  _starfield->Draw(view, proj, proj * view);
  
  GameState::Tick(deltatime);
}
