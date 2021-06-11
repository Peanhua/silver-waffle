#version 330 core

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform mat4 in_mvp;

in vec3 in_vertex;
in vec2 in_texcoord;

out vec2 v2f_texcoord;

void main()
{
  v2f_texcoord = in_texcoord;
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
