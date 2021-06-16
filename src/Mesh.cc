#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "Image.hh"
#include <cassert>
#include <iostream>

Mesh::Mesh(unsigned int options)
  : _options(options | OPTION_VERTEX),
    _transform(1),
    _vao(0),
    _vertex_vbo(0),
    _element_vbo(0),
    _color_vbo(0),
    _texcoord_vbo(0),
    _normal_vbo(0),
    _generic_vec2_vbo(0),
    _generic_vec3_vbo(0),
    _primitive_type(GL_TRIANGLES),
    _shader_program(nullptr),
    _bounding_sphere_radius(0)
{
}


Mesh::~Mesh()
{
  if(_vao != 0)
    glDeleteVertexArrays(1, &_vao);

  std::vector<GLuint> vbos {
    _vertex_vbo,
    _element_vbo,
    _color_vbo,
    _texcoord_vbo,
    _normal_vbo,
    _generic_vec2_vbo,
    _generic_vec3_vbo,
  };
  for(auto vbo : vbos)
    if(vbo != 0)
      glDeleteBuffers(1, &vbo);

  for(auto c : _children)
    delete c;
}


Mesh::Mesh(const Mesh & other)
  : _options(other._options),
    _transform(other._transform),
    _vao(0),
    _vertex_vbo(0),
    _element_vbo(0),
    _color_vbo(0),
    _texcoord_vbo(0),
    _normal_vbo(0),
    _generic_vec2_vbo(0),
    _generic_vec3_vbo(0),
    _primitive_type(other._primitive_type),
    _vertices(other._vertices),
    _indices(other._indices),
    _colors(other._colors),
    _texcoords(other._texcoords),
    _normals(other._normals),
    _generic_vec2s(other._generic_vec2s),
    _generic_vec3s(other._generic_vec3s),
    _shader_program(other._shader_program),
    _bounding_sphere_radius(other._bounding_sphere_radius)
{
  for(auto c : other._children)
    {
      Mesh * child = new Mesh(*c);
      _children.push_back(child);
    }
}


void Mesh::PreDrawSetupShader(ShaderProgram * shader_program) const
{
  assert(shader_program == shader_program);
}


void Mesh::Draw(const glm::mat4 & model, const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & mvp, ShaderProgram * shader_program) const
{
  const glm::mat4 mymvp(mvp * _transform);
  const glm::mat4 mymodel(model * _transform);

  if(_vertices.size() > 0)
    {
      auto shader = shader_program ? shader_program : _shader_program;
      assert(shader);
      shader->Use();

      if(_options & OPTION_TEXTURE)
        {
          assert(_textures.size() > 0);
          for(unsigned int i = 0; i < _textures.size(); i++)
            {
              glActiveTexture(GL_TEXTURE0 + i);
              glBindTexture(GL_TEXTURE_2D, _textures[i]->GetTextureId());
              shader->SetInt("texture" + std::to_string(i), static_cast<int>(i));
            }
          glActiveTexture(GL_TEXTURE0);
        }

      if(_options & OPTION_BLEND)
        glEnable(GL_BLEND);

      shader->SetMatrix("in_model",      mymodel);
      shader->SetMatrix("in_view",       view);
      shader->SetMatrix("in_projection", projection);
      shader->SetMatrix("in_mvp",        mymvp);
      PreDrawSetupShader(shader);
      
      glBindVertexArray(_vao);
      if(_options & OPTION_ELEMENT)
        glDrawElements(_primitive_type, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, nullptr);
      else
        glDrawArrays(_primitive_type, 0, static_cast<GLsizei>(_vertices.size()));

      if(_options & OPTION_BLEND)
        glDisable(GL_BLEND);
    }

  for(auto c : _children)
    c->Draw(mymodel, view, projection, mymvp, shader_program);
}


void Mesh::ClearVertices()
{
  _vertices.clear();
}


void Mesh::ClearColors()
{
  _colors.clear();
}


void Mesh::Clear()
{
  _vertices.clear();
  _indices.clear();
  _colors.clear();
  _texcoords.clear();
  _normals.clear();
  _generic_vec2s.clear();
  _generic_vec3s.clear();
}


void Mesh::AddVertex(const glm::vec3 & vertex)
{
  assert(!(_options & OPTION_VERTEX_W));
  _vertices.push_back(vertex.x);
  _vertices.push_back(vertex.y);
  _vertices.push_back(vertex.z);
}


void Mesh::AddVertex(const glm::vec4 & vertex)
{
  assert(_options & OPTION_VERTEX_W);
  _vertices.push_back(vertex.x);
  _vertices.push_back(vertex.y);
  _vertices.push_back(vertex.z);
  _vertices.push_back(vertex.w);
}


void Mesh::AddVertexT(const glm::vec3 & vertex, const glm::vec2 & texcoord)
{
  AddVertex(vertex);
  AddTexCoord(texcoord);
}


