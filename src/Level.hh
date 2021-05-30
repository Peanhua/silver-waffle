#ifndef LEVEL_HH_
#define LEVEL_HH_

#include "glm.hh"
#include <random>
#include <string>
#include <vector>

class Camera;
class Image;
class Mesh;
class ObjectPlanet;
class Scene;


class Level
{
public:
  Level(Scene * scene, const std::string & planet_texture, double planet_size, const glm::vec2 & planet_ring_radius = glm::vec2(0, 0));

  void   Start();
  void   Tick(double deltatime);

  bool   IsFinished()   const;
  double GetTotalTime() const;
  double GetTime()      const;
  
private:
  class ProgramEntry
  {
  public:
    ProgramEntry();

    void SetStartTime(double time);
    void SetStopTime(double time);
    void SetSpawnInterval(double interval);
    void Tick(Scene * scene, std::mt19937_64 & random_generator, double deltatime);
    
  private:
    double _invader_spawn_timer;
    double _invader_spawn_start_time;
    double _invader_spawn_stop_time;
    double _invader_spawn_interval;
  };
  
  Scene * _scene;
  
  std::mt19937_64 _random_generator;

  double         _planet_size;
  double         _planet_position_start;
  ObjectPlanet * _planet;
  double         _time;

  std::vector<ProgramEntry *> _program;
};

#endif
