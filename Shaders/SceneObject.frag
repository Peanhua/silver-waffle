#version 330 core

#include "Include/Input.frag"
#include "Include/Output.frag"
#include "Include/Uniforms.glsl"

#include "Include/DirectionalLight.frag"


void main()
{
  vec3 basecolor = fin.color.rgb;
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

  float alpha = fin.color.a;
  float pos = fin.position_model.y - (40.0 - 1.0);
  if(pos > 0.0)
    {
#ifdef USE_BLEND
      alpha *= clamp(1.0 - pos * 0.1, 0, 1);
#else
      discard;
#endif
    }

#ifdef USE_ALPHA_DISTANCE20_FADE
  const float PI = 4.0 * atan(1.0);
  float point = (1.0 - fract(in_time * 0.5)) * PI * 2.0;
  float d = fin.distance_to_center - point;
  alpha *= 0.9 + 0.1 * sin(d);
#endif
  
  
  out_color = vec4(color, alpha);
  out_glow = vec4(in_glow, alpha);
}
