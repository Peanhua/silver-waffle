#include "WidgetSpaceshipStatus.hh"
#include "ObjectSpaceship.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"


WidgetSpaceshipStatus::WidgetSpaceshipStatus(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size),
    _spaceship(nullptr)
{
  {
    _meter = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_COLOR | Mesh::OPTION_BLEND);
    assert(_meter);
    _meter->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));

    std::vector<glm::vec3> vertices {
      glm::vec3(1, 1, 0), 
      glm::vec3(1, 0, 0), 
      glm::vec3(0, 0, 0),
      glm::vec3(0, 1, 0),
    };
  
    for(auto v : vertices)
      {
        _meter->AddVertex(glm::vec3(v.xy(), 0));
        _meter->AddColor(glm::vec3(0, 1, 0));
      }

    std::vector<unsigned int> indices {
      0, 1, 3,
      1, 2, 3
    };
    for(auto i : indices)
      _meter->AddElement(i);
  }


  {
    auto borders = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_BLEND);
    assert(borders);
    borders->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
    borders->SetPrimitiveType(GL_LINES);
  
    std::vector<glm::vec3> vertices {
      glm::vec3(0, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(1, 0, 0),
      glm::vec3(1, 1, 0),
      glm::vec3(1, 1, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(0, 1, 0),
      glm::vec3(0, 0, 0),
    };
    for(auto v : vertices)
      {
        borders->AddVertex(glm::vec3(v.xy(), v.z));
        borders->AddColor(glm::vec3(0, 0.5, 1));
      }
  
    _meter->AddChild(borders);
  }
  
  _meter->UpdateGPU();
}


void WidgetSpaceshipStatus::Draw() const
{
  auto model = GetModel();
  model = glm::scale(model, glm::vec3(GetSize().x, _spaceship->GetHealth(), 1));
  _meter->Draw(model, GetView(), GetProjection(), GetProjection() * GetView() * model);
  
  Widget::Draw();
}


void WidgetSpaceshipStatus::SetSpaceship(ObjectSpaceship * spaceship)
{
  _spaceship = spaceship;
}


ObjectSpaceship * WidgetSpaceshipStatus::GetSpaceship() const
{
  return _spaceship;
}

