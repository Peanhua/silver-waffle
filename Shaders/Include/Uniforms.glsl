layout (std140) uniform Data
{
  mat4  in_view;
  mat4  in_projection;
  vec3  in_light_color;
  vec2  in_resolution;
  float in_time;
};


uniform mat4 in_mvp;
uniform mat4 in_model;
uniform vec3 in_glow;

#ifdef USE_TEXTURE
uniform sampler2D texture0;
#endif
