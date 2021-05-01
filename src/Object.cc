#include "Object.hh"
#include <iostream>

Object::Object(const glm::vec3 & position)
  : _position(position),
    _mesh(nullptr)
{
}

Object::~Object()
{
  for(auto child : this->_children)
    delete child;
}

void Object::Draw(const Camera & camera, ShaderProgram & shader) const
{
  glm::mat4 model = glm::translate(glm::mat4(1.0f), this->_position);
  glm::mat4 mvp = camera.GetViewProjection() * model;
  shader.SetMatrix("in_mvp", mvp);

  if(this->_mesh)
    this->_mesh->Draw();
  
  for(auto child : this->_children)
    child->Draw(camera, shader);
}

void Object::Tick(double deltatime)
{
  for(auto child : this->_children)
    child->Tick(deltatime);
}

void Object::AddChild(Object * child)
{
  this->_children.push_back(child);
}

const glm::vec3 & Object::GetPosition() const
{
  return this->_position;
}


void Object::SetMesh(Mesh * mesh)
{
  this->_mesh = mesh;
}
