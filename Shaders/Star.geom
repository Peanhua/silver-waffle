#version 330 core

#define GEOM_IN points
#define GEOM_OUT points
#define GEOM_MAX_OUT_VERTICES 3

#include "Include/Input.geom"
#include "Include/Output.geom"
#include "Include/Uniforms.glsl"


void main()
{
  const float f = 0.01f;

  vec4 c0 = gin[0].color;
  vec4 c1 = gin[0].color;
  vec4 c2 = gin[0].color;
  c0.a *= 0.75;
  c1.a *= 0.5;
  c2.a *= 0.25;
  
  gout.color = c0;
  gl_Position = gl_in[0].gl_Position + vec4(f, 0, 0, 0);
  EmitVertex();

  gout.color = c1;
  gl_Position = gl_in[0].gl_Position + vec4(f, 0, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(0, f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(-f, 0, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(0, -f, 0, 0);
  EmitVertex();

  gout.color = c2;
  gl_Position = gl_in[0].gl_Position + vec4(f, f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(f, -f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(-f, f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(-f, -f, 0, 0);
  EmitVertex();

  EndPrimitive();
}
