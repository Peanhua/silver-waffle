#ifndef MESH_HH_
#define MESH_HH_

#include "glm.hh"
#include <GL/glew.h>
#include <vector>

class Mesh
{
public:
  Mesh();

  void Draw() const;

protected:
  void AddVertex(const glm::vec3 & position);
  void AddColor(const glm::vec3 & color);
  void SetPrimitiveType(const GLenum primitive_type);
  void Update();
  
private:
  GLuint _vertex_vbo;
  GLuint _color_vbo;
  GLenum _primitive_type;

  std::vector<GLfloat> _vertices;
  std::vector<GLfloat> _colors;
};

#endif
