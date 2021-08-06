#include "Include/Input.frag"
#include "Include/Output.frag"
#include "Include/Uniforms.glsl"


void main()
{
  out_color = fin.color;
  out_glow  = fin.color;
}
