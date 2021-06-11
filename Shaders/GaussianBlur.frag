#version 330 core

uniform sampler2D texture0;
  
in vec2 v2f_texcoord;

out vec4 final_color;
  
const float gaussfact[] = float[](20, 17, 13, 6, 2, 1);
const float gaussfact_total = 20 + (17 + 13 + 6 + 2 + 1) * 2;


void main()
{
  vec2 tsize = 1.0 / textureSize(texture0, 0);

  vec3 blurred = vec3(0, 0, 0);
  for(int i = 0; i < gaussfact.length(); ++i)
    {
#ifdef HORIZONTAL
      blurred += texture(texture0, v2f_texcoord - vec2(i * tsize.x, 0.0)).rgb * gaussfact[i] / gaussfact_total;
      blurred += texture(texture0, v2f_texcoord + vec2(i * tsize.x, 0.0)).rgb * gaussfact[i] / gaussfact_total;
#else
      blurred += texture(texture0, v2f_texcoord - vec2(0.0, i * tsize.y)).rgb * gaussfact[i] / gaussfact_total;
      blurred += texture(texture0, v2f_texcoord + vec2(0.0, i * tsize.y)).rgb * gaussfact[i] / gaussfact_total;
#endif
    }

  blurred *= 0.825;
  
  final_color = vec4(blurred, 1.0);
}
