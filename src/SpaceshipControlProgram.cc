/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "SpaceshipControlProgram.hh"
#include "Level.hh"
#include "ObjectSpaceship.hh"
#include "Scene.hh"
#include "ScreenLevel.hh"
#include "SubsystemScreen.hh"


SpaceshipControlProgram::SpaceshipControlProgram(ObjectSpaceship * spaceship)
  : _spaceship(spaceship),
    _next(nullptr),
    _time(0)
{
}

  
SpaceshipControlProgram::~SpaceshipControlProgram()
{
}

  
SpaceshipControlProgram * SpaceshipControlProgram::Tick(double deltatime)
{
  _time += deltatime;
  
  if(IsFinished())
    return _next;

  PTick(deltatime);

  return this;
}


void SpaceshipControlProgram::SetNext(SpaceshipControlProgram * next)
{
  _next = next;
}


void SpaceshipControlProgram::PTick(double deltatime)
{
  assert(deltatime == deltatime);
}



SCP_MoveForward::SCP_MoveForward(ObjectSpaceship * spaceship, double speed, double time)
  : SpaceshipControlProgram(spaceship),
    _speed(speed),
    _finish_time(time)
{
}


void SCP_MoveForward::PTick(double deltatime)
{
  _spaceship->SetPosition(_spaceship->GetPosition() + _spaceship->GetForwardVector() * static_cast<float>(_speed * deltatime));
}


bool SCP_MoveForward::IsFinished() const
{
  return _time > _finish_time;
}


SCP_MoveRight::SCP_MoveRight(ObjectSpaceship * spaceship, double speed, double time)
  : SpaceshipControlProgram(spaceship),
    _speed(speed),
    _finish_time(time)
{
}


void SCP_MoveRight::PTick(double deltatime)
{
  _spaceship->SetPosition(_spaceship->GetPosition() + _spaceship->GetRightVector() * static_cast<float>(_speed * deltatime));
}


bool SCP_MoveRight::IsFinished() const
{
  return _time > _finish_time;
}


SCP_Roll::SCP_Roll(ObjectSpaceship * spaceship, double speed, double time)
  : SpaceshipControlProgram(spaceship),
    _speed(speed),
    _finish_time(time)
{
}


void SCP_Roll::PTick(double deltatime)
{
  _spaceship->RotateRoll(_speed * deltatime);
}


bool SCP_Roll::IsFinished() const
{
  return _time > _finish_time;
}


SCP_Pitch::SCP_Pitch(ObjectSpaceship * spaceship, double speed, double time)
  : SpaceshipControlProgram(spaceship),
    _speed(speed),
    _finish_time(time)
{
}


void SCP_Pitch::PTick(double deltatime)
{
  _spaceship->RotatePitch(_speed * deltatime);
}


bool SCP_Pitch::IsFinished() const
{
  return _time > _finish_time;
}


SCP_MoveTowards::SCP_MoveTowards(ObjectSpaceship * spaceship, const glm::vec3 & direction, double time)
  : SpaceshipControlProgram(spaceship),
    _direction(direction),
    _finish_time(time)
{
}


void SCP_MoveTowards::PTick(double deltatime)
{
  _spaceship->Translate(_direction * static_cast<float>(deltatime));
}


bool SCP_MoveTowards::IsFinished() const
{
  return _time > _finish_time;
}


SCP_Delay::SCP_Delay(ObjectSpaceship * spaceship, double time)
  : SpaceshipControlProgram(spaceship),
    _finish_time(time)
{
}


bool SCP_Delay::IsFinished() const
{
  return _time > _finish_time;
}



SCP_ChanceToFire::SCP_ChanceToFire(ObjectSpaceship * spaceship, double chance, double time)
  : SpaceshipControlProgram(spaceship),
    _chance(chance),
    _finish_time(time),
    _cooldown(0)
{
}

void SCP_ChanceToFire::PTick(double deltatime)
{
  _cooldown -= deltatime;
  if(_cooldown < 0.0)
    {
      _cooldown += 0.1;
      if(static_cast<double>(_spaceship->GetRand()) < _chance)
        for(unsigned int i = 0; i < _spaceship->GetWeaponCount(); i++)
          _spaceship->FireWeapon(i);
    }
}


bool SCP_ChanceToFire::IsFinished() const
{
  return _time > _finish_time;
}



SCP_MoveSidewaysSin::SCP_MoveSidewaysSin(ObjectSpaceship * spaceship, double speed, double timescale, double time)
  : SpaceshipControlProgram(spaceship),
    _speed(speed),
    _timescale(timescale),
    _finish_time(time)
{
}


