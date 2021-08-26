uniform sampler2D texture0;
uniform mat4 in_mvp;
uniform vec3 in_font_color;
uniform float in_font_weight;

in vec2 texcoord;

out vec4 final_color;

vec4 FontColor();

void main()
{
  if(true)
    final_color = FontColor();
  else
    {
      vec4 color = vec4(in_font_color, 1);
      final_color = color * texture2D(texture0, texcoord.st);
    }
}



const bool  OUTLINE         = false;
const vec4  OUTLINE_COLOR   = vec4(0, 1, 0, 1);
const float OUTLINE_WIDTH   = 0.1;

const bool  SOFT_EDGES      = true;
const float SOFT_EDGE_WIDTH = 0.2;

vec4 FontColor()
{
  vec4 color = vec4(in_font_color, 1);

  float distAlphaMask = texture2D(texture0, texcoord.st).r;

  if(SOFT_EDGES)
    color.a *= smoothstep(in_font_weight - SOFT_EDGE_WIDTH, in_font_weight, distAlphaMask);
  else
    color.a *= float(distAlphaMask >= in_font_weight);

  if(OUTLINE)
    if(distAlphaMask <= in_font_weight)
      if(distAlphaMask >= in_font_weight - OUTLINE_WIDTH)
        {
          float strength = smoothstep(in_font_weight - OUTLINE_WIDTH,
                                      in_font_weight,
                                      distAlphaMask);
          color = mix(color, OUTLINE_COLOR, strength);
        }

  return color;
}
