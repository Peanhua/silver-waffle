#version 330 core

#include "Include/Input.frag"
#include "Include/Output.frag"
#include "Include/Uniforms.glsl"

#include "Include/DirectionalLight.frag"


void main()
{
  vec3 basecolor = fin.color;
#ifdef USE_TEXTURE
  basecolor *= texture(texture0, fin.texcoord).rgb;
#endif
  
  const vec3 light_position = vec3(0, 0, 100);
  const vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
  const float material_shininess = 16.0f;
  vec3 color = DirectionalLight(light_position,
                                ambient,
                                fin.position,
                                normalize(fin.normal),
                                basecolor,
                                material_shininess);

  out_color = vec4(color, 1.0);
  out_glow = vec4(in_glow, 1.0);
}
