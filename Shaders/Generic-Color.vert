uniform mat4 in_mvp;

in vec3 in_vertex;
in vec4 in_color;

out vec4 diffuse_color;

void main()
{
  diffuse_color = in_color;
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
