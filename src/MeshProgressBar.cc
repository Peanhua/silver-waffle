#include "MeshProgressBar.hh"
#include "SubsystemAssetLoader.hh"

MeshProgressBar::MeshProgressBar(float width, float height, const glm::vec3 & color)
  : Mesh(OPTION_ELEMENT | OPTION_COLOR),
    _width(width),
    _height(height),
    _value(0.0)
{
  SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));

  {
    std::vector<glm::vec3> vertices
      {
        glm::vec3(1, 1, 0), 
        glm::vec3(1, 0, 0), 
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
      };
  
    for(auto v : vertices)
      {
        AddVertex(v);
        AddColor(color);
      }

    std::vector<unsigned int> indices
      {
        0, 1, 3,
        1, 2, 3
      };
    for(auto i : indices)
      AddElement(i);
  }

  {
    auto borders = new Mesh(Mesh::OPTION_COLOR);
    assert(borders);
    borders->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
    borders->SetPrimitiveType(GL_LINES);
  
    std::vector<glm::vec3> vertices
      {
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
        borders->AddVertex(v);
        borders->AddColor(glm::vec3(0, 0.5, 1));
      }
  
    AddChild(borders);
  }
  UpdateGPU();
  
  SetValue(_value);
}


void MeshProgressBar::SetValue(float value)
{
  assert(value >= 0.0f);
  assert(value <= 1.0f);
  _value = value;

  bool horizontal = _width > _height;
  if(horizontal)
    {
      auto t = glm::scale(glm::mat4(1), glm::vec3(_value * _width, _height, 1));
      SetTransform(t);
    }
  else
    {
      auto t = glm::translate(glm::mat4(1), glm::vec3(0, (1.0f - _value) * _height, 0));
      t = glm::scale(t, glm::vec3(_width, _value * _height, 1));
      SetTransform(t);
    }
}


float MeshProgressBar::GetValue() const
{
  return _value;
}
