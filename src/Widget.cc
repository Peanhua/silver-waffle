#include "Widget.hh"
#include "Font.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemSettings.hh"
#include <iostream>


Widget::Widget(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : _parent(parent),
    _position(position),
    _size(size),
    _scale(glm::vec3(1, 1, 1)),
    _imagemesh(nullptr),
    _focused_borders_mesh(nullptr),
    _font(nullptr),
    _textmesh(nullptr),
    _textcolor(glm::vec3(1, 1, 1)),
    _textpadding(glm::vec2(0, 0)),
    _visible(true),
    _focused(false),
    _activated(false)
{
  if(parent)
    _parent->_children.push_back(this);

  OnSizeUpdated();
  UpdateMVP();
}


Widget::~Widget()
{
}


void Widget::Destroy()
{
  assert(_parent);
  if(_parent)
    _parent->_destroyed_children.push_back(this);
}


Widget * Widget::GetWidgetAt(const glm::ivec2 & position)
{
  Widget * rv = nullptr;
  for(auto it = _children.cbegin(); !rv && it != _children.cend(); it++)
    if(*it)
      rv = (*it)->GetWidgetAt(position);

  if(!rv)
    {
      glm::ivec2 p = GetAbsolutePosition();
      if(position.x >= p.x && position.y >= p.y)
        {
          p += _size;
          if(position.x < p.x && position.y < p.y)
            rv = this;
        }
    }

  return rv;
}


glm::ivec2 Widget::GetAbsolutePosition() const
{
  glm::ivec2 rv(_position);

  if(_parent)
    rv += _parent->GetAbsolutePosition();

  return rv;
}


const glm::ivec2 & Widget::GetPosition() const
{
  return _position;
}


const glm::ivec2 & Widget::GetSize() const
{
  return _size;
}


void Widget::Tick(double deltatime)
{
  for(auto c : _children)
    if(c)
      c->Tick(deltatime);

  for(auto c : _destroyed_children)
    {
      for(unsigned int i = 0; i < _children.size(); i++)
        if(_children[i] == c)
          _children[i] = nullptr;
      delete c;
    }
  _destroyed_children.clear();
}


void Widget::Draw() const
{
  if(!_visible)
    return;
  
  if(_imagemesh)
    _imagemesh->Draw(glm::mat4(1), GetView(), GetProjection(), GetMVP());
  if(_textmesh)
    {
      auto shader = _textmesh->GetShaderProgram();
      shader->Use();
      shader->SetVec3("in_font_color", _textcolor);
      glm::mat4 model(1);
      model = glm::translate(model, glm::vec3(_textpadding, 0));
      _textmesh->Draw(model, GetView(), GetProjection(), GetMVP() * model);
    }
  
  if(_focused && _focused_borders_mesh)
    _focused_borders_mesh->Draw(glm::mat4(1), GetView(), GetProjection(), GetMVP());

  for(auto c : _children)
    if(c)
      c->Draw();
}


void Widget::OnClicked(bool pressed)
{
  if(!_visible)
    return;
  
  _activated = pressed;
  if(_on_clicked)
    _on_clicked(pressed, 0, glm::ivec2(0, 0));
}


void Widget::SetOnClicked(on_clicked_t callback)
{
  _on_clicked = callback;
}



void Widget::UpdateMVP()
{
  const double width = Settings->GetInt("screen_width");
  const double height = Settings->GetInt("screen_height");
  _projection = glm::ortho(0.0, width - 1.0, height - 1.0, 0.0, -1.0, 1.0);

  _view = glm::mat4(1);

  auto pos = GetAbsolutePosition();
  _model = glm::translate(glm::mat4(1), glm::vec3(pos.xy(), 0));
  _model = glm::scale(_model, glm::vec3(_scale.xy(), 0));
  
 _mvp = _projection * _view * _model;
}


const glm::mat4 & Widget::GetMVP() const
{
  return _mvp;
}


const glm::mat4 & Widget::GetProjection() const
{
  return _projection;
}


const glm::mat4 & Widget::GetView() const
{
  return _view;
}


const glm::mat4 & Widget::GetModel() const
{
  return _model;
}


void Widget::SetImage(const std::string & name)
{
  SetImage(AssetLoader->LoadImage(name));
}

void Widget::SetImage(Image * image)
{
  if(!_imagemesh)
    {
      _imagemesh = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
      assert(_imagemesh);
      _imagemesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Texture"));

      std::vector<glm::vec2> texcoords {
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1),
        glm::vec2(0, 0) 
      };
      for(auto tc : texcoords)
        _imagemesh->AddTexCoord(tc);
      
      std::vector<unsigned int> indices {
        0, 1, 3,
        1, 2, 3
      };
      for(auto i : indices)
        _imagemesh->AddElement(i);
    }
  assert(_imagemesh);

  _imagemesh->SetTexture(image);
  OnSizeUpdated();
}


