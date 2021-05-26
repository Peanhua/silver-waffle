#version 330 core

in FRAGMENT_DATA
{
  vec4 color;
} g2f;

out vec4 final_colour;

void main()
{
  final_colour = g2f.color;
}
