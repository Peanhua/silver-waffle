// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 330

#include "Include/Input.vert"
#include "Include/Output.vert"
#include "Include/Uniforms.glsl"


void main()
{
  vec4 pos = in_view * in_model * vec4(in_vertex, 1);
  vout.position = pos.xyz / pos.w;
  vout.color = in_color;
  mat3 normal_matrix = mat3(transpose(inverse(in_view * in_model)));
  vout.normal = normal_matrix * in_normal;
#ifdef USE_TEXTURE
  vout.texcoord = in_texcoord;
#endif
  gl_Position = in_mvp * vec4(in_vertex, 1);
}
