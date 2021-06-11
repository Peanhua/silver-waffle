#ifndef ADDITIVE_BLENDING_HH_
#define ADDITIVE_BLENDING_HH_

class Mesh;

#include <GL/glew.h>


class AdditiveBlending
{
public:
  AdditiveBlending();

  void Blend(GLuint a, GLuint b);

private:
  Mesh * _mesh;
};

#endif
