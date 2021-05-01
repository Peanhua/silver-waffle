#ifndef INPUTMANAGER_HH_
#define INPUTMANAGER_HH_

#include <functional>
#include <SDL_keyboard.h>

class InputManager
{
public:
  InputManager();
  
  void Tick(double deltatime);
  bool IsRunning() const;

  void SetOnKeyboard(std::function<void(bool pressed, SDL_Keycode key, SDL_Keymod mod)> callback);
  
private:
  bool _running;
  
  std::function<void(bool pressed, SDL_Keycode key, SDL_Keymod mod)> _on_keyboard;
  
};

#endif
