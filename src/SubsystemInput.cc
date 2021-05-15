#include "SubsystemInput.hh"
#include "GameState.hh"
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
          if(!event.key.repeat)
            gamestate->OnKeyboard(event.type == SDL_KEYDOWN, event.key.keysym.sym, static_cast<SDL_Keymod>(event.key.keysym.mod));
          break;

        case SDL_MOUSEMOTION:
          gamestate->OnMouseMove(glm::ivec2(event.motion.x, event.motion.y), glm::ivec2(event.motion.xrel, event.motion.yrel));
          break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          {
            unsigned int b;
            
            if(event.button.button == SDL_BUTTON_MIDDLE)
              b = 2;
            else if(event.button.button == SDL_BUTTON_RIGHT)
              b = 1;
            else // if(event.button.button == SDL_BUTTON_LEFT)
              b = 0;
            
            bool pressed;
            
            if(event.type == SDL_MOUSEBUTTONDOWN)
              pressed = true;
            else /* if(event.type == SDL_MOUSEBUTTONUP) */
              pressed = false;

            gamestate->OnMouseButton(pressed, b, glm::ivec2(event.button.x, event.button.y));
          }
          break;
        }
    }
}


bool SubsystemInput::IsRunning() const
{
  return _running;
}
