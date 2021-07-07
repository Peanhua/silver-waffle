#ifndef SCENE_HH_
#define SCENE_HH_

#include "glm.hh"
#include <array>
#include <functional>
#include <random>

class Camera;
class Explosion;
class Mesh;
class Object;
class ObjectCollectible;
class ObjectInvader;
class ObjectMovable;
class ObjectProjectile;
class ObjectSpaceship;
class SpaceParticles;


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
  
  Scene(const glm::vec3 & play_area_size, const std::array<bool, 3> & play_area_wraps);
  virtual ~Scene();
  
  virtual void      Draw(const Camera & camera) const;
  virtual void      Tick(double deltatime);

  const glm::vec3 & GetPlayAreaSize() const;

  void              CreatePlayer();
  virtual void      SetupPlayer();
  ObjectSpaceship * GetPlayer() const;

  void              ClearPlanets();
  void              AddPlanet(Object * planet);
  Object *          GetClosestPlanet(const glm::vec3 & position) const;

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

protected:
  SpaceParticles * _particles;
  std::array<bool, 3>            _play_area_wraps;

  virtual void SetupSceneObject(Object * object, bool destroy_on_block) = 0;
  
private:
  std::mt19937_64                _random_generator;
  std::uniform_real_distribution<float> _rdist;
  glm::vec3                      _play_area_size;
  ObjectSpaceship *              _player;
  Container<ObjectInvader *>     _invaders;
  Container<ObjectProjectile *>  _projectiles;
  Container<ObjectCollectible *> _collectibles;
  Container<Object *>            _objects;
  Container<Explosion *>         _explosions;
  Container<Object *>            _planets;
  double                         _time;
  bool                           _warp_engine_starting;
  float                          _warp_throttle;
  bool                           _tutorialmessages_enabled;
  std::vector<bool>              _tutorialmessages;
};


#endif
