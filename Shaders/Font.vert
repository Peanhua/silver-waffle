#version 330 core

uniform sampler2D texture0;
uniform mat4 in_mvp;
uniform vec3 in_font_color;

in vec3 in_vertex;
in vec2 in_texcoord;

out vec2 texcoord;

void main()
{
  texcoord = in_texcoord;
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
