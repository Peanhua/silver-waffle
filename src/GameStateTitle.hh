#ifndef GAME_STATE_TITLE_HH_
#define GAME_STATE_TITLE_HH_

#include "GameState.hh"
#include <vector>

class Camera;
class Milkyway;
class Object;
class SpaceParticles;
class WidgetTeletyper;


class GameStateTitle : public GameState
{
public:
  GameStateTitle();
  ~GameStateTitle();
  
  void Tick(double deltatime) override;
  void TickPlot();
  
private:
  double           _time;
  Milkyway *       _milkyway;
  SpaceParticles * _particles;
  Camera *         _camera;
  std::vector<Object *> _planets;
  WidgetTeletyper * _teletyper;
  unsigned int      _plot_phase;
};

#endif
