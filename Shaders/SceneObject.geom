#version 330 core

#define GEOM_IN triangles
#define GEOM_OUT triangle_strip
#define GEOM_MAX_OUT_VERTICES 3

#include "Include/Input.geom"
#include "Include/Output.geom"
#include "Include/Uniforms.glsl"


void main()
{
  for(int i = 0; i < GEOM_MAX_OUT_VERTICES; i++)
    {
      int ind = i % 3;
      gout.position = gin[ind].position;
      gout.position_model = gin[ind].position_model;
      gout.normal   = gin[ind].normal;
      gout.color    = gin[ind].color;
#ifdef USE_TEXTURE
      gout.texcoord = gin[ind].texcoord;
#endif
      gl_Position = gl_in[ind].gl_Position;
      EmitVertex();
    }
  EndPrimitive();
}

