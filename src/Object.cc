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
#if 0
  glPushMatrix();
  glTranslatef(this->_position.x, this->_position.y, this->_position.z);
#else
  glm::mat4 model = glm::translate(glm::mat4(1.0f), this->_position);
  glm::mat4 mvp = camera.GetViewProjection() * model;
  shader.SetMatrix("in_mvp", mvp);
  /*
  shader.SetMatrix("u_proj", camera.GetProjection());
  shader.SetMatrix("u_view", camera.GetView());
  shader.SetMatrix("u_model", model);
  */
#endif
  if(this->_mesh)
    this->_mesh->Draw();
  
  for(auto child : this->_children)
    child->Draw(camera, shader);

#if 0
  glPopMatrix();
#endif
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
