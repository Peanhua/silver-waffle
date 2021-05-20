// Blinn-Phong adaptation from https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model

#version 150

in vec3 diffuse_color;
in vec3 vertex;
in vec3 normal;

out vec4 final_color;

const vec3 lightPos = vec3(-20.0, 50.0, 20.0);
const vec3 lightColor = vec3(1.0, 0.59, 0.19);
const float lightPower = 3840.0;
const vec3 ambientColor = vec3(0.1, 0.1, 0.1);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 32.0;

void main()
{
  vec3 normnormal = normalize(normal);
  vec3 lightDir = lightPos - vertex;
  float distance = length(lightDir);
  distance = distance * distance;
  lightDir = normalize(lightDir);

  float lambertian = max(dot(lightDir, normnormal), 0.0);
  float specular = 0.0;

  if(lambertian > 0.0)
    {
      vec3 viewDir = normalize(-vertex);
      vec3 halfDir = normalize(lightDir + viewDir);
      float specAngle = max(dot(halfDir, normnormal), 0.0);
      specular = pow(specAngle, shininess);
    }
  
  vec3 color =
    ambientColor +
    diffuse_color * lambertian * lightColor * lightPower / distance +
    specColor * specular * lightColor * lightPower / distance;

  final_color = vec4(color, 1.0);
}
