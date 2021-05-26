#version 330 core

layout (points) in;
layout (points, max_vertices = 9) out;


in VERTEX_DATA
{
  vec4 color;
} v2g[];


out FRAGMENT_DATA
{
  vec4 color;
} g2f;


void main()
{
  const float f = 0.01f;

  vec4 c0 = v2g[0].color;
  vec4 c1 = v2g[0].color;
  vec4 c2 = v2g[0].color;
  c0.a *= 0.75;
  c1.a *= 0.5;
  c2.a *= 0.25;
  
  g2f.color = c0;
  gl_Position = gl_in[0].gl_Position + vec4(f, 0, 0, 0);
  EmitVertex();

  g2f.color = c1;
  gl_Position = gl_in[0].gl_Position + vec4(f, 0, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(0, f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(-f, 0, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(0, -f, 0, 0);
  EmitVertex();

  g2f.color = c2;
  gl_Position = gl_in[0].gl_Position + vec4(f, f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(f, -f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(-f, f, 0, 0);
  EmitVertex();
  gl_Position = gl_in[0].gl_Position + vec4(-f, -f, 0, 0);
  EmitVertex();

  EndPrimitive();
}
