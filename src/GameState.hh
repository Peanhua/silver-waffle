#ifndef GAME_STATE_HH_
#define GAME_STATE_HH_

#include <SDL_keyboard.h>

class GameState
{
public:
  GameState();
  virtual ~GameState();
  
  GameState * GetCurrentState();
  void Quit();
  
  virtual void Tick(double deltatime);
  virtual void OnKeyboard(bool pressed, SDL_Keycode key, SDL_Keymod mod);
  
private:
  bool _running;
};

#endif
