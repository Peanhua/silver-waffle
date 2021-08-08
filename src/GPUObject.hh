#ifndef GPUOBJECT_HH_
#define GPUOBJECT_HH_

class GPUObject
{
public:
  virtual ~GPUObject()
  {
  }
  
  virtual void UpdateGPU() = 0;
};

#endif
