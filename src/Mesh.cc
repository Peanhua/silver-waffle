#include "Mesh.hh"
#include <cassert>
#include <iostream>

Mesh::Mesh(const unsigned int options)
  : _options(options),
    _transform(1),
    _vao(0),
    _vertex_vbo(0),
    _element_vbo(0),
    _color_vbo(0),
    _primitive_type(GL_TRIANGLES),
    _shader_program(nullptr),
    _bounding_sphere_radius(0)
{
}


void Mesh::Draw(const glm::mat4 & mvp) const
{
  const glm::mat4 transform(mvp * _transform);

  if(_vertices.size() > 0)
    {
      assert(_shader_program);
      _shader_program->Use();

      _shader_program->SetMatrix("in_mvp", transform);
      
      glBindVertexArray(_vao);
      if(_options & OPTION_ELEMENT)
        glDrawElements(_primitive_type, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, nullptr);
      else
        glDrawArrays(_primitive_type, 0, static_cast<GLsizei>(_vertices.size()));
    }

  for(auto c : _children)
    c->Draw(transform);
}


void Mesh::AddVertex(const glm::vec3 & position)
{
  _vertices.push_back(position.x);
  _vertices.push_back(position.y);
  _vertices.push_back(position.z);
}


void Mesh::AddColor(const glm::vec3 & color)
{
  assert(_options & OPTION_COLOR);
  _colors.push_back(color.x);
  _colors.push_back(color.y);
  _colors.push_back(color.z);
}


void Mesh::AddElement(unsigned int index)
{
  _indices.push_back(index);
}


void Mesh::AddElement(unsigned int index1, unsigned int index2)
{
  AddElement(index1);
  AddElement(index2);
}


void Mesh::AddElement(unsigned int index1, unsigned int index2, unsigned int index3)
{
  AddElement(index1, index2);
  AddElement(index3);
}


void Mesh::AddElement(unsigned int index1, unsigned int index2, unsigned int index3, unsigned int index4)
{
  AddElement(index1, index2, index3);
  AddElement(index4);
}



void Mesh::SetPrimitiveType(const GLenum primitive_type)
{
  _primitive_type = primitive_type;
}


void Mesh::UpdateGPU()
{
  {
    if(_vao == 0)
      glGenVertexArrays(1, &_vao);
    assert(_vao != 0);
    glBindVertexArray(_vao);
  }

  {
    if(_vertex_vbo == 0)
      glGenBuffers(1, &_vertex_vbo);
    assert(_vertex_vbo != 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_vertices.size() * sizeof(GLfloat)), _vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(ALOC_VERTEX);
    glVertexAttribPointer(ALOC_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  }
  
  if(_options & OPTION_ELEMENT)
    {
      if(_element_vbo == 0)
        glGenBuffers(1, &_element_vbo);
      assert(_element_vbo != 0);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _element_vbo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(_indices.size() * sizeof(GLuint)), _indices.data(), GL_STATIC_DRAW);
    }

  if(_options & OPTION_COLOR)
    {
      if(_color_vbo == 0)
        glGenBuffers(1, &_color_vbo);
      assert(_color_vbo != 0);

      glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_colors.size() * sizeof(GLfloat)), _colors.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(ALOC_COLOR);
      glVertexAttribPointer(ALOC_COLOR, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

  for(auto c : _children)
    c->UpdateGPU();
}


void Mesh::SetShaderProgram(ShaderProgram * shader_program)
{
  _shader_program = shader_program;
}


ShaderProgram * Mesh::GetShaderProgram() const
{
  return _shader_program;
}


void Mesh::CalculateBoundingSphereRadius(const glm::mat4 & transform)
{
  const glm::mat4 mytransform(transform * _transform);
  
  _bounding_sphere_radius = 0.0;
  for(auto c : _children)
    {
      c->CalculateBoundingSphereRadius(mytransform);
      _bounding_sphere_radius = std::max(_bounding_sphere_radius, c->GetBoundingSphereRadius());
    }

  for(unsigned int i = 0; i < _vertices.size() / 3; i++)
    {
      glm::vec4 v(_vertices[i * 3 + 0], _vertices[i * 3 + 1], _vertices[i * 3 + 2], 1);
      v = v * mytransform;
      v = v / v.w;
      _bounding_sphere_radius = std::max(_bounding_sphere_radius, static_cast<double>(glm::length(v.xyz())));
    }
}


double Mesh::GetBoundingSphereRadius() const
{
  return _bounding_sphere_radius;
}

