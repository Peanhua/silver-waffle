#define GEOM_IN points
#define GEOM_OUT line_strip
#define GEOM_MAX_OUT_VERTICES 2

#include "Include/Input.geom"
#include "Include/Output.geom"
#include "Include/Uniforms.glsl"

uniform float in_particle_length;

void main()
{
  gout.color  = gin[0].color;
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();

  gout.color  = vec4(0, 0, 0, 0);
  gl_Position = gl_in[0].gl_Position + in_mvp * vec4(0, in_particle_length, 0, 0);
  EmitVertex();

  EndPrimitive();
}
