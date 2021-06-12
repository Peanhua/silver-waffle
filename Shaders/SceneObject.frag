#version 330 core

uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;
uniform vec3 in_light_color;
uniform vec3 in_glow;
#ifdef USE_TEXTURE
uniform sampler2D texture0;
#endif

in vec3 v2f_position;
in vec3 v2f_normal;
in vec3 v2f_color;
#ifdef USE_TEXTURE
in vec2 v2f_texcoord;
#endif

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_glow;


#include "FunctionDirectionalLight.frag"


void main()
{
  vec3 basecolor = v2f_color;
#ifdef USE_TEXTURE
  basecolor *= texture(texture0, v2f_texcoord).rgb;
#endif
  
  const vec3 light_position = vec3(0, 0, 100);
  const vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
  const float material_shininess = 16.0f;
  vec3 color = DirectionalLight(light_position,
                                ambient,
                                v2f_position,
                                normalize(v2f_normal),
                                basecolor,
                                material_shininess);

  out_color = vec4(color, 1.0);
  out_glow = vec4(in_glow, 1.0);
}
