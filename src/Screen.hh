#ifndef SCREEN_HH_
#define SCREEN_HH_

#include "glm.hh"
#include <SDL_keyboard.h>

class Widget;


class Screen
{
public:
  Screen();
  virtual ~Screen();

  Screen * GetChild() const;
  void SetChild(Screen * screen);

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
  bool     _running;
  Screen * _child;

  Widget * _root_widget;
  Widget * _focused_widget;
  Widget * _modal_widget;
};

#endif