void Mesh::SetVertex(unsigned int index, const glm::vec3 & vertex)
{
  assert(!(_options & OPTION_VERTEX_W));
  assert(index * 3 + 2 < _vertices.size());
  _vertices[index * 3 + 0] = vertex.x;
  _vertices[index * 3 + 1] = vertex.y;
  _vertices[index * 3 + 2] = vertex.z;
}


void Mesh::SetVertex(unsigned int index, const glm::vec4 & vertex)
{
  assert(_options & OPTION_VERTEX_W);
  assert(index * 4 + 3 < _vertices.size());
  _vertices[index * 4 + 0] = vertex.x;
  _vertices[index * 4 + 1] = vertex.y;
  _vertices[index * 4 + 2] = vertex.z;
  _vertices[index * 4 + 3] = vertex.w;
}  


glm::vec3 Mesh::GetVertex3(unsigned int index) const
{
  assert(!(_options & OPTION_VERTEX_W));
  assert(index * 3 + 2 < _vertices.size());
  return glm::vec3(_vertices[index * 3 + 0],
                   _vertices[index * 3 + 1],
                   _vertices[index * 3 + 2]);
}


glm::vec4 Mesh::GetVertex4(unsigned int index) const
{
  assert(_options & OPTION_VERTEX_W);
  assert(index < _vertices.size() + 4);
  return glm::vec4(_vertices[index * 4 + 0],
                   _vertices[index * 4 + 1],
                   _vertices[index * 4 + 2],
                   _vertices[index * 4 + 3]);
}


void Mesh::AddColor(const glm::vec3 & color)
{
  assert(_options & OPTION_COLOR);
  assert(!(_options & OPTION_BLEND));
  _colors.push_back(color.r);
  _colors.push_back(color.g);
  _colors.push_back(color.b);
  _colors.push_back(1.0f);
}


void Mesh::AddColor(const glm::vec4 & color)
{
  assert(_options & OPTION_COLOR);
  assert(_options & OPTION_BLEND);
  _colors.push_back(color.r);
  _colors.push_back(color.g);
  _colors.push_back(color.b);
  _colors.push_back(color.a);
}


void Mesh::SetAllColor(const glm::vec3 & color, bool children)
{
  _colors.clear();
  for(unsigned int i = 0; i < _vertices.size(); i++)
    AddColor(color);

  if(children)
    for(auto c : _children)
      c->SetAllColor(color, children);
}


void Mesh::SetAllColor(const glm::vec4 & color, bool children)
{
  _colors.clear();
  for(unsigned int i = 0; i < _vertices.size(); i++)
    AddColor(color);

  if(children)
    for(auto c : _children)
      c->SetAllColor(color, true);
}


void Mesh::AddTexCoord(const glm::vec2 & coord)
{
  assert(_options & OPTION_TEXTURE);
  _texcoords.push_back(coord.x);
  _texcoords.push_back(coord.y);
}


void Mesh::AddNormal(const glm::vec3 & normal)
{
  assert(_options & OPTION_NORMAL);
  _normals.push_back(normal.x);
  _normals.push_back(normal.y);
  _normals.push_back(normal.z);
}


