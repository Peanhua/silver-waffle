// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 330

in vec3 v2f_position;
in vec3 v2f_normal;
in vec3 v2f_color;

out vec4 out_color;

const vec3 sun_position = vec3(0, 0, 100);
const vec3 light_color = vec3(1.0, 0.59, 0.19);
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

  vec3 color = ambient;
  color += v2f_color * lambertian * light_color;
  color += v2f_color * specular * light_color;
    
  out_color = vec4(color, 1);
}
