#include "Object.hh"
#include "Camera.hh"
#include "CollisionShapeOBB.hh"
#include "CollisionShapeSphere.hh"
#include "Loot.hh"
#include "Mesh.hh"
#include "ObjectCollectible.hh"
#include "Scene.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"


Object::Object(Scene * scene)
  : _random_generator(0),
    _scene(scene),
    _controller(nullptr),
    _collision_shape(nullptr),
    _position(0, 0, 0),
    _orientation(1, 0, 0, 0),
    _exceed_actions{ExceedAction::DESTROY, ExceedAction::DESTROY, ExceedAction::DESTROY},
    _mesh(nullptr),
    _sleeping(false),
    _destroyed(false),
    _use_health(true),
    _health(100.0),
    _max_health(_health),
    _collision_channels(0),
    _collision_mask(0),
    _destroybox_low(1, 1, 1),
    _destroybox_high(0, 0, 0)
{
}


Object::~Object()
{
}


Object::Object(const Object & source)
  : _scene(source._scene),
    _controller(nullptr),
    _collision_shape(nullptr),
    _position(source._position),
    _orientation(source._orientation),
    _exceed_actions{source._exceed_actions[0], source._exceed_actions[1], source._exceed_actions[2]},
    _mesh(source._mesh),
    _sleeping(source._sleeping),
    _destroyed(source._destroyed),
    _use_health(source._use_health),
    _health(source._health),
    _max_health(source._max_health),
    _collision_channels(source._collision_channels),
    _collision_mask(source._collision_mask),
    _destroybox_low(source._destroybox_low),
    _destroybox_high(source._destroybox_high)
{
  auto sc = source.GetCollisionShape();
  auto scobb = dynamic_cast<CollisionShapeOBB *>(sc);
  if(scobb)
    _collision_shape = new CollisionShapeOBB(this, scobb->GetSize());
  else
    {
      auto scsphere = dynamic_cast<CollisionShapeSphere *>(sc);
      if(scsphere)
        _collision_shape = new CollisionShapeSphere(this, scsphere->GetRadius());
      else
        assert(false);
    }
}