void Mesh::AddElement(unsigned int index)
{
  assert(_options & OPTION_ELEMENT);
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


void Mesh::AddGenericVecInput(const glm::vec2 & vector)
{
  assert(_options & OPTION_GENERIC_VEC2_INPUT);
  _generic_vec2s.push_back(vector.x);
  _generic_vec2s.push_back(vector.y);
}


void Mesh::AddGenericVecInput(const glm::vec3 & vector)
{
  assert(_options & OPTION_GENERIC_VEC3_INPUT);
  _generic_vec3s.push_back(vector.x);
  _generic_vec3s.push_back(vector.y);
  _generic_vec3s.push_back(vector.z);
}


glm::vec2 Mesh::GetGenericVec2(unsigned int index) const
{
  assert(_options & OPTION_GENERIC_VEC2_INPUT);
  assert(index * 2 + 1 < _generic_vec2s.size());
  return glm::vec2(_generic_vec2s[index * 2 + 0],
                   _generic_vec2s[index * 2 + 1]);
}


GLenum Mesh::GetPrimitiveType() const
{
  return _primitive_type;
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

    const unsigned int components = (_options & OPTION_VERTEX_W) ? 4 : 3;
    assert(_vertices.size() % components == 0);
    const GLenum usage = (_options & OPTION_DYNAMIC_VERTEX) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    
    glBindBuffer(GL_ARRAY_BUFFER, _vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_vertices.size() * sizeof(GLfloat)), _vertices.data(), usage);
    glEnableVertexAttribArray(ALOC_VERTEX);
    glVertexAttribPointer(ALOC_VERTEX, static_cast<GLint>(components), GL_FLOAT, GL_FALSE, 0, nullptr);
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

      const unsigned int components = 4; //_options & OPTION_BLEND ? 4 : 3;
      assert(_colors.size() % components == 0);

      glBindBuffer(GL_ARRAY_BUFFER, _color_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_colors.size() * sizeof(GLfloat)), _colors.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(ALOC_COLOR);
      glVertexAttribPointer(ALOC_COLOR, static_cast<GLint>(components), GL_FLOAT, GL_FALSE, 0, nullptr);
    }

  if(_options & OPTION_TEXTURE)
    {
      if(_texcoord_vbo == 0)
        glGenBuffers(1, &_texcoord_vbo);
      assert(_texcoord_vbo != 0);

      glBindBuffer(GL_ARRAY_BUFFER, _texcoord_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_texcoords.size() * sizeof(GLfloat)), _texcoords.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(ALOC_TEXCOORD);
      glVertexAttribPointer(ALOC_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

  if(_options & OPTION_NORMAL)
    {
      if(_normal_vbo == 0)
        glGenBuffers(1, &_normal_vbo);
      assert(_normal_vbo != 0);

      glBindBuffer(GL_ARRAY_BUFFER, _normal_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_normals.size() * sizeof(GLfloat)), _normals.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(ALOC_NORMAL);
      glVertexAttribPointer(ALOC_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

  if(_options & OPTION_GENERIC_VEC2_INPUT)
    {
      if(_generic_vec2_vbo == 0)
        glGenBuffers(1, &_generic_vec2_vbo);
      assert(_generic_vec2_vbo != 0);

      glBindBuffer(GL_ARRAY_BUFFER, _generic_vec2_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_generic_vec2s.size() * sizeof(GLfloat)), _generic_vec2s.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(ALOC_GENERIC_VEC2);
      glVertexAttribPointer(ALOC_GENERIC_VEC2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

  if(_options & OPTION_GENERIC_VEC3_INPUT)
    {
      if(_generic_vec3_vbo == 0)
        glGenBuffers(1, &_generic_vec3_vbo);
      assert(_generic_vec3_vbo != 0);

      glBindBuffer(GL_ARRAY_BUFFER, _generic_vec3_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(_generic_vec3s.size() * sizeof(GLfloat)), _generic_vec3s.data(), GL_STATIC_DRAW);
      glEnableVertexAttribArray(ALOC_GENERIC_VEC3);
      glVertexAttribPointer(ALOC_GENERIC_VEC3, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

  for(auto c : _children)
    c->UpdateGPU();
}


void Mesh::UpdateGPU(unsigned int update_options, unsigned int first, unsigned int count)
{
  if(update_options & OPTION_VERTEX)
    {
      assert(_vertex_vbo != 0);
      
      const unsigned int components = (_options & OPTION_VERTEX_W) ? 4 : 3;
      assert(_vertices.size() % components == 0);
      
      glBindBuffer(GL_ARRAY_BUFFER, _vertex_vbo);
      glBufferSubData(GL_ARRAY_BUFFER,
                      first * components * sizeof(GLfloat),
                      static_cast<GLsizeiptr>(count * components * sizeof(GLfloat)),
                      _vertices.data() + first * components);
    }
  // todo: Add the remaining options.
}


void Mesh::SetShaderProgram(ShaderProgram * shader_program, bool set_children)
{
  _shader_program = shader_program;

  if(set_children)
    for(auto c : _children)
      c->SetShaderProgram(shader_program, true);
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
      v = mytransform * v;
      v = v / v.w;
      _bounding_sphere_radius = std::max(_bounding_sphere_radius, static_cast<double>(glm::length(v.xyz())));
    }
}


void Mesh::SetBoundingSphereRadius(double radius)
{
  _bounding_sphere_radius = radius;
}


double Mesh::GetBoundingSphereRadius() const
{
  return _bounding_sphere_radius;
}


void Mesh::ApplyTransform(const glm::mat4 & transform)
{
  _transform *= transform;
}


void Mesh::SetTransform(const glm::mat4 & transform)
{
  _transform = transform;
}


void Mesh::SetTexture(unsigned int index, Image * texture_image, bool set_children)
{
  assert(index <= _textures.size());
  assert(texture_image);
  
  if(index == _textures.size())
    _textures.push_back(texture_image);
  else
    _textures[index] = texture_image;
  
  if(set_children)
    for(auto child : _children)
      child->SetTexture(index, texture_image, true);
}


Image * Mesh::GetTexture(unsigned int index) const
{
  assert(index < _textures.size());
  return _textures[index];
}


Mesh * Mesh::FindChild(const std::string & name)
{
  if(_name == name)
    return this;

  Mesh * rv = nullptr;
  for(auto it = _children.begin(); !rv && it != _children.end(); it++)
    rv = (*it)->FindChild(name);
  
  return rv;
}


void Mesh::AddChild(Mesh * child)
{
  assert(child);
  _children.push_back(child);
}


unsigned int Mesh::GetOptions(unsigned int interested_in) const
{
  return _options & interested_in;
}

