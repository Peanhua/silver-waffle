// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 330

uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;
uniform vec3 in_light_color;
uniform sampler2D texture0;

in vec3 v2f_position;
in vec3 v2f_normal;
in vec3 v2f_color;
in vec2 v2f_texcoord;

out vec4 out_color;

const vec3 sun_position = vec3(0, 0, 100);
const vec3 ambient = vec3(0.1f, 0.1f, 0.1f);
const float material_shininess = 16.0f;

void main()
{
  vec3 normal = normalize(v2f_normal);
  vec3 light_direction = normalize(sun_position - v2f_position);

  float lambertian = max(dot(light_direction, normal), 0.0f);
  float specular = 0.0f;

  if(lambertian > 0.0f)
    {
      vec3 view_direction = normalize(-v2f_position);
      vec3 half_direction = normalize(light_direction + view_direction);
      float specular_angle = max(dot(half_direction, normal), 0.0f);
      specular = pow(specular_angle, material_shininess);
    }

  vec3 basecolor = v2f_color;
#ifdef USE_TEXTURE
  basecolor *= texture(texture0, v2f_texcoord).rgb;
#endif
  
  vec3 color = ambient;
  color += basecolor * lambertian * in_light_color;
  color += basecolor * specular * in_light_color;
#if false
  const float gamma = 2.2;
  out_color = vec4(pow(color, vec3(1.0 / gamma)), 1.0);
#else
  out_color = vec4(color, 1.0);
#endif
}