void Object::Draw(const Camera & camera) const
{
  if(Settings->GetBool("draw_collision") && _collision_shape && _collision_shape->GetDebugMesh())
    {
      auto dmesh = _collision_shape->GetDebugMesh();
      const glm::mat4 model(glm::translate(glm::mat4(1), _position) * glm::toMat4(_orientation));
      const glm::mat4 mvp(camera.GetViewProjection() * model);
      dmesh->Draw(model, camera.GetView(), camera.GetProjection(), mvp);
    }
  else
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
  for(int i = 0; i < 3; i++)
    if(_destroybox_low[i] < _destroybox_high[i] && (_position[i] < _destroybox_low[i] || _position[i] > _destroybox_high[i]))
      switch(_exceed_actions[i])
        {
        case ExceedAction::IGNORE:
          break;
          
        case ExceedAction::STOP:
          {
            auto pos = GetPosition();
            if(pos[i] < _destroybox_low[i])
              {
                pos[i] = _destroybox_low[i];
                SetPosition(pos);
                auto d = dynamic_cast<ObjectMovable *>(this);
                if(d)
                  {
                    auto vel = d->GetVelocity();
                    if(vel[i] < 0.0f)
                      {
                        vel[i] = 0;
                        d->SetVelocity(vel);
                      }
                  }
              }
            else if(pos[i] > _destroybox_high[i])
              {
                pos[i] = _destroybox_high[i];
                SetPosition(pos);
                auto d = dynamic_cast<ObjectMovable *>(this);
                if(d)
                  {
                    auto vel = d->GetVelocity();
                    if(vel[i] > 0.0f)
                      {
                        vel[i] = 0;
                        d->SetVelocity(vel);
                      }
                  }
              }
          }
          break;
          
        case ExceedAction::WRAP:
          {
            auto pos = GetPosition();
            if(pos[i] < _destroybox_low[i])
              {
                pos[i] += _destroybox_high[i] - _destroybox_low[i];
                SetPosition(pos);
              }
            else if(pos[i] > _destroybox_high[i])
              {
                pos[i] -= _destroybox_high[i] - _destroybox_low[i];
                SetPosition(pos);
              }
          }
          break;
          
        case ExceedAction::DESTROY:
          Destroy(nullptr);
          break;
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


bool Object::CheckCollision(const Object & other, glm::vec3 & out_hit_direction) const
{
  if(!GetCollisionShape() || !other.GetCollisionShape())
    return false;
  
  if(!IsAlive() || !other.IsAlive())
    return false;

  if(!(GetCollidesWithChannels() & other.GetCollisionChannels()))
    return false;

  return GetCollisionShape()->CheckCollision(*other.GetCollisionShape(), out_hit_direction);
}


void Object::AddToCollisionChannel(CollisionChannel channel)
{
  _collision_channels |= static_cast<uint64_t>(1) << static_cast<uint64_t>(channel);
}


void Object::RemoveFromCollisionChannel(CollisionChannel channel)
{
  _collision_channels &= ~(static_cast<uint64_t>(1) << static_cast<uint64_t>(channel));
}


void Object::SetCollisionChannels(uint64_t channels)
{
  _collision_channels = channels;
}


void Object::AddCollidesWithChannel(CollisionChannel channel)
{
  _collision_mask |= static_cast<uint64_t>(1) << static_cast<uint64_t>(channel);
}


void Object::RemoveCollidesWithChannel(CollisionChannel channel)
{
  _collision_mask &= ~(static_cast<uint64_t>(1) << static_cast<uint64_t>(channel));
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
  if(!_use_health)
    return;
  
  assert(IsAlive());
  _health -= damage;

  if(!IsAlive())
    {
      if(perpetrator)
        _scene->AddExplosion(GetPosition(), impulse);
      Destroy(perpetrator);
    }
}


void Object::Destroy(Object * destroyer)
{
  if(!_destroyed)
    {
      _destroyed = true;
      OnDestroyed(destroyer);
    }
}


void Object::Revive(unsigned int health)
{
  _destroyed = false;
  SetHealth(health);
}


void Object::OnDestroyed(Object * destroyer)
{
  for(auto callback : _on_destroyed)
    callback(destroyer);
  SpawnLoot();
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


void Object::SetUseHealth(bool enable)
{
  _use_health = enable;
}


bool Object::IsAlive() const
{
  if(_destroyed)
    return false;
  
  return !_use_health || _health > 0.0;
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



void Object::SetOnExceedingPlayAreaLimits(unsigned int axis, ExceedAction action)
{
  assert(axis < 3);
  _exceed_actions[axis] = action;
}


double Object::GetVisualBoundingSphereRadius() const
{
  if(!_mesh)
    return 0;

  return _mesh->GetBoundingSphereRadius();
}


Controller * Object::GetController() const
{
  return _controller;
}


void Object::SetController(Controller * controller)
{
  _controller = controller;
}


CollisionShape * Object::GetCollisionShape() const
{
  return _collision_shape;
}


void Object::SetCollisionShape(CollisionShape * collision_shape)
{
  delete _collision_shape;
  _collision_shape = collision_shape;
  assert(!collision_shape || collision_shape->GetOwner() == this);
}


bool Object::IsSleeping() const
{
  return _sleeping;
}


void Object::SetIsSleeping(bool sleeping)
{
  _sleeping = sleeping;
}


void Object::SpawnLoot()
{
  auto rand = [this] { return static_cast<double>(_random_generator() - _random_generator.min()) / static_cast<double>(_random_generator.max()); };

  for(auto loot : _lootset)
    {
      auto item = loot->CreateLootItem(rand(), rand());
      if(item)
        {
          auto c = AssetLoader->LoadObjectCollectible(static_cast<int>(item->_type));
          assert(c);
          auto coll = new ObjectCollectible(*c);
          coll->SetBonus(item->_type, item->_bonus);
          GetScene()->AddCollectible(coll,
                                     GetPosition() + 0.5f * glm::vec3(rand() - 0.5, rand() - 0.5, 0.0),
                                     glm::vec3(0, -5, 0));

          switch(item->_type)
            {
            case ObjectCollectible::Type::SCORE_BONUS:
            case ObjectCollectible::Type::UPGRADEMATERIAL_ATTACK:
            case ObjectCollectible::Type::UPGRADEMATERIAL_DEFENSE:
            case ObjectCollectible::Type::UPGRADEMATERIAL_PHYSICAL:
            case ObjectCollectible::Type::WARP_FUEL:
              {
                auto rotangle = glm::normalize(glm::vec3(rand() * 2.0 - 1.0,
                                                         rand() * 2.0 - 1.0,
                                                         rand() * 2.0 - 1.0));
                coll->SetAngularVelocity(glm::angleAxis(glm::radians(90.0f), rotangle), 0.1 + rand() * 10.0);
              }
              break;
            case ObjectCollectible::Type::DAMAGE_MULTIPLIER:
            case ObjectCollectible::Type::SCORE_MULTIPLIER:
            case ObjectCollectible::Type::SHIELD:
            case ObjectCollectible::Type::NONE:
              break;
            }
        }
    }
}


void Object::ClearLoot()
{
  _lootset.clear();
}


void Object::AddLoot(Loot * loot)
{
  _lootset.push_back(loot);
}
