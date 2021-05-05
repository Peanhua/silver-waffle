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
      OPTION_COLOR   = 1,
      OPTION_ELEMENT = 2
    };

  Mesh(const unsigned int options);

  void   Draw(const glm::mat4 & mvp) const;
  double GetBoundingSphereRadius()   const;
  
protected:
  void AddVertex(const glm::vec3 & position);
  void AddColor(const glm::vec3 & color);
  void AddElement(unsigned int index);
  void AddElement(unsigned int index1, unsigned int index2);
  void AddElement(unsigned int index1, unsigned int index2, unsigned int index3);
  void AddElement(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4);
  void SetPrimitiveType(const GLenum primitive_type);
  void Update();
  
  void            SetShaderProgram(ShaderProgram * shader_program);
  ShaderProgram * GetShaderProgram() const;

  
private:
  enum AttribLocation
    {
      ALOC_VERTEX = 0,
      ALOC_COLOR  = 1
    };
  
  unsigned int _options;
  
  GLuint _vao;
  GLuint _vertex_vbo;
  GLuint _element_vbo;
  GLuint _color_vbo;
  GLenum _primitive_type;

  std::vector<GLfloat> _vertices;
  std::vector<GLuint>  _indices;
  std::vector<GLfloat> _colors;

  ShaderProgram * _shader_program;

  double _bounding_sphere_radius;

  void CalculateBoundingSphereRadius();
};

#endif
