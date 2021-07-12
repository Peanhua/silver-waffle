#ifndef CONTROLLER_HH_
#define CONTROLLER_HH_

class ObjectSpaceship;

class Controller
{
public:
  Controller(ObjectSpaceship * spaceship);
  virtual ~Controller();

  virtual void SteerForward(bool pressed);
  virtual void SteerBackward(bool pressed);
  virtual void SteerLeft(bool pressed);
  virtual void SteerRight(bool pressed);
  virtual void ActivateWeapon(bool pressed);

  ObjectSpaceship * GetOwner() const;
  
protected:
  ObjectSpaceship * _spaceship;
};

#endif
