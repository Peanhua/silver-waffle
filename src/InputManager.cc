#include "InputManager.hh"
#include <SDL.h>
#include <cassert>

InputManager::InputManager()
  : _running(true)
{
}


void InputManager::Tick(double deltatime)
{
  assert(deltatime == deltatime);
  
  SDL_Event event;

  while(this->_running && SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_QUIT:
          this->_running = false;
          break;
          
        case SDL_WINDOWEVENT:
          {
            switch(event.window.event)
              {
              case SDL_WINDOWEVENT_CLOSE:
                this->_running = false;
                break;
              }
          }
          break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
          if(this->_on_keyboard)
            this->_on_keyboard(event.type == SDL_KEYDOWN, event.key.keysym.sym, static_cast<SDL_Keymod>(event.key.keysym.mod));
          break;
        }
    }
}


bool InputManager::IsRunning() const
{
  return this->_running;
}


void InputManager::SetOnKeyboard(std::function<void(bool pressed, SDL_Keycode key, SDL_Keymod mod)> callback)
{
  this->_on_keyboard = callback;
}
