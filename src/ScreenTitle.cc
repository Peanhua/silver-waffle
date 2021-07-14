#include "ScreenTitle.hh"
#include "Camera.hh"
#include "ScreenLevelMain.hh"
#include "Milkyway.hh"
#include "ObjectMovable.hh"
#include "SolarSystemObject.hh"
#include "SpaceParticles.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include "WidgetButton.hh"
#include "WidgetHighscores.hh"
#include "WidgetTeletyper.hh"
#include <iostream>
#include <GL/glew.h>


ScreenTitle::ScreenTitle()
  : Screen(),
    _time(0),
    _plot_phase(0)
{
  _milkyway = new Milkyway();

  _particles = new SpaceParticles(15.0, 90.0, 0);

  _camera = new Camera();
  _camera->SetFOV(90.0);
  _camera->SetClippingPlanes(0.001, 1300.0);
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
    int bw = 670;
    int bh = 270;
  
    auto w = new WidgetTeletyper(GetRootWidget(), glm::ivec2((width - bw) / 2, (height - bh) / 5), glm::ivec2(bw, bh));
    w->SetImage("White");
    w->SetImageColor(glm::vec4(0, 0.5, 0, 0.125));
    w->SetTextFont(AssetLoader->LoadFont(14));
    w->SetCharactersPerSecond(15);
    w->SetTextColor(glm::vec3(0, 1, 0));
    w->SetTextPadding(glm::vec2(20, 10));
    w->SetIsFocusable(false);
    _teletyper = w;
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
          auto s = new ScreenLevelMain(nullptr);
          s->SetupLevels();
          SetChild(s);
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

  {
    int bw = 150;
    int bh = 60;
  
    new WidgetHighscores(root, glm::ivec2(20, height / 2 + (height / 2 - bh) / 2), glm::ivec2(bw, bh));
  }
}


ScreenTitle::~ScreenTitle()
{
  for(auto o : _planets)
    delete o;
  delete _camera;
  delete _milkyway;
  delete _particles;
}


void ScreenTitle::Tick(double deltatime)
{
  _time += deltatime;
  TickPlot();
  
  _particles->Tick(deltatime);

  for(auto planet : _planets)
    planet->Tick(deltatime);
    
  
  glDisable(GL_DEPTH_TEST);
  _milkyway->Draw(*_camera);
  
  glEnable(GL_DEPTH_TEST);
  for(auto planet : _planets)
    planet->Draw(*_camera);

  _particles->Draw(*_camera);
  
  Screen::Tick(deltatime);
}


void ScreenTitle::TickPlot()
{
  if(_teletyper->GetTime() > 5.0)
    {
      auto plot = AssetLoader->LoadText("Data/Plot-Title-" + std::to_string(_plot_phase) + ".txt");
      _teletyper->SetText(plot.substr(0, plot.length() - 1));

      if(plot.empty())
        _plot_phase = 0;
      else
        _plot_phase++;
    }
}
