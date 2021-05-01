#include <cassert>
#include "Mesh.hh"

Mesh::Mesh()
  : _vertex_vbo(0),
    _color_vbo(0),
    _primitive_type(GL_TRIANGLES)
{
}


void Mesh::Draw() const
{
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
}
