#ifndef GAME_STATE_HH_
#define GAME_STATE_HH_

#include "glm.hh"
#include <SDL_keyboard.h>

class Widget;


class GameState
{
public:
  GameState();
  virtual ~GameState();

  GameState * GetChildState() const;
  void SetChildState(GameState * gamestate);

  bool IsRunning() const;
  void Quit();

  void     SetRootWidget(Widget * widget);
  Widget * GetRootWidget() const;

  void     SetModalWidget(Widget * widget);
  
  virtual void Tick(double deltatime);
  virtual void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod);
  virtual void OnMouseMove(const glm::ivec2 & position, const glm::ivec2 & relative_movement);
  virtual void OnMouseButton(bool pressed, unsigned int button, const glm::ivec2 & position);
  virtual void OnQuit();
  
private:
  bool        _running;
  GameState * _child;

  Widget * _root_widget;
  Widget * _focused_widget;
  Widget * _modal_widget;
};

#endif
