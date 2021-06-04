#version 330 core

uniform sampler2D texture0;
uniform mat4 in_mvp;
uniform vec3 in_font_color;

in vec2 texcoord;

out vec4 final_color;

vec4 FontColor();

void main()
{
  final_color = FontColor();
}


const float FONT_WEIGHT     = 0.6;

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
    color.a *= smoothstep(FONT_WEIGHT - SOFT_EDGE_WIDTH, FONT_WEIGHT, distAlphaMask);
  else
    color.a *= float(distAlphaMask >= FONT_WEIGHT);

  if(OUTLINE)
    if(distAlphaMask <= FONT_WEIGHT)
      if(distAlphaMask >= FONT_WEIGHT - OUTLINE_WIDTH)
        {
          float strength = smoothstep(FONT_WEIGHT - OUTLINE_WIDTH,
                                      FONT_WEIGHT,
                                      distAlphaMask);
          color = mix(color, OUTLINE_COLOR, strength);
        }

  return color;
}
