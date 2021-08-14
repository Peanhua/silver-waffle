#ifndef GPUOBJECT_HH_
#define GPUOBJECT_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

class GPUObject
{
public:
  virtual ~GPUObject()
  {
  }
  
  virtual void UpdateGPU() = 0;
};

#endif
