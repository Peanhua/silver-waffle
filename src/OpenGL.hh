#ifndef OPENGL_HH_
#define OPENGL_HH_

#include <string>
#include <iostream>
#include <GL/glew.h>

namespace OpenGL
{
  class Singleton
  {
  public:
    Singleton();
    
  private:
  };
  

  extern Singleton * singleton;
}

#endif

