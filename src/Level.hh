#ifndef LEVEL_HH_
#define LEVEL_HH_

#include <string>

class Camera;
class Image;
class Mesh;

class Level
{
public:
  Level(const std::string & planet_texture, double distance_multiplier);
  
  void Tick(double deltatime);
  void Draw(const Camera & camera) const;

  void Start();
  bool IsFinished() const;
  
private:
  Image * _planet_texture;
  double  _distance_multiplier;

  Mesh *  _planet;
  double  _planet_position;
  double  _planet_rotation;
};

#endif
