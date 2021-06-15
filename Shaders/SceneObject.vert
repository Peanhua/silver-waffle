#version 330

#include "Include/Input.vert"
#include "Include/Output.vert"
#include "Include/Uniforms.glsl"


void main()
{
  vec4 pos = in_model * vec4(in_vertex, 1);
  vout.position_model = pos.xyz / pos.w;

  pos = in_view * in_model * vec4(in_vertex, 1);
  vout.position = pos.xyz / pos.w;

  float distance_to_center = length(in_vertex);
#ifdef USE_ALPHA_DISTANCE20_FADE
  vout.color = vec4(in_color.rgb,
                    in_color.a * clamp(1.0 - distance_to_center / 20.0, 0.0, 1.0));
#else
  vout.color = in_color;
#endif
  
  mat3 normal_matrix = mat3(transpose(inverse(in_view * in_model)));
  
  vout.normal = normal_matrix * in_normal;
  
#ifdef USE_TEXTURE
  vout.texcoord = in_texcoord;
#endif
  
  vout.distance_to_center = distance_to_center;
  
  gl_Position = in_mvp * vec4(in_vertex, 1);
}
