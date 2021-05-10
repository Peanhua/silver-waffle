#version 150

in vec3 diffuse_color;
out vec4 final_colour;

void main()
{
  final_colour = vec4(diffuse_color, 1);
}
