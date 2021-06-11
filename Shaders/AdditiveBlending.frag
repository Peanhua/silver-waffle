#version 330 core

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform mat4 in_mvp;

in vec2 v2f_texcoord;

out vec4 final_color;

void main()
{
  vec3 a = texture(texture0, v2f_texcoord).rgb;
  vec3 b = texture(texture1, v2f_texcoord).rgb;
  final_color = vec4(a + b, 1.0);
}
