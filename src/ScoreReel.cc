#include "ScoreReel.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>


ScoreReel::ScoreReel(unsigned int drum_count)
  : _drum_count(drum_count),
    _drum_width(0.6)
{
  auto nums = AssetLoader->LoadMesh("Numbers");
  std::vector<Mesh *> numbers;
  for(char i = '0'; i <= '9'; i++)
    {
      auto n = nums->FindChild(std::string("Number") + i);
      assert(n);
      n->SetShaderProgram(AssetLoader->LoadShaderProgram("Flat"));
      numbers.push_back(n);
    }
  
  
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      _drums.push_back(CreateDrum(numbers));
      _drums_angles.push_back(0);
      _drums_target_angles.push_back(0);
    }

  
  _background = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_ELEMENT);
  _background->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
  std::vector<glm::vec3> vertices {
    glm::vec3(_drum_count * _drum_width, 0,  1.1), 
    glm::vec3(_drum_count * _drum_width, 0, -1.1), 
    glm::vec3(0,                         0, -1.1),
    glm::vec3(0,                         0,  1.1),
  };
  
  std::vector<unsigned int> indices {
    0, 3, 1,
    1, 3, 2
  };
  
  for(auto v : vertices)
    {
      _background->AddVertex(v);
      _background->AddColor(glm::vec3(0.64, 0.12, 0.77));
    }
  for(auto ii : indices)
    _background->AddElement(ii);
  
  _background->UpdateGPU();

  SetScore(0);
}


void ScoreReel::Tick(double deltatime)
{
  for(unsigned int i = 0; i < _drums.size(); i++)
    {
      auto diff = _drums_target_angles[i] - _drums_angles[i];
      _drums_angles[i] += diff * deltatime;
    }
}


void ScoreReel::Draw() const
{
  glm::mat4 proj = glm::perspective(glm::radians(30.0), 1024.0 / 768.0, 0.001, 1000.0);
  glm::mat4 view = glm::lookAt(glm::vec3(0, -20, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  glm::mat4 model = glm::translate(glm::mat4(1), glm::vec3(0.5f * -static_cast<float>(_drum_count * _drum_width), 0, 4));
  _background->Draw(model, view, proj, proj * view * model);
  
  for(unsigned int i = 0; i < _drums.size(); i++)
    {
      glm::mat4 drummodel(model);
      drummodel = glm::translate(drummodel, glm::vec3(_drum_width * i, 0, 0));
      drummodel = glm::rotate(drummodel, glm::radians(180.0f), glm::vec3(1, 0, 0));
      drummodel = glm::rotate(drummodel, glm::radians(static_cast<float>(_drums_angles[i])), glm::vec3(1, 0, 0));

      _drums[i]->Draw(drummodel, view, proj, proj * view * drummodel);
    }
}


void ScoreReel::SetScore(unsigned int score)
{
  _score = score;
  for(unsigned int i = 0; i < _drum_count; i++)
    {
      const auto drumscore = score % 10;
      score /= 10;
      
      const auto drumind = _drum_count - i - 1;
      const double adjustment = 0.75;
      _drums_target_angles[drumind] = 360.0 / 10.0 * (adjustment + static_cast<double>(10 - drumscore));
    }
}


unsigned int ScoreReel::GetScore() const
{
  return _score;
}


Mesh * ScoreReel::CreateDrum(const std::vector<Mesh *> numbers) const
{
  Mesh * drum = new Mesh(0);
  
  glm::mat4 rot(1);
  for(int i = 0; i <= 9; i++)
    {
      auto num = new Mesh(0);
      auto forward = glm::column(rot, 1);
      auto mov = glm::translate(glm::mat4(1), forward.xyz() * 1.2f);
      rot = glm::rotate(rot, glm::radians(-18.0f), glm::vec3(1, 0, 0));
      num->ApplyTransform(mov * rot);
      num->AddChild(numbers[i]);
      drum->AddChild(num);
      rot = glm::rotate(rot, glm::radians(18.0f + 36.0f), glm::vec3(1, 0, 0));
    }

  return drum;
}

