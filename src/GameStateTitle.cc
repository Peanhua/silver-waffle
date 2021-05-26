#include "GameStateTitle.hh"
#include "Starfield.hh"
#include "GameStateGame.hh"
#include "Widget.hh"
#include <iostream>
#include <GL/glew.h>


GameStateTitle::GameStateTitle()
  : GameState()
{
  _starfield = new Starfield(1.0, 30.0, 0);

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(1024, 768));
  assert(root);
  SetRootWidget(root);

  auto w = new Widget(root, glm::ivec2(1024/2 - 100, 768/2 - 100), glm::ivec2(200, 100));
  w->SetImage("Button-Play-Up");
  w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
  {
    assert(button == button);
    assert(position == position);
    if(!pressed)
      SetChildState(new GameStateGame());
  });
  
  w = new Widget(root, glm::ivec2(1024/2 - 100, 768/2 + 100), glm::ivec2(200, 100));
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
  glEnable(GL_DEPTH_TEST);
  _starfield->Tick(deltatime);
  glm::mat4 proj = glm::perspective(glm::radians(90.0), 1024.0 / 768.0, 0.001, 300.0);
  glm::mat4 view = glm::lookAt(glm::vec3(0, -20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  _starfield->Draw(view, proj, proj * view);
  
  GameState::Tick(deltatime);
}
