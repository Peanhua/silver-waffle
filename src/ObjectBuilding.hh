#ifndef OBJECT_BUILDING_HH_
#define OBJECT_BUILDING_HH_

#include "Object.hh"


class ObjectBuilding : public Object
{
public:
  ObjectBuilding(Scene * scene, unsigned int random_seed, unsigned int type);

  void Tick(double deltatime) override;

  void AddSpawn(unsigned int spawn_type, double spawn_interval, unsigned int max_count, const std::string & control_program);

private:
  struct SpawnEntry
  {
    SpawnEntry(unsigned int type_, double interval_, double next_spawn_time_, unsigned int max_count_, const std::string & control_program_)
      : type(type_),
        interval(interval_),
        next_spawn_time(next_spawn_time_),
        count(0),
        max_count(max_count_),
        control_program(control_program_)
    {
    }
    
    unsigned int type;
    double       interval;
    double       next_spawn_time;
    unsigned int count;
    unsigned int max_count;
    std::string  control_program;
  };

  std::vector<SpawnEntry> _spawns;
};

#endif
