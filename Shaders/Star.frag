#version 330 core

in FRAGMENT_DATA
{
  vec4 color;
} g2f;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_glow;

void main()
{
  out_color = g2f.color;
  out_glow  = g2f.color;
}
