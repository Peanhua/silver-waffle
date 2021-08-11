#ifndef SUBSYSTEM_HH_
#define SUBSYSTEM_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include <string>

class Subsystem
{
public:
  Subsystem(const std::string & name);
  virtual ~Subsystem();

  const std::string & GetName() const;
  
  virtual bool Start() = 0;
  virtual void StopThreads();
  virtual void Stop()  = 0;
  virtual void PreTick();
  virtual void Tick(double deltatime);
  virtual void PostTick();
  virtual bool IsRunning() const;

private:
  std::string _name;
};

#endif
