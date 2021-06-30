#ifndef SCENE_HH_
#define SCENE_HH_

#include "glm.hh"
#include <functional>
#include <random>

class Camera;
class Explosion;
class Mesh;
class Milkyway;
class Object;
class ObjectCollectible;
class ObjectInvader;
class ObjectMovable;
class ObjectProjectile;
class ObjectSpaceship;
class SpaceParticles;
class WormholeWall;


class Scene
{
public:

  template<typename T> class Container : public std::vector<T>
  {
  public:
    // Constructor to silence Valgrind:
    Container()
      : std::vector<T>(),
        _pos(0)
    {
    }

    
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

  const glm::vec2 & GetPlayAreaSize() const;

  ObjectSpaceship * CreatePlayer();
  ObjectSpaceship * GetPlayer() const;

  void              ClearPlanets();
  void              AddPlanet(Object * planet);

  ObjectInvader *   AddInvader(const glm::vec3 & position);
  void              AddProjectile(Object * owner, const glm::vec3 & position, const glm::vec3 & velocity, double damage, double lifetime);
  void              AddExplosion(const glm::vec3 & position, const glm::vec3 & velocity);
  bool              AddCollectible(ObjectCollectible * collectible, const glm::vec3 & position, const glm::vec3 & velocity);
  bool              AddObject(Object * object, const glm::vec3 & position);

  std::vector<ObjectMovable *> * GetNearbyObjects(const glm::vec3 & position, float radius);

  void StartWarpEngine();
  void StopWarpEngine();
  bool IsWarpEngineStarting() const;

  void TutorialMessage(unsigned int id, const std::string & message);
  void EnableTutorialMessages(bool enabled);

  double GetTime() const;
  
private:
  std::mt19937_64                _random_generator;
  glm::vec2                      _play_area_size;
  ObjectSpaceship *              _player;
  Container<ObjectInvader *>     _invaders;
  Container<ObjectProjectile *>  _projectiles;
  Container<ObjectCollectible *> _collectibles;
  Container<Object *>            _objects;
  Container<Explosion *>         _explosions;
  Milkyway *                     _milkyway;
  SpaceParticles *               _particles;
  WormholeWall *                 _wall;
  Container<Object *>            _planets;
  double                         _time;
  bool                           _warp_engine_starting;
  float                          _warp_throttle;
  bool                           _tutorialmessages_enabled;
  std::vector<bool>              _tutorialmessages;
};


#endif
