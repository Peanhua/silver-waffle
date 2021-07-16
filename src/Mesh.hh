#ifndef MESH_HH_
#define MESH_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

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
      OPTION_COLOR              = 1<<0,
      OPTION_ELEMENT            = 1<<1,
      OPTION_TEXTURE            = 1<<2,
      OPTION_BLEND              = 1<<3,
      OPTION_GENERIC_VEC3_INPUT = 1<<4,
      OPTION_NORMAL             = 1<<5,
      OPTION_VERTEX_W           = 1<<6,
      OPTION_GENERIC_VEC2_INPUT = 1<<7,
      OPTION_VERTEX             = 1<<8,
      OPTION_DYNAMIC_VERTEX     = 1<<9,
    };
  enum AttribLocation
    {
      ALOC_VERTEX,
      ALOC_COLOR,
      ALOC_TEXCOORD,
      ALOC_NORMAL,
      ALOC_GENERIC_VEC2,
      ALOC_GENERIC_VEC3,
    };
  

  Mesh(unsigned int options);
  virtual ~Mesh();
  Mesh(const Mesh &);
  Mesh(Mesh &&)                  = delete; // todo: move constructor
  Mesh & operator=(const Mesh &) = delete; // todo: copy assignment
  Mesh & operator=(Mesh &&)      = delete; // todo: move assignment
  
  bool   LoadFromFile(const std::string & filename, const std::string & shader_prefix);
  void   UpdateGPU();
  void   UpdateGPU(unsigned int update_options, unsigned int first, unsigned int count);
  void   CalculateBoundingVolumes(const glm::mat4 & transform = glm::mat4(1));
  void   ApplyTransform(const glm::mat4 & transform);
  const glm::mat4 & GetTransform() const;
  void   SetTransform(const glm::mat4 & transform);

  virtual void PreDrawSetupShader(ShaderProgram * shader_program) const;
  void Draw(const glm::mat4 & model, const glm::mat4 & mvp, ShaderProgram * shader_program = nullptr) const;
  void         SetBoundingSphereRadius(double radius);
  double       GetBoundingSphereRadius() const;
  void              SetBoundingBoxHalfSize(const glm::vec3 & size);
  const glm::vec3 & GetBoundingBoxHalfSize() const;
  unsigned int GetOptions(unsigned int interested_in) const;
  
  void Clear();
  void ClearVertices();
  void ClearColors();
  void AddVertex(const glm::vec2 & vertex);
  void AddVertex(const glm::vec3 & vertex);
  void AddVertex(const glm::vec4 & vertex);
  void AddVertexT(const glm::vec3 & vertex, const glm::vec2 & texcoord);
  void SetVertex(unsigned int index, const glm::vec3 & vertex);
  void SetVertex(unsigned int index, const glm::vec4 & vertex);
  glm::vec3 GetVertex3(unsigned int index) const;
  glm::vec4 GetVertex4(unsigned int index) const;
  void AddColor(const glm::vec3 & color);
  void AddColor(const glm::vec4 & color);
  void SetAllColor(const glm::vec3 & color, bool children = true);
  void SetAllColor(const glm::vec4 & color, bool children = true);
  void AddTexCoord(const glm::vec2 & coord);
  void AddNormal(const glm::vec3 & normal);
  void AddElement(unsigned int index);
  void AddElement(unsigned int index1, unsigned int index2);
  void AddElement(unsigned int index1, unsigned int index2, unsigned int index3);
  void AddElement(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4);
  void AddGenericVecInput(const glm::vec2 & vector);
  void AddGenericVecInput(const glm::vec3 & vector);
  glm::vec2 GetGenericVec2(unsigned int index) const;

  GLenum GetPrimitiveType() const;
  void   SetPrimitiveType(const GLenum primitive_type);
  
  void            SetShaderProgram(ShaderProgram * shader_program, bool set_children = false);
  ShaderProgram * GetShaderProgram() const;

  void            SetTexture(unsigned int index, Image * texture_image, bool set_children = false);
  Image *         GetTexture(unsigned int index = 0) const;

  void   AddChild(Mesh * child);
  Mesh * FindChild(const std::string & name);
  
private:
  std::string  _name;
  unsigned int _options;

  glm::mat4 _transform;
  
  GLuint _vao;
  GLuint _vertex_vbo;
  GLuint _element_vbo;
  GLuint _color_vbo;
  GLuint _texcoord_vbo;
  GLuint _normal_vbo;
  GLuint _generic_vec2_vbo;
  GLuint _generic_vec3_vbo;
  GLenum _primitive_type;

  std::vector<GLfloat> _vertices;
  std::vector<GLuint>  _indices;
  std::vector<GLfloat> _colors;
  std::vector<GLfloat> _texcoords;
  std::vector<GLfloat> _normals;
  std::vector<GLfloat> _generic_vec2s;
  std::vector<GLfloat> _generic_vec3s;

  ShaderProgram * _shader_program;
  std::vector<Image *> _textures;

  double    _bounding_sphere_radius;
  glm::vec3 _bounding_box_size;

  std::vector<Mesh *> _children;

  bool LoadFromAssimpNode(const aiScene * scene, aiNode * node, bool first, const std::string & shader_prefix);
};

#endif
