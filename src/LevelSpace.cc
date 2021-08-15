/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "LevelSpace.hh"
#include "ObjectLevelEntrance.hh"
#include "ObjectCollectible.hh"
#include "ObjectPlanet.hh"
#include "Scene.hh"
#include "SolarSystemObject.hh"
#include "SubsystemAssetLoader.hh"


#define SCALE 100.0


LevelSpace::LevelSpace(Scene * scene, SolarSystemObject * planet)
  : Level(scene),
    _solar_system_object(planet)
{
  _planet_size = planet->GetRelativeSize() * SCALE;
  _name = planet->GetName();
  _halt_without_program = true;
  
  LoadConfig(std::string(DATADIR) + "/Data/Level-" + _name);

  double len = 0.0;
  for(auto p : _program)
    if(p)
      len = std::max(len, p->GetRemainingTime());
  _planet_position_start = len;
}


void LevelSpace::Start()
{
  Level::Start();

  auto planet = dynamic_cast<ObjectPlanet *>(_solar_system_object->CreateSceneObject(_scene, SCALE));
  assert(planet);
 
  planet->SetPosition(glm::vec3(0, _planet_position_start, -_planet_size * 0.5 - 3));
  planet->SetVelocity(glm::vec3(0, -1, 0));
  
  _scene->AddPlanet(planet);


  auto ble = new ObjectLevelEntrance(_scene, _solar_system_object);
  _scene->AddObject(ble, {0, _planet_position_start + _planet_size * 0.2, 0});
  ble->SetVelocity(glm::vec3(0, -1, 0));
}
