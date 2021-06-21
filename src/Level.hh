#ifndef LEVEL_HH_
#define LEVEL_HH_

#include "glm.hh"
#include <random>
#include <string>
#include <vector>
#include <json11.hpp>

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

  bool                IsFinished() const;
  const std::string & GetName()    const;
  double              GetTime()    const;
  
protected:
  class ProgramEntry
  {
  public:
    ProgramEntry();
    ProgramEntry(const json11::Json & config);

    void SetStartTime(double time);
    void SetStopTime(double time);
    void SetSpawnInterval(double interval);
    void SetInvaderControlProgram(const std::string & program_name);
    ProgramEntry * Tick(Scene * scene, std::mt19937_64 & random_generator, double deltatime, bool disable_spawning);
    void SetNext(ProgramEntry * next);
    double GetRemainingTime() const;
    
  private:
    double _invader_spawn_timer;
    double _invader_spawn_start_time;
    double _invader_spawn_stop_time;
    double _invader_spawn_interval;
    std::string _invader_control_program;
    bool   _boss;
    int    _invader_type;
    unsigned int _bosses_alive;
    ProgramEntry * _next;
  };
  
  Scene * _scene;
  
  std::mt19937_64 _random_generator;

  std::string    _name;
  double         _time;

  std::vector<ProgramEntry *> _program;

  void LoadConfig(const std::string & filename);
};

#endif
