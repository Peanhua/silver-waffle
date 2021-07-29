DATA
{
  vec3 position;
  vec3 position_model;
  vec3 normal;
  vec4 color;
#ifdef USE_TEXTURE
  vec2 texcoord;
#endif
#ifdef USE_EMISSION
  float emission;
#endif
  float distance_to_center;
}
