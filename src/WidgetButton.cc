#include "WidgetButton.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


WidgetButton::WidgetButton(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size)
{
  _mesh_up = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT | Mesh::OPTION_BLEND);
  _mesh_up->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
  std::vector<glm::vec3> vertices
    { // borders:
      glm::vec3(0, 0, 0), // 0
      glm::vec3(1, 0, 0), // 1

      glm::vec3(0.05, 0.07, 0), // 2
      glm::vec3(0.95, 0.07, 0), // 3

      glm::vec3(0.05, 0.93, 0), // 4
      glm::vec3(0.95, 0.93, 0), // 5

      glm::vec3(0, 1, 0), // 6
      glm::vec3(1, 1, 0), // 7

      // flat middle:
      glm::vec3(0.05, 0.07, 0), // 8
      glm::vec3(0.95, 0.07, 0), // 9

      glm::vec3(0.05, 0.93, 0), // 10
      glm::vec3(0.95, 0.93, 0), // 11
    };
  std::vector<glm::vec4> colors
    {
      glm::vec4(0.443, 0.631, 1.000, 1.0),
      glm::vec4(0.000, 0.212, 0.639, 1.0),

      glm::vec4(0.443, 0.631, 1.000, 1.0),
      glm::vec4(0.000, 0.212, 0.639, 1.0),
      
      glm::vec4(0.000, 0.212, 0.639, 1.0),
      glm::vec4(0.000, 0.000, 0.000, 1.0),

      glm::vec4(0.000, 0.212, 0.639, 1.0),
      glm::vec4(0.000, 0.000, 0.000, 1.0),


      glm::vec4(0.000, 0.153, 1.000, 1.0),
      glm::vec4(0.000, 0.153, 1.000, 1.0),

      glm::vec4(0.000, 0.153, 1.000, 1.0),
      glm::vec4(0.000, 0.153, 1.000, 1.0),
    };
  std::vector<unsigned int> indices
    {
      0, 2, 1,
      1, 2, 3,
      0, 6, 2,
      2, 6, 4,
      1, 3, 7,
      3, 5, 7,
      4, 7, 5,
      4, 6, 7,

      8, 10, 9,
      9, 10, 11,
    };
      
  for(auto v : vertices)
    _mesh_up->AddVertex(v);
  for(auto c : colors)
    _mesh_up->AddColor(c);
  for(auto i : indices)
    _mesh_up->AddElement(i);

  _mesh_up->UpdateGPU();
}


WidgetButton::~WidgetButton()
{
  delete _mesh_up;
}


void WidgetButton::Draw() const
{
  if(_mesh_up)
    {
      auto m = glm::scale(glm::mat4(1), glm::vec3(GetSize(), 1));
      _mesh_up->Draw(m, GetView(), GetProjection(), GetMVP() * m);
    }
  
  Widget::Draw();
}