Image * Widget::GetImage() const
{
  if(!_imagemesh)
    return nullptr;
  
  return _imagemesh->GetTexture();
}


Font * Widget::GetFont() const
{
  return _font;
}


void Widget::SetText(const std::string & text)
{
  if(!_font)
    {
      _font = AssetLoader->LoadFont(20);
      assert(_font);
    }
  if(!_textmesh)
    {
      _textmesh = new Mesh(Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
      assert(_textmesh);
      _textmesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Font"));
    }
  _text = text;
  _textmesh->Clear();
  _font->Render(text, *_textmesh, 1);
  _textmesh->UpdateGPU();
}


void Widget::SetScale(const glm::vec2 & scale)
{
  _scale = scale;
}


void Widget::SetIsVisible(bool is_visible)
{
  _visible = is_visible;
}


bool Widget::GetIsVisible() const
{
  return _visible;
}


void Widget::SetIsFocused(bool is_focused)
{
  _focused = is_focused;
}


bool Widget::GetIsFocused() const
{
  return _focused;
}


void Widget::OnSizeUpdated()
{
  if(_imagemesh)
    {
      std::vector<glm::vec3> vertices {
        glm::vec3(1, 1, 0), 
        glm::vec3(1, 0, 0), 
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
      };
      
      _imagemesh->ClearVertices();
      glm::vec2 s = GetSize();
      for(auto v : vertices)
        _imagemesh->AddVertex(glm::vec3(v.x * s.x, v.y * s.y, 0));
      
      _imagemesh->UpdateGPU();
    }

  if(!_focused_borders_mesh)
    {
      _focused_borders_mesh = new Mesh(Mesh::OPTION_COLOR);
      assert(_focused_borders_mesh);
      _focused_borders_mesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-Color"));
      _focused_borders_mesh->SetPrimitiveType(GL_LINES);

      std::vector<glm::vec3> colors {
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
        glm::vec3(1, 1, 1),
      };
      for(auto c : colors)
        _focused_borders_mesh->AddColor(c);
    }
  assert(_focused_borders_mesh);
  
  _focused_borders_mesh->ClearVertices();
      
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
  auto size = GetSize();
  for(auto v : vertices)
    _focused_borders_mesh->AddVertex(glm::vec3(v.x * static_cast<float>(size.x), v.y * static_cast<float>(size.y), v.z));
  
  _focused_borders_mesh->UpdateGPU();
}


void Widget::SetTextColor(const glm::vec3 & color)
{
  _textcolor = color;
}


void Widget::SetTextPadding(const glm::vec2 & padding)
{
  _textpadding = padding;
}


void Widget::SetTextPaddingCentered(bool horizontally, bool vertically)
{
  assert(_font);
  if(horizontally)
    _textpadding.x = (_size.x - _font->GetWidth(_text)) / 2;

  if(vertically)
    _textpadding.y = (_size.y - _font->GetHeight()) / 2;
}

