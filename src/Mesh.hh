#ifndef MESH_HH_
#define MESH_HH_

#include "glm.hh"
#include <GL/glew.h>
#include <vector>
#include <assimp/scene.h>

class ShaderProgram;
class Image;

class Mesh
{
public:
  enum Option
    {
      OPTION_COLOR         = 1<<0,
      OPTION_ELEMENT       = 1<<1,
      OPTION_TEXTURE       = 1<<2,
      OPTION_BLEND         = 1<<3,
      OPTION_BLEND_DISCARD = 1<<4,
    };
  enum AttribLocation
    {
      ALOC_VERTEX   = 0,
      ALOC_COLOR    = 1,
      ALOC_TEXCOORD = 2
    };
  

  Mesh(const unsigned int options);

  bool   LoadFromFile(const std::string & filename);
  void   UpdateGPU();
  void   CalculateBoundingSphereRadius(const glm::mat4 & transform = glm::mat4(1));
  void   ApplyTransform(const glm::mat4 & transform);
  
  void   Draw(const glm::mat4 & mvp) const;
  double GetBoundingSphereRadius()   const;

  void ClearVertices();
  void AddVertex(const glm::vec3 & position);
  void AddColor(const glm::vec3 & color);
  void AddTexCoord(const glm::vec2 & coord);
  void AddElement(unsigned int index);
  void AddElement(unsigned int index1, unsigned int index2);
  void AddElement(unsigned int index1, unsigned int index2, unsigned int index3);
  void AddElement(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4);
  void SetPrimitiveType(const GLenum primitive_type);
  
  void            SetShaderProgram(ShaderProgram * shader_program);
  ShaderProgram * GetShaderProgram() const;

  void            SetTexture(Image * texture_image);
  Image *         GetTexture() const;

  
private:
  unsigned int _options;

  glm::mat4 _transform;
  
  GLuint _vao;
  GLuint _vertex_vbo;
  GLuint _element_vbo;
  GLuint _color_vbo;
  GLuint _texcoord_vbo;
  GLenum _primitive_type;

  std::vector<GLfloat> _vertices;
  std::vector<GLuint>  _indices;
  std::vector<GLfloat> _colors;
  std::vector<GLfloat> _texcoords;

  ShaderProgram * _shader_program;
  Image *         _texture;

  double _bounding_sphere_radius;

  std::vector<Mesh *> _children;

  bool LoadFromAssimpNode(const aiScene * scene, aiNode * node, bool first);
};

#endif
