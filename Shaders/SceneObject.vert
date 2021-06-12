// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 330

in vec3 in_vertex;
in vec3 in_color;
in vec3 in_normal;
#ifdef USE_TEXTURE
in vec2 in_texcoord;
#endif

out vec3 v2f_position;
out vec3 v2f_normal;
out vec3 v2f_color;
#ifdef USE_TEXTURE
out vec2 v2f_texcoord;
#endif

uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;
uniform vec3 in_light_color;
uniform vec3 in_glow;
#ifdef USE_TEXTURE
uniform sampler2D texture0;
#endif


void main()
{
  vec4 pos = in_view * in_model * vec4(in_vertex, 1);
  v2f_position = pos.xyz / pos.w;
  v2f_color = in_color;
  mat3 normal_matrix = mat3(transpose(inverse(in_view * in_model)));
  v2f_normal = normal_matrix * in_normal;
#ifdef USE_TEXTURE
  v2f_texcoord = in_texcoord;
#endif
  gl_Position = in_mvp * vec4(in_vertex, 1);
}
