#ifndef SCENE_HH_
#define SCENE_HH_

#include "glm.hh"
#include <functional>
#include <random>

class Camera;
class Explosion;
class Mesh;
class Object;
class ObjectCollectible;
class ObjectInvader;
class ObjectProjectile;
class ObjectSpaceship;
class WormholeWall;


class Scene
{
public:
  typedef std::function<void(Object * destroyer, Object * target)> on_destroyed_t;
  typedef std::function<void(ObjectCollectible * collectible)>     on_collectible_collected_t;

  template<typename T> class Container : public std::vector<T>
  {
  public:
    unsigned int GetNextFreeIndex()
    {
      for(unsigned int i = 0; i < std::vector<T>::size(); i++)
        {
          _pos++;
          if(_pos >= std::vector<T>::size())
            _pos = 0;

          auto obj = std::vector<T>::at(_pos);
          if(!obj || !obj->IsAlive())
            return _pos;
        }
      return static_cast<unsigned int>(std::vector<T>::size());
    }
    
  private:
    unsigned int _pos;
  };
  
  Scene();
  
  void              Draw(const Camera & camera) const;
  void              Tick(double deltatime);

  void              Initialize(double difficulty);

  const glm::vec2 & GetPlayAreaSize() const;
  ObjectSpaceship * GetPlayer() const;

  void              ClearPlanets();
  void              AddPlanet(Object * planet);
  
  ObjectInvader *   AddInvader(const glm::vec3 & position);
  void              AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);
  void              AddExplosion(const glm::vec3 & position, const glm::vec3 & velocity);
  ObjectCollectible * AddCollectible(const glm::vec3 & position, const glm::vec3 & velocity);
  void              SetOnDestroyed(on_destroyed_t callback);
  void              SetOnCollectibleCollected(on_collectible_collected_t callback);
  
private:
  std::mt19937_64                 _random_generator;
  glm::vec2                       _play_area_size;
  ObjectSpaceship *               _player;
  Container<ObjectInvader *>      _invaders;
  Container<ObjectProjectile *>   _projectiles;
  Container<ObjectCollectible *>  _collectibles;
  Container<Explosion *>          _explosions;
  on_destroyed_t                  _on_destroyed_callback;
  on_collectible_collected_t      _on_collectible_collected_callback;
  WormholeWall *                  _wall;
  std::vector<Object *>           _planets;
};


#endif
