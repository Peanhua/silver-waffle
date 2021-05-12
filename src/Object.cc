#include "Object.hh"
#include <iostream>

Object::Object(Scene * scene)
  : _scene(scene),
    _position(0, 0, 0),
    _orientation(1, 0, 0, 0),
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
  
  if(_mesh)
    {
      const glm::mat4 mymvp(glm::translate(mvp, _position) * glm::toMat4(_orientation));
      _mesh->Draw(mymvp);
    }
}


void Object::Tick(double deltatime)
{
  assert(deltatime == deltatime);
}


void Object::SetPosition(const glm::vec3 & position)
{
  _position = position;
}


void Object::Translate(const glm::vec3 & translation)
{
  _position += translation;
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


glm::vec3 Object::GetForwardVector() const
{
  return glm::column(glm::toMat4(_orientation), 1);
}


glm::vec3 Object::GetRightVector() const
{
  return glm::column(glm::toMat4(_orientation), 0);
}


glm::vec3 Object::GetUpVector() const
{
  return glm::column(glm::toMat4(_orientation), 2);
}


void Object::RotateRoll(double angle)
{
  _orientation *= glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(0, 1, 0));
}


void Object::RotatePitch(double angle)
{
  _orientation = glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(1, 0, 0));
}


void Object::RotateYaw(double angle)
{
  _orientation = glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(0, 0, 1));
}


void Object::Rotate(const glm::quat & rotation)
{
  _orientation *= rotation;
}


const glm::quat & Object::GetOrientation() const
{
  return _orientation;
}

