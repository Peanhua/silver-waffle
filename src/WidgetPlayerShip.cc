#include "WidgetPlayerShip.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include "TextureRenderer.hh"


WidgetPlayerShip::WidgetPlayerShip(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size)
{
  _mesh = AssetLoader->LoadMesh("Player");
  _texture_renderer = new TextureRenderer(512, 512);
}


WidgetPlayerShip::~WidgetPlayerShip()
{
  delete _texture_renderer;
}


void WidgetPlayerShip::Tick(double deltatime)
{
  _texture_renderer->BeginRender();
  glm::mat4 proj = glm::perspective(glm::radians(60.0), 512.0 / 512.0, 0.001, 1000.0);
  glm::mat4 view = glm::lookAt(glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  glm::mat4 model = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 0, 1));
  model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0, 1, 0));
  model = glm::translate(model, glm::vec3(0.1, -0.4, 0));
  _mesh->Draw(model, view, proj, proj * view * model, AssetLoader->LoadShaderProgram("ScoreReel"));
  _texture_renderer->EndRender();

  auto image = GetImage();
  if(!image)
    {
      image = new Image(true);
      SetImage(image);
    }
  image->SetTextureId(_texture_renderer->GetTextureId());

  Widget::Tick(deltatime);
}
