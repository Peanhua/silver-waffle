const float PI = radians(180.0f);
const float maxtime = 2.0f;

uniform mat4  in_mvp;
uniform float in_time;
uniform vec3  in_velocity;

in vec3 in_vertex;
in vec3 in_color;
in vec3 in_generic_vec3;

out vec4 diffuse_color;

void main()
{
  float a = max(0.0f, maxtime - in_time);
  float colormult = 0.75f + sin(PI * fract(100.0f * in_time));
  diffuse_color = vec4(in_color * colormult, a);

  float dist = sqrt(in_time);
  gl_Position = in_mvp * vec4(in_vertex + (in_velocity + in_generic_vec3) * dist, 1.0f);
}
