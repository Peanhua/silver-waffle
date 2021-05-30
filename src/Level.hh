#ifndef LEVEL_HH_
#define LEVEL_HH_

#include <random>
#include <string>
#include <vector>

class Camera;
class Image;
class Mesh;
class Scene;


class Level
{
public:
  Level(Scene * scene, const std::string & planet_texture, double planet_size);

  void SetPlanetRing(float start, float end);
  
  void Tick(double deltatime);
  void Draw(const Camera & camera) const;

  void Start();
  bool IsFinished() const;
  
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

  Image * _planet_texture;
  double  _planet_size;
  Mesh *  _planet;
  Mesh *  _planet_ring;
  double  _planet_position_start;
  double  _planet_position;
  double  _planet_rotation;

  std::vector<ProgramEntry *> _program;
};

#endif
