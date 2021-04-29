#include "OpenGL.hh"
#include <iostream>
#include <cassert>

OpenGL::Singleton * OpenGL::singleton = nullptr;

namespace OpenGL
{
  static void GLAPIENTRY MessageCallback(GLenum source,
                                         GLenum type,
                                         GLuint id,
                                         GLenum severity,
                                         GLsizei length,
                                         const GLchar* message,
                                         const void* userParam)
  {
    assert(source == source);
    assert(id == id);
    assert(length == length);
    assert(userParam == userParam);
    
    std::cerr << "OpenGL: " << ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ) << " type = " << type << ", severity = " << severity << ", message = " << message << std::endl;

    assert(type != GL_DEBUG_TYPE_ERROR);
  }
  
  Singleton::Singleton()
  {
#ifndef NDEBUG
    if(GLEW_VERSION_4_3)
      {
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(MessageCallback, 0);
      }
#endif
  }

}
