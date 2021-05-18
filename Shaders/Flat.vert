// Adapted from http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/

#version 150

// Input vertex data, different for all executions of this shader.
in vec3 in_vertex;
in vec3 in_color;
in vec3 in_normal;

// Output data ; will be interpolated for each fragment.
out vec3 diffuse_color;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;

// Values that stay constant for the whole mesh.
uniform mat4 in_mvp;
uniform mat4 in_view;
uniform mat4 in_model;

void main()
{
  // Values that stay constant for the whole mesh.
  vec3 LightPosition_worldspace = vec3(0, 0, 0);

  // Output position of the vertex, in clip space : in_mvp * position
  gl_Position =  in_mvp * vec4(in_vertex,1);

  diffuse_color = in_color;
	
  // Position of the vertex, in worldspace : in_model * position
  Position_worldspace = (in_model * vec4(in_vertex,1)).xyz;
	
  // Vector that goes from the vertex to the camera, in camera space.
  // In camera space, the camera is at the origin (0,0,0).
  vec3 vertexPosition_cameraspace = ( in_view * in_model * vec4(in_vertex,1)).xyz;
  EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

  // Vector that goes from the vertex to the light, in camera space. in_model is ommited because it's identity.
  vec3 LightPosition_cameraspace = ( in_view * vec4(LightPosition_worldspace,1)).xyz;
  LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
  // Normal of the the vertex, in camera space
  Normal_cameraspace = ( in_view * in_model * vec4(in_normal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
