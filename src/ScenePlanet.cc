/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "ScenePlanet.hh"
#include "Camera.hh"
#include "ControllerPlanet.hh"
#include "Mesh.hh"
#include "ObjectPlanetAtmosphere.hh"
#include "ObjectPlanetGround.hh"
#include "ObjectSpaceship.hh"
#include "SubsystemAssetLoader.hh"


ScenePlanet::ScenePlanet()
  : Scene({2000, 0, 100}, {true, false, false}),
    _landing_sequence(true)
{
  _gravity = { 0, 0, -9.81 };

  const float groundsize = 40;
  auto ground = new ObjectPlanetGround(this, {GetPlayAreaSize().x, groundsize}, AssetLoader->LoadImage("8k_earth_daymap"));
  AddObject(ground);
  ground->SetAutoDestroyBox({1, 1, 1}, {0, 0, 0});
  ground->SetPosition({0, 0, -(GetPlayAreaSize().z / 2.0f + 0.5f)});

  auto atmosphere = new ObjectPlanetAtmosphere(this, GetPlayAreaSize().xz(), {1, 0, 0}, {0, 0, 0.5});
  atmosphere->Translate({0, groundsize / 2.0f, 0});
  AddPlanet(atmosphere);
}


void ScenePlanet::SetupPlayer()
{
  Scene::SetupPlayer();
  auto player = GetPlayer();
  player->EnableVelocity(true, false, true);
  player->SetPosition({50, 0, GetPlayAreaSize().z * 0.5f});
  player->RotateYaw(-90.0);
  for(unsigned int i = 0; i < 4; i++)
    {
      player->EnableEngine(i,     false);
      player->EnableEngine(i + 4, true);
    }
  player->SetController(new ControllerPlanet(player));
  if(_landing_sequence)
    {
      _landing_sequence = false;
      player->AddImpulse({300, 0, -10});
    }
}


void ScenePlanet::SetupSceneObject(Object * object, bool destroy_on_block)
{
  assert(destroy_on_block == destroy_on_block);
  
  const auto m = GetPlayAreaSize();
  const auto low  = glm::vec3(-m.x * 0.5f, 1, -m.z * 0.5f);
  const auto high = glm::vec3( m.x * 0.5f, 0,  m.z * 0.5f);
  object->SetAutoDestroyBox(low, high);
  
  for(unsigned int i = 0; i < 3; i++)
    object->SetOnExceedingPlayAreaLimits(i, _play_area_wraps[i] ? Object::ExceedAction::WRAP : Object::ExceedAction::STOP);

  auto d = dynamic_cast<ObjectMovable *>(object);
  if(d)
    d->EnableVelocity(true, false, true);
}


glm::vec3 ScenePlanet::GetRandomSpawnPosition()
{
  auto rand = [this]()
  {
    return _rdist(_random_generator);
  };

  float groundz = 0; // todo
  float emptytop = GetPlayAreaSize().z * 0.4f;

  glm::vec3 pos {
    GetPlayAreaSize().x * rand(),
    GetPlayAreaSize().y * rand(),
    groundz  +  rand() * (GetPlayAreaSize().z - groundz - emptytop)  -  (GetPlayAreaSize().z * 0.5f)
  };
      
  return pos;
}


void ScenePlanet::Draw(const Camera & camera) const
{
  Scene::Draw(camera);
#if 0
  // Draws the same scene on both sides so that when player is near the edge, the other side can be seen.
  // Todo: Fix to draw at most 1 times instead of 3.
  {
    int i = -1;
    auto cam2 = new Camera(camera);
    cam2->SetPosition(cam2->GetPosition() + glm::vec3(i * 100, 0, 0));
    cam2->SetTargetPosition(cam2->GetTargetPosition() + glm::vec3(i * 100, 0, 0));
    Scene::Draw(*cam2);
  }
  {
    int i = 1;
    auto cam2 = new Camera(camera);
    cam2->SetPosition(cam2->GetPosition() + glm::vec3(i * 100, 0, 0));
    cam2->SetTargetPosition(cam2->GetTargetPosition() + glm::vec3(i * 100, 0, 0));
    Scene::Draw(*cam2);
  }
#endif
}


bool ScenePlanet::AreInSameCollisionPartition(Object * a, Object * b) const
{
  auto dist = std::abs(a->GetPosition().x - b->GetPosition().x);
  auto mindist = static_cast<float>(a->GetMesh()->GetBoundingSphereRadius() + b->GetMesh()->GetBoundingSphereRadius());
  return dist < mindist;
}

