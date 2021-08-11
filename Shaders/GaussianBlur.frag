uniform sampler2D texture0;
uniform int in_horizontal;

in vec2 v2f_texcoord;

out vec4 final_color;

const float gaussfact[] = float[](20, 19, 16, 12, 6, 1);
const float gaussfact_total = 20 + (19 + 16 + 12 + 6 + 1) * 2;


void main()
{
  vec2 tsize = 1.0 / textureSize(texture0, 0);

  vec3 blurred = texture(texture0, v2f_texcoord).rgb * gaussfact[0] / gaussfact_total;
  if(in_horizontal != 0)
    for(int i = 1; i < gaussfact.length(); ++i)
      {
        blurred += texture(texture0, v2f_texcoord - vec2(i * tsize.x, 0.0)).rgb * gaussfact[i] / gaussfact_total;
        blurred += texture(texture0, v2f_texcoord + vec2(i * tsize.x, 0.0)).rgb * gaussfact[i] / gaussfact_total;
      }
  else
    for(int i = 1; i < gaussfact.length(); ++i)
      {
        blurred += texture(texture0, v2f_texcoord - vec2(0.0, i * tsize.y)).rgb * gaussfact[i] / gaussfact_total;
        blurred += texture(texture0, v2f_texcoord + vec2(0.0, i * tsize.y)).rgb * gaussfact[i] / gaussfact_total;
      }
  
  final_color = vec4(blurred, 1.0);
}
