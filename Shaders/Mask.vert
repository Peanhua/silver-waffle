#version 150

uniform mat4 in_mvp;

in vec3 in_vertex;

void main()
{
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
