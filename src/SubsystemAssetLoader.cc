#include "SubsystemAssetLoader.hh"
#include <cassert>


SubsystemAssetLoader * AssetLoader = nullptr;


SubsystemAssetLoader::SubsystemAssetLoader()
  : Subsystem("AssetLoader")
{
}


bool SubsystemAssetLoader::Start()
{
  _text_assets["x.vert"] = R"(
#version 130

uniform mat4 in_mvp;

in vec3 in_vertex;
in vec3 in_color;

out vec3 diffuse_color;

void main()
{
  diffuse_color = in_color;
  gl_Position = in_mvp * vec4(in_vertex, 1.0f);
}
)";


  _text_assets["x.frag"] = R"(
#version 130

in vec3 diffuse_color;
out vec4 final_colour;

void main()
{
  final_colour = vec4(diffuse_color, 1);
}
)";

  assert(!AssetLoader);
  AssetLoader = this;

  return true;
}


void SubsystemAssetLoader::Stop()
{
  if(AssetLoader == this)
    AssetLoader = nullptr;
}


const std::string & SubsystemAssetLoader::LoadText(const std::string & filename)
{
  auto it = _text_assets.find(filename);
  if(it != _text_assets.end())
    return (*it).second;

  assert(false);
}


ShaderProgram * SubsystemAssetLoader::LoadShaderProgram(const std::string & name)
{
  auto it = _shader_programs.find(name);
  if(it != _shader_programs.end())
    return (*it).second;

  auto vs = AssetLoader->LoadText(name + ".vert");
  auto fs = AssetLoader->LoadText(name + ".frag");
  std::vector<std::string> vss({vs});
  std::vector<std::string> fss({fs});
  auto sp = new ShaderProgram(vss, fss);
  assert(sp);
  _shader_programs[name] = sp;
  return sp;
}


Mesh * SubsystemAssetLoader::LoadMesh(const std::string & name)
{
  auto it = _meshes.find(name);
  if(it != _meshes.end())
    return (*it).second;

  auto mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT);
  assert(mesh);
  if(mesh->LoadFromFile("3d-models/" + name + ".dae"))
    {
      mesh->UpdateGPU();
      mesh->CalculateBoundingSphereRadius();
      _meshes[name] = mesh;
    }

  return mesh;
}
