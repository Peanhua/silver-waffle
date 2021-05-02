#include "SubsystemInput.hh"
#include <SDL.h>
#include <cassert>

SubsystemInput::SubsystemInput(SubsystemGameState & subsystem_gamestate)
  : Subsystem("Input"),
    _running(true),
    _subsystem_gamestate(subsystem_gamestate)
{
}

bool SubsystemInput::Start()
{
  return _running;
}


void SubsystemInput::Stop()
{
}


void SubsystemInput::Tick(double deltatime)
{
  assert(deltatime == deltatime);

  auto gamestate = _subsystem_gamestate.GetGameState();
  if(!gamestate)
    return;
  
  SDL_Event event;

  while(this->_running && SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_QUIT:
          _running = false;
          break;
          
        case SDL_WINDOWEVENT:
          {
            switch(event.window.event)
              {
              case SDL_WINDOWEVENT_CLOSE:
                _running = false;
                break;
              }
          }
          break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
          gamestate->OnKeyboard(event.type == SDL_KEYDOWN, event.key.keysym.sym, static_cast<SDL_Keymod>(event.key.keysym.mod));
          break;
        }
    }
}


bool SubsystemInput::IsRunning() const
{
  return _running;
}
