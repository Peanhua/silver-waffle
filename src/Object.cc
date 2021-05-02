#include "Object.hh"
#include <iostream>

Object::Object(const glm::vec3 & position)
  : _position(position),
    _mesh(nullptr)
{
}

Object::~Object()
{
  for(auto child : _children)
    delete child;
}

void Object::Draw(const glm::mat4 & mvp) const
{
  const glm::mat4 mymvp(mvp * glm::translate(glm::mat4(1.0f), _position));

  if(_mesh)
    _mesh->Draw(mymvp);
  
  for(auto child : _children)
    child->Draw(mymvp);
}

void Object::Tick(double deltatime)
{
  for(auto child : _children)
    child->Tick(deltatime);
}

void Object::AddChild(Object * child)
{
  _children.push_back(child);
}


void Object::SetPosition(const glm::vec3 & position)
{
  _position = position;
}


const glm::vec3 & Object::GetPosition() const
{
  return _position;
}


void Object::SetMesh(Mesh * mesh)
{
  _mesh = mesh;
}
