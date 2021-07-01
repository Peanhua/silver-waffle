#include "PlanetLevel.hh"

PlanetLevel::PlanetLevel(Scene * scene)
  : Level(scene)
{
  _name = "Planet";

  LoadConfig("Data/Level-Planet");
}
