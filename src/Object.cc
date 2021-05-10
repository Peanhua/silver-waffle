#include "Object.hh"
#include <iostream>

Object::Object(Scene * scene)
  : _scene(scene),
    _position(0, 0, 0),
    _mesh(nullptr),
    _health(100.0)
{
}


Object::~Object()
{
}


void Object::Draw(const glm::mat4 & mvp) const
{
  if(!IsAlive())
    return;
  
  const glm::mat4 mymvp(mvp * glm::translate(glm::mat4(1.0f), _position));

  if(_mesh)
    _mesh->Draw(mymvp);
}


void Object::Tick(double deltatime)
{
  assert(deltatime == deltatime);
}


void Object::SetPosition(const glm::vec3 & position)
{
  _position = position;
}


const glm::vec3 & Object::GetPosition() const
{
  return _position;
}


Mesh * Object::GetMesh() const
{
  return _mesh;
}


void Object::SetMesh(Mesh * mesh)
{
  _mesh = mesh;
}


bool Object::CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const
{
  auto distance = glm::distance(GetPosition(), other.GetPosition());
  if(distance > static_cast<float>(GetMesh()->GetBoundingSphereRadius() + other.GetMesh()->GetBoundingSphereRadius()))
    return false;

  out_hit_direction = glm::normalize(other.GetPosition() - GetPosition());
  return true;
}


void Object::Hit(double damage, const glm::vec3 & impulse)
{
  assert(impulse == impulse);
  _health -= damage;
}


void Object::SetHealth(double health)
{
  _health = health;
}



bool Object::IsAlive() const
{
  return _health > 0.0;
}

