#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_

#include "GameState.hh"

class Widget;

class GameStateTitle : public GameState
{
public:
  GameStateTitle();

  void Tick(double deltatime) override;
  void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod) override;
  void OnMouseMove(const glm::ivec2 & position, const glm::ivec2 & relative_movement) override;
  void OnMouseButton(bool pressed, unsigned int button, const glm::ivec2 & position) override;
  
private:
  Widget * _root;
  Widget * _focused;
};

#endif
