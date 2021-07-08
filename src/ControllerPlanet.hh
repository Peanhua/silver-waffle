#ifndef CONTROLLER_PLANET_HH_
#define CONTROLLER_PLANET_HH_

#include "Controller.hh"


class ControllerPlanet : public Controller
{
public:
  ControllerPlanet(ObjectSpaceship * spaceship);

  void SteerForward(bool pressed) override;
  void SteerBackward(bool pressed) override;
  void SteerLeft(bool pressed) override;
  void SteerRight(bool pressed) override;

private:
  bool _facing_right;
};

#endif
