#ifndef MILKYWAY_HH_
#define MILKYWAY_HH_

class Camera;
class Mesh;

class Milkyway
{
public:
  Milkyway();

  void Draw(const Camera & camera) const;

private:
  Mesh * _mesh;
};

#endif
