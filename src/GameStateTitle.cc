#include "GameStateTitle.hh"
#include "Camera.hh"
#include "GameStateGame.hh"
#include "Milkyway.hh"
#include "ObjectMovable.hh"
#include "SolarSystemObject.hh"
#include "SpaceParticles.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "WidgetButton.hh"
#include "WidgetTeletyper.hh"
#include <iostream>
#include <GL/glew.h>


GameStateTitle::GameStateTitle()
  : GameState()
{
  _milkyway = new Milkyway();

  _particles = new SpaceParticles(15.0, 90.0, 0);

  _camera = new Camera();
  _camera->SetFOV(90.0);
  _camera->SetClippingPlanes(0.001, 1300.0);
  _camera->UpdateProjection();
  _camera->SetPosition(glm::vec3(0.0f, -20.0f, 0.0f));


  auto startdist = 70.0;
  auto z = -11.0f;
  auto distance = 0.0;
  {
    auto sobj = AssetLoader->LoadSolarSystemObject(SolarSystemObject::Type::STAR, 0);
    assert(sobj);
    _planets.push_back(sobj->CreateSceneObject(nullptr, 0.2));
    _planets[_planets.size() - 1]->SetPosition(glm::vec3(distance, startdist, z));
    static_cast<ObjectMovable *>(_planets[_planets.size() - 1])->SetAngularVelocity(glm::angleAxis(glm::radians(1.5f), glm::vec3(0, 0, 1)), 1.5);
    distance += sobj->GetRelativeSize() * 0.2;
  }
  {
    bool done = false;
    for(unsigned int i = 0; !done; i++)
      {
        auto sobj = AssetLoader->LoadSolarSystemObject(SolarSystemObject::Type::PLANET, i);
        if(sobj)
          {
            distance += sobj->GetRelativeSize() * 1.1;
            _planets.push_back(sobj->CreateSceneObject(nullptr, 1.0));
            _planets[_planets.size() - 1]->SetPosition(glm::vec3(distance, startdist, z));
            static_cast<ObjectMovable *>(_planets[_planets.size() - 1])->SetAngularVelocity(glm::angleAxis(glm::radians(1.5f), glm::vec3(0, 0, 1)), 10.0);
            distance += sobj->GetRelativeSize() * 1.1;
          }
        else
          done = true;
      }
  }

  
  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");

  auto root = new Widget(nullptr, glm::ivec2(0, 0), glm::ivec2(width, height));
  assert(root);
  SetRootWidget(root);

  {
    int bw = 650;
    int bh = 250;
  
    auto w = new WidgetTeletyper(root, glm::ivec2((width - bw) / 2, (height - bh) / 5), glm::ivec2(bw, bh));
    w->SetImage("PanelBorders");
    w->SetImageColor(glm::vec4(0, 1, 0, 1));
    w->SetTextFont(AssetLoader->LoadFont(14));
    w->SetCharactersPerSecond(5);
    w->SetText(R"(Planet Earth, year 3052.          
  Urgent message from:
    The headquarters of the United Dystopia of the Earth
   
  Evil alien invaders are attacking the Earth.
   
  Your mission is to destroy their mothership,
  located behind the Neptune.
   
  Good luck.
)");
    w->SetTextColor(glm::vec3(0, 1, 0));
    w->SetTextPadding(glm::vec2(10, 2));
    w->SetIsFocusable(false);
  }
  
  {
    int bw = 150;
    int bh = 60;
  
    auto w = new WidgetButton(root, glm::ivec2((width - bw) / 2, height - bh * 4), glm::ivec2(bw, bh));
    w->SetTextFont(AssetLoader->LoadFont(30));
    w->SetText("Play");
    w->SetTextColor(glm::vec3(0.5, 1.0, 0.5));
    w->SetTextPaddingCentered(true, true);
    w->SetTextFontWeight(0.5f);
    w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
    {
      assert(button == button);
      assert(position == position);
      if(!pressed)
        {
          auto s = new GameStateGame(nullptr);
          s->SetupLevels();
          SetChildState(s);
        }
    });
  
    w = new WidgetButton(root, glm::ivec2((width - bw) / 2, height - bh * 2), glm::ivec2(bw, bh));
    w->SetTextFont(AssetLoader->LoadFont(30));
    w->SetText("Quit");
    w->SetTextColor(glm::vec3(1.0, 0.5, 0.5));
    w->SetTextPaddingCentered(true, true);
    w->SetTextFontWeight(0.5f);
    w->SetOnClicked([this](bool pressed, unsigned int button, const glm::ivec2 & position)
    {
      assert(button == button);
      assert(position == position);
      if(!pressed)
        Quit();
    });
  }
}


void GameStateTitle::Tick(double deltatime)
{

  _particles->Tick(deltatime);

  for(auto planet : _planets)
    planet->Tick(deltatime);
    
  
  glDisable(GL_DEPTH_TEST);
  _milkyway->Draw(*_camera);
  
  glEnable(GL_DEPTH_TEST);
  _camera->UpdateView();

  for(auto planet : _planets)
    planet->Draw(*_camera);

  _particles->Draw(*_camera);
  
  GameState::Tick(deltatime);
}
