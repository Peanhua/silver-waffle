#ifndef GAUSSIAN_BLUR_HH_
#define GAUSSIAN_BLUR_HH_

class Mesh;

#include <vector>
#include <GL/glew.h>


class GaussianBlur
{
public:
  GaussianBlur();
  ~GaussianBlur();

  GLuint Blur(unsigned int count, GLuint source_texture_id);

private:
  std::vector<GLuint> _fbos;
  std::vector<GLuint> _texture_ids;
  std::vector<Mesh *> _meshes;
};

#endif
