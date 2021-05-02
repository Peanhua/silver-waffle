#ifndef SUBSYSTEM_HH_
#define SUBSYSTEM_HH_

#include <string>

class Subsystem
{
public:
  Subsystem(const std::string & name);
  virtual ~Subsystem();

  const std::string & GetName() const;
  
  virtual bool Start() = 0;
  virtual void Stop()  = 0;
  virtual void PreTick();
  virtual void Tick(double deltatime);
  virtual void PostTick();
  virtual bool IsRunning() const;

private:
  std::string _name;
};

#endif
