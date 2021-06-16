#include "Object.hh"
#include "Camera.hh"
#include "Mesh.hh"
#include "Scene.hh"
#include "ShaderProgram.hh"


Object::Object(Scene * scene)
  : _scene(scene),
    _position(0, 0, 0),
    _orientation(1, 0, 0, 0),
    _mesh(nullptr),
    _health(100.0),
    _max_health(_health),
    _collision_sphere_radius(-1),
    _collision_channels(0),
    _collision_mask(0),
    _destroybox_low(1, 1, 1),
    _destroybox_high(0, 0, 0)
{
}


Object::~Object()
{
}


void Object::Draw(const Camera & camera) const
{
  Draw(camera.GetView(), camera.GetProjection(), camera.GetViewProjection());
}


void Object::Draw(const glm::mat4 & view, const glm::mat4 & projection, const glm::mat4 & vp) const
{
  if(_mesh)
    {
      auto shader = _mesh->GetShaderProgram();
      shader->Use();
      shader->SetVec("in_glow", glm::vec3(0.0, 0.0, 0.0));

      const glm::mat4 model(glm::translate(glm::mat4(1), _position) * glm::toMat4(_orientation));
      const glm::mat4 mvp(vp * model);
      _mesh->Draw(model, view, projection, mvp);
    }
}


void Object::Tick(double deltatime)
{
  assert(deltatime == deltatime);
  if((_destroybox_low.x < _destroybox_high.x && (_position.x < _destroybox_low.x || _position.x > _destroybox_high.x)) ||
     (_destroybox_low.y < _destroybox_high.y && (_position.y < _destroybox_low.y || _position.y > _destroybox_high.y)) ||
     (_destroybox_low.z < _destroybox_high.z && (_position.z < _destroybox_low.z || _position.z > _destroybox_high.z))   )
    {
      Hit(nullptr, 99999, glm::vec3(0, 0, 0));
    }
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


void Object::SetCollisionSphereRadius(double radius)
{
  _collision_sphere_radius = radius;
}


double Object::GetCollisionSphereRadius() const
{
  if(_collision_sphere_radius > 0)
    return _collision_sphere_radius;
  else
    {
      assert(_mesh);
      return _mesh->GetBoundingSphereRadius();
    }
}


bool Object::CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const
{
  if(!IsAlive() || !other.IsAlive())
    return false;

  if(!(GetCollidesWithChannels() & other.GetCollisionChannels()))
    return false;
  
  auto distance = glm::distance(GetPosition(), other.GetPosition());
  if(distance > static_cast<float>(GetCollisionSphereRadius() + other.GetCollisionSphereRadius()))
    return false;

  out_hit_direction = glm::normalize(other.GetPosition() - GetPosition());
  return true;
}


void Object::AddToCollisionChannel(CollisionChannel channel)
{
  _collision_channels |= static_cast<uint64_t>(1) << static_cast<uint64_t>(channel);
}


void Object::AddCollidesWithChannel(CollisionChannel channel)
{
  _collision_mask |= static_cast<uint64_t>(1) << static_cast<uint64_t>(channel);
}


uint64_t Object::GetCollisionChannels() const
{
  return _collision_channels;
}


uint64_t Object::GetCollidesWithChannels() const
{
  return _collision_mask;
}


void Object::Hit(Object * perpetrator, double damage, const glm::vec3 & impulse)
{
  assert(IsAlive());
  _health -= damage;

  if(!IsAlive())
    {
      if(perpetrator)
        _scene->AddExplosion(GetPosition(), impulse);
      OnDestroyed(perpetrator);
    }
}


void Object::OnDestroyed(Object * destroyer)
{
  for(auto callback : _on_destroyed)
    callback(destroyer);
}


double Object::GetHealth() const
{
  return _health;
}


void Object::SetHealth(double health)
{
  _health = health;
}


double Object::GetMaxHealth() const
{
  return _max_health;
}


void Object::SetMaxHealth(double health)
{
  _max_health = health;
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
  _orientation *= glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(1, 0, 0));
}


void Object::RotateYaw(double angle)
{
  _orientation *= glm::angleAxis(glm::radians(static_cast<float>(angle)), glm::vec3(0, 0, 1));
}


void Object::Rotate(const glm::quat & rotation)
{
  _orientation *= rotation;
}


const glm::quat & Object::GetOrientation() const
{
  return _orientation;
}


void Object::SetOrientation(const glm::quat & orientation)
{
  _orientation = orientation;
}


Scene * Object::GetScene() const
{
  return _scene;
}


void Object::SetScene(Scene * scene)
{
  _scene = scene;
}


void Object::OnCollision(Object & other, const glm::vec3 & hit_direction)
{
  other.Hit(this, 50, -hit_direction);
}
 

void Object::SetAutoDestroyBox(const glm::vec3 & low, const glm::vec3 & high)
{
  _destroybox_low = low;
  _destroybox_high = high;
}


void Object::SetOnDestroyed(on_destroyed_t callback)
{
  _on_destroyed.push_back(callback);
}

