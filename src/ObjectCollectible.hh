#ifndef OBJECT_COLLECTIBLE_HH_
#define OBJECT_COLLECTIBLE_HH_

#include "ObjectMovable.hh"
#include <map>


class ObjectCollectible : public ObjectMovable
{
public:
  enum class Type
    {
      SCORE_BONUS,
      DAMAGE_MULTIPLIER,
      SCORE_MULTIPLIER,
      SHIELD,
      UPGRADEMATERIAL_ATTACK,
      UPGRADEMATERIAL_DEFENSE,
      UPGRADEMATERIAL_PHYSICAL,
      WARP_FUEL,
      SIZEOF_,
      NONE = SIZEOF_
    };
  
  ObjectCollectible(Scene * scene);

  void OnCollision(Object & other, const glm::vec3 & hit_direction) override;
  
  void   SetBonus(Type type, double amount);
  bool   HasBonus(Type type) const;
  double GetBonus(Type type) const;
  
private:
  std::map<Type, double> _bonuses;
};

#endif
