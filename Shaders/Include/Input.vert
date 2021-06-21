#ifdef USE_VEC4_IN_VERTEX
in vec4 in_vertex;
#else
in vec3 in_vertex;
#endif
in vec4 in_color;
in vec3 in_normal;
#ifdef USE_TEXTURE
in vec2 in_texcoord;
#endif
#ifdef USE_GENERIC_VEC2
in vec2 in_generic_vec2;
#endif
