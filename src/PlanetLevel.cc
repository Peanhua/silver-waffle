#include "PlanetLevel.hh"
#include "SolarSystemObject.hh"


PlanetLevel::PlanetLevel(Scene * scene, SolarSystemObject * planet)
  : Level(scene)
{
  _name = planet->GetName();

  LoadConfig("Data/Level-Planet");
}
