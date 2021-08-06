uniform sampler2D texture0;

in vec4 color;
in vec2 texcoord;

out vec4 final_colour;

void main()
{
  final_colour = texture(texture0, texcoord) * color;
}
