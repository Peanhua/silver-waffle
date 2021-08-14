#ifndef SPACESHIP_CONTROL_PROGRAM_HH_
#define SPACESHIP_CONTROL_PROGRAM_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "GLM.hh"
#include <atomic>
#include <vector>

class ObjectSpaceship;
class Path;


class SpaceshipControlProgram
{
public:
  SpaceshipControlProgram(ObjectSpaceship * spaceship);
  virtual ~SpaceshipControlProgram();
  
  SpaceshipControlProgram * Tick(double deltatime);
  SpaceshipControlProgram * GetCurrent();
  void                      SetNext(SpaceshipControlProgram * next);
  
  virtual bool IsFinished() const = 0;
  virtual void PTick(double deltatime);
  
protected:
  ObjectSpaceship *         _spaceship;
  SpaceshipControlProgram * _next;
  double                    _time;
};


class SCP_Delay : public SpaceshipControlProgram
{
public:
  SCP_Delay(ObjectSpaceship * spaceship, double time);
  bool IsFinished() const override;
private:
  double _finish_time;
};


class SCP_MoveForward : public SpaceshipControlProgram
{
public:
  SCP_MoveForward(ObjectSpaceship * spaceship, double speed, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  double _speed;
  double _finish_time;
};


class SCP_MoveRight : public SpaceshipControlProgram
{
public:
  SCP_MoveRight(ObjectSpaceship * spaceship, double speed, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  double _speed;
  double _finish_time;
};


class SCP_MoveSidewaysSin : public SpaceshipControlProgram
{
public:
  SCP_MoveSidewaysSin(ObjectSpaceship * spaceship, double speed, double timescale, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  double _speed;
  double _timescale;
  double _finish_time;
};


class SCP_MoveTowards : public SpaceshipControlProgram
{
public:
  SCP_MoveTowards(ObjectSpaceship * spaceship, const glm::vec3 & direction, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  glm::vec3 _direction;
  double    _finish_time;
};


class SCP_MoveTo : public SpaceshipControlProgram
{
public:
  SCP_MoveTo(ObjectSpaceship * spaceship, const glm::vec3 & destination, double speed, bool use_constant_speed);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  glm::vec3 _destination;
  double    _speed;
  bool      _constant_speed;
};


class SCP_Roll : public SpaceshipControlProgram
{
public:
  SCP_Roll(ObjectSpaceship * spaceship, double speed, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  double _speed;
  double _finish_time;
};


class SCP_Pitch : public SpaceshipControlProgram
{
public:
  SCP_Pitch(ObjectSpaceship * spaceship, double speed, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  double _speed;
  double _finish_time;
};


class SCP_ChanceToFire : public SpaceshipControlProgram
{
public:
  SCP_ChanceToFire(ObjectSpaceship * spaceship, double chance, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  double _chance;
  double _finish_time;
  double _cooldown;
};


class SCP_Infinitely : public SpaceshipControlProgram
{
public:
  SCP_Infinitely(ObjectSpaceship * spaceship);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
};


class SCP_WaitForPlayerDistanceMin : public SpaceshipControlProgram
{
public:
  SCP_WaitForPlayerDistanceMin(ObjectSpaceship * spaceship, float distance);
  bool IsFinished() const override;
private:
  float _distance;
};


class SCP_WaitForPlayerHorizontalDistanceMin : public SpaceshipControlProgram
{
public:
  SCP_WaitForPlayerHorizontalDistanceMin(ObjectSpaceship * spaceship, float distance);
  bool IsFinished() const override;
private:
  float _distance;
};


class SCP_FacePlayer : public SpaceshipControlProgram
{
public:
  SCP_FacePlayer(ObjectSpaceship * spaceship);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  bool _done;

  float GetFacingDiff() const;
};


class SCP_ExitCurrentLevel : public SpaceshipControlProgram
{
public:
  SCP_ExitCurrentLevel(ObjectSpaceship * spaceship);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  bool _done;
};


class SCP_PathMoveTo : public SpaceshipControlProgram
{
public:
  SCP_PathMoveTo(ObjectSpaceship * spaceship, const glm::vec3 & destination);
  ~SCP_PathMoveTo();
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  glm::vec3           _destination;
  unsigned int        _job_id;
  std::atomic<std::vector<glm::ivec2> *> _path;
  unsigned int                           _pathpos;
};


#endif