void SCP_MoveSidewaysSin::PTick(double deltatime)
{
  auto v = _spaceship->GetRightVector();
  v *= std::sin(_time * _timescale) * _speed * deltatime;
  _spaceship->SetPosition(_spaceship->GetPosition() + v);
}


bool SCP_MoveSidewaysSin::IsFinished() const
{
  return _time > _finish_time;
}



SCP_Infinitely::SCP_Infinitely(ObjectSpaceship * spaceship)
  : SpaceshipControlProgram(spaceship)
{
}


void SCP_Infinitely::PTick(double deltatime)
{
  if(_next)
    {
      auto p = _next->GetCurrent();
      if(p)
        p->PTick(deltatime);
    }
}


bool SCP_Infinitely::IsFinished() const
{
  return false;
}


SpaceshipControlProgram * SpaceshipControlProgram::GetCurrent()
{
  if(!IsFinished())
    return this;
  
  if(_next)
    return _next->GetCurrent();
  
  return nullptr;
}



SCP_WaitForPlayerDistanceMin::SCP_WaitForPlayerDistanceMin(ObjectSpaceship * spaceship, float distance)
  : SpaceshipControlProgram(spaceship),
    _distance(distance)
{
}


bool SCP_WaitForPlayerDistanceMin::IsFinished() const
{
  bool rv = false;
  
  auto player = _spaceship->GetScene()->GetPlayer();
  if(player && player->IsAlive())
    rv = glm::length(player->GetPosition() - _spaceship->GetPosition()) < _distance;

  return rv;
}


SCP_WaitForPlayerHorizontalDistanceMin::SCP_WaitForPlayerHorizontalDistanceMin(ObjectSpaceship * spaceship, float distance)
  : SpaceshipControlProgram(spaceship),
    _distance(distance)
{
}


bool SCP_WaitForPlayerHorizontalDistanceMin::IsFinished() const
{
  bool rv = false;
  
  auto player = _spaceship->GetScene()->GetPlayer();
  if(player && player->IsAlive())
    rv = std::abs(player->GetPosition().x - _spaceship->GetPosition().x) < _distance;

  return rv;
}


SCP_FacePlayer::SCP_FacePlayer(ObjectSpaceship * spaceship)
  : SpaceshipControlProgram(spaceship),
    _done(false)
{
}


void SCP_FacePlayer::PTick(double deltatime)
{
  auto player = _spaceship->GetScene()->GetPlayer();
  if(player && player->IsAlive())
    {
      const double rotspeed = 45;
      auto rot = rotspeed * deltatime;
      if(player->GetPosition().x < _spaceship->GetPosition().x)
        rot *= -1.0;
      _spaceship->RotateYaw(rot);
    }
}


float SCP_FacePlayer::GetFacingDiff() const
{
  auto player = _spaceship->GetScene()->GetPlayer();
  if(!player || !player->IsAlive())
    return 0;

  float current = glm::eulerAngles(_spaceship->GetOrientation()).z;

  float target;
  if(player->GetPosition().x < _spaceship->GetPosition().x)
    target = glm::radians(90.0f);
  else
    target = glm::radians(-90.0f);
  
  return target - current;
}


bool SCP_FacePlayer::IsFinished() const
{
  return std::abs(GetFacingDiff()) < glm::radians(1.0f);
}



SCP_MoveTo::SCP_MoveTo(ObjectSpaceship * spaceship, const glm::vec3 & destination, double speed, bool use_constant_speed)
  : SpaceshipControlProgram(spaceship),
    _destination(destination),
    _speed(speed),
    _constant_speed(use_constant_speed)
{
}


void SCP_MoveTo::PTick(double deltatime)
{
  auto movement = _destination - _spaceship->GetPosition();
  if(_constant_speed)
    {
      auto distance = glm::length(movement);
      if(distance > 1.0f)
        movement = glm::normalize(movement);
    }
  auto m = static_cast<float>(_speed * deltatime);
  _spaceship->SetPosition(_spaceship->GetPosition() + movement * m);
}


bool SCP_MoveTo::IsFinished() const
{
  return glm::length(_spaceship->GetPosition() - _destination) < 0.1f;
}


SCP_ExitCurrentLevel::SCP_ExitCurrentLevel(ObjectSpaceship * spaceship)
  : SpaceshipControlProgram(spaceship),
    _done(false)
{
}


void SCP_ExitCurrentLevel::PTick(double deltatime)
{
  assert(deltatime == deltatime);
  auto screen = dynamic_cast<ScreenLevel *>(ScreenManager->GetScreen());
  assert(screen);
  screen->GetCurrentLevel()->Stop();
  _done = true;
}


bool SCP_ExitCurrentLevel::IsFinished() const
{
  return _done;
}

