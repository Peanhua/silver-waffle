#ifndef MESH_HH_
#define MESH_HH_

#include "glm.hh"
#include "ShaderProgram.hh"
#include <GL/glew.h>
#include <vector>

class Mesh
{
public:
  enum Option
    {
      COLOR = 1
    };
  
  Mesh(const Option options);

  void   Draw(const glm::mat4 & mvp) const;
  double GetBoundingSphereRadius()   const;
  
protected:
  void AddVertex(const glm::vec3 & position);
  void AddColor(const glm::vec3 & color);
  void SetPrimitiveType(const GLenum primitive_type);
  void Update();
  
  void            SetShaderProgram(ShaderProgram * shader_program);
  ShaderProgram * GetShaderProgram() const;

  
private:
  Option _options;
  
  GLuint _vao;
  GLuint _vertex_vbo;
  GLuint _color_vbo;
  GLenum _primitive_type;

  std::vector<GLfloat> _vertices;
  std::vector<GLfloat> _colors;

  ShaderProgram * _shader_program;

  double _bounding_sphere_radius;

  void CalculateBoundingSphereRadius();
};

#endif
