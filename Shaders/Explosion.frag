in vec4 diffuse_color;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_glow;

void main()
{
  out_color = diffuse_color;
  out_glow = vec4(1, 1, 1, diffuse_color.a);
}
