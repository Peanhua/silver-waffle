#include <cassert>
#include "Mesh.hh"

Mesh::Mesh(Option options)
  : _options(options),
    _vao(0),
    _vertex_vbo(0),
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
  
  glBindVertexArray(_vao);
  glDrawArrays(_primitive_type, 0, static_cast<GLsizei>(_vertices.size()));
}


void Mesh::AddVertex(const glm::vec3 & position)
{
  _vertices.push_back(position.x);
  _vertices.push_back(position.y);
  _vertices.push_back(position.z);
}


void Mesh::AddColor(const glm::vec3 & color)
{
  _colors.push_back(color.x);
  _colors.push_back(color.y);
  _colors.push_back(color.z);
}


void Mesh::SetPrimitiveType(const GLenum primitive_type)
{
  _primitive_type = primitive_type;
}


void Mesh::Update()
{
  if(_vao == 0)
    glGenVertexArrays(1, &_vao);
  assert(_vao != 0);
  
  if(_vertex_vbo == 0)
    glGenBuffers(1, &_vertex_vbo);
  assert(_vertex_vbo != 0);

  if(_options & Option::COLOR)
    {
      if(_color_vbo == 0)
        glGenBuffers(1, &_color_vbo);
      assert(_color_vbo != 0);
    }

  
  glBindVertexArray(_vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, _vertex_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_vertices.size() * sizeof(GLfloat)), _vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  if(_options & Option::COLOR)
    {
      glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_colors.size() * sizeof(GLfloat)), _colors.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

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
      double distance = glm::length(v);
      if(distance > _bounding_sphere_radius)
        _bounding_sphere_radius = distance;
    }
}


double Mesh::GetBoundingSphereRadius() const
{
  return _bounding_sphere_radius;
}

