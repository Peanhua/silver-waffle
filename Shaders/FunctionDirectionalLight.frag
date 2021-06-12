// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
vec3 DirectionalLight(vec3 light_position,
                      vec3 ambient_color,
                      vec3 position,
                      vec3 normal,
                      vec3 base_color,
                      float material_shininess)
{
  vec3 light_direction = normalize(light_position - position);

  float lambertian = max(dot(light_direction, normal), 0.0f);
  float specular = 0.0f;

  if(lambertian > 0.0f)
    {
      vec3 view_direction = normalize(-position);
      vec3 half_direction = normalize(light_direction + view_direction);
      float specular_angle = max(dot(half_direction, normal), 0.0f);
      specular = pow(specular_angle, material_shininess);
    }
  
  vec3 color = ambient_color;
  color += base_color * lambertian * in_light_color;
  color += base_color * specular * in_light_color;

  return color;
}
