#ifndef LEVEL_HH_
#define LEVEL_HH_

#include <random>
#include <string>

class Camera;
class Image;
class Mesh;
class Scene;


class Level
{
public:
  Level(Scene * scene, const std::string & planet_texture);
  
  void Tick(double deltatime);
  void Draw(const Camera & camera) const;

  void Start();
  bool IsFinished() const;
  
private:
  Scene * _scene;
  
  std::mt19937_64 _random_generator;

  Image * _planet_texture;
  Mesh *  _planet;
  double  _planet_position;
  double  _planet_rotation;

  double _invader_spawn_timer;
};

#endif
