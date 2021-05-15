#include "GameStateTitle.hh"
#include "GameStateGame.hh"
#include "Widget.hh"
#include <iostream>


GameStateTitle::GameStateTitle()
  : GameState()
{
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
