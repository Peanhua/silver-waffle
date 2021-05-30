#include "ObjectCollectible.hh"
#include "SubsystemAssetLoader.hh"

ObjectCollectible::ObjectCollectible(Scene * scene, unsigned int score_bonus)
  : ObjectMovable(scene),
    _score_bonus(score_bonus)
{
  SetMesh(AssetLoader->LoadMesh("ScoreBonus"));
}


unsigned int ObjectCollectible::GetScoreBonus() const
{
  return _score_bonus;
}
