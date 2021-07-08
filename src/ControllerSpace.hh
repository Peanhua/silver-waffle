#ifndef CONTROLLER_SPACE_HH_
#define CONTROLLER_SPACE_HH_

#include "Controller.hh"


class ControllerSpace : public Controller
{
public:
  using Controller::Controller;

  void SteerLeft(bool pressed) override;
  void SteerRight(bool pressed) override;
};

#endif
