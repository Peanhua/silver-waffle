#include "LevelPlanet.hh"
#include "SolarSystemObject.hh"


LevelPlanet::LevelPlanet(Scene * scene, SolarSystemObject * planet)
  : Level(scene)
{
  _name = planet->GetName();

  LoadConfig("Data/Level-Planet");
}
