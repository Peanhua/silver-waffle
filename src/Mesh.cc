#include <cassert>
#include "Mesh.hh"

Mesh::Mesh()
  : _vertex_vbo(0),
    _color_vbo(0),
    _primitive_type(GL_TRIANGLES),
    _shader_program(nullptr),
    _bounding_sphere_radius(0)
{
}


void Mesh::Draw(const glm::mat4 & mvp) const
{
  assert(_shader_program);
  _shader_program->Use();
  _shader_program->SetMatrix("in_mvp", mvp);
  
  glBindBuffer(GL_ARRAY_BUFFER, this->_vertex_vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, this->_color_vbo);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glDrawArrays(this->_primitive_type, 0, static_cast<GLsizei>(this->_vertices.size()));
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}


void Mesh::AddVertex(const glm::vec3 & position)
{
  this->_vertices.push_back(position.x);
  this->_vertices.push_back(position.y);
  this->_vertices.push_back(position.z);
}


void Mesh::AddColor(const glm::vec3 & color)
{
  this->_colors.push_back(color.x);
  this->_colors.push_back(color.y);
  this->_colors.push_back(color.z);
}


void Mesh::SetPrimitiveType(const GLenum primitive_type)
{
  this->_primitive_type = primitive_type;
}


void Mesh::Update()
{
  if(this->_vertex_vbo == 0)
    glGenBuffers(1, &this->_vertex_vbo);
  if(this->_color_vbo == 0)
    glGenBuffers(1, &this->_color_vbo);

  glBindBuffer(GL_ARRAY_BUFFER, this->_vertex_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->_vertices.size() * sizeof(GLfloat)), this->_vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, this->_color_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(this->_colors.size() * sizeof(GLfloat)), this->_colors.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  CalculateBoundingSphereRadius();
}


void Mesh::SetShaderProgram(ShaderProgram * shader_program)
{
  _shader_program = shader_program;
}


ShaderProgram * Mesh::GetShaderProgram() const
{
  return _shader_program;
}


void Mesh::CalculateBoundingSphereRadius()
{
  _bounding_sphere_radius = 0.0;
  for(auto v : _vertices)
    {
      auto distance = glm::length(v);
      if(distance > _bounding_sphere_radius)
        _bounding_sphere_radius = distance;
    }
}


double Mesh::GetBoundingSphereRadius() const
{
  return _bounding_sphere_radius;
}

