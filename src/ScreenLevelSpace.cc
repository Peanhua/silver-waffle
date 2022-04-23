/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ScreenLevelSpace.hh"
#include "Camera.hh"
#include "LevelSpace.hh"
#include "SceneSpace.hh"
#include "SubsystemAssetLoader.hh"


ScreenLevelSpace::ScreenLevelSpace(ScreenLevel * parent)
  : ScreenLevel("LevelSpace", parent)
{
  _camera = new Camera();
  _camera->SetFOV(60);
  _camera->SetClippingPlanes(0.01, 10000.0);
  _camera->SetPosition({0, -20, 14.5});
  _camera->SetFrustumCulling(false);
  
  _scene = new SceneSpace();

  Initialize();
}


void ScreenLevelSpace::SetupLevels()
{
  bool done = false;
  for(unsigned int i = 0; !done; i++)
    {
      auto sobj = AssetLoader->LoadSolarSystemObject(SolarSystemObject::Type::PLANET, i);
      if(sobj)
        _levels.push_back(new LevelSpace(_scene, sobj));
      else
        done = true;
    }

  ScreenLevel::SetupLevels();
}
