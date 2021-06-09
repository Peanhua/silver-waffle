#version 330 core

in vec4 diffuse_color;
out vec4 final_colour;

void main()
{
  final_colour = diffuse_color;
}
