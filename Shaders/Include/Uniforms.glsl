uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;
uniform vec3 in_light_color;
uniform vec3 in_glow;
uniform float in_time;
#ifdef USE_TEXTURE
uniform sampler2D texture0;
#endif
