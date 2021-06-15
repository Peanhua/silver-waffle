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
  Level(Scene * scene);
  virtual ~Level();
  
  virtual void   Start();
  virtual void   Tick(double deltatime);

  virtual bool   IsFinished()   const = 0;
  virtual double GetTotalTime() const = 0;

  double GetTime() const;
  
protected:
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

  double         _time;

  std::vector<ProgramEntry *> _program;
};

#endif
