#ifndef SPACESHIP_CONTROL_PROGRAM_HH_
#define SPACESHIP_CONTROL_PROGRAM_HH_

#include "glm.hh"
#include <random>

class ObjectSpaceship;


class SpaceshipControlProgram
{
public:
  SpaceshipControlProgram(ObjectSpaceship * spaceship);
  virtual ~SpaceshipControlProgram();
  
  SpaceshipControlProgram * Tick(double deltatime);
  void SetNext(SpaceshipControlProgram * next);

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
  SCP_ChanceToFire(ObjectSpaceship * spaceship, unsigned long random_seed, double chance, double time);
  void PTick(double deltatime) override;
  bool IsFinished() const override;
private:
  std::mt19937_64 _random_generator;
  double          _chance;
  double          _finish_time;
  double          _cooldown;
};


#endif
