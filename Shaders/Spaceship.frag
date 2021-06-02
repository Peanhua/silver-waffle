#version 330 core

uniform mat4  in_mvp;
uniform float in_time;
uniform int   in_shields;

in vec4 diffuse_color;
out vec4 final_color;

void main()
{
  vec4 color = diffuse_color;

  if(in_shields != 0)
    {
      color *= vec4(0.5, 0.5, 1.0 + 0.5 * abs(sin(3.14 * 2 * in_time * 4)), 1.0);
    }
  
  final_color = color;
}
