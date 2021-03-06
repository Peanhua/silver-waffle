/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Widget.hh"
#include "Font.hh"
#include "Mesh.hh"
#include "ShaderProgram.hh"
#include "SubsystemAssetLoader.hh"
#include "SubsystemGfx.hh"
#include "SubsystemSettings.hh"
#include <iostream>


Widget::Widget(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : _parent(parent),
    _position(position),
    _size(size),
    _scale(1, 1),
    _imagemesh(nullptr),
    _image_color(1, 1, 1, 1),
    _images_current(0),
    _focused_borders_mesh(nullptr),
    _font(nullptr),
    _textmesh(nullptr),
    _textcolor(1, 1, 1),
    _textpadding(0, 0),
    _text_font_weight(0.6f),
    _visible(true),
    _focused(false),
    _focusable(true),
    _activated(false)
{
  if(parent)
    _parent->_children.push_back(this);

  OnSizeUpdated();
  UpdateMVP();
}


Widget::~Widget()
{
  for(auto c : _children)
    delete c;
  delete _textmesh;
  delete _imagemesh;
  delete _focused_borders_mesh;
}


void Widget::Destroy()
{
  DestroyChildren();
  
  assert(_parent);
  if(_parent)
    _parent->_destroyed_children.push_back(this);

  if(_on_destroy)
    _on_destroy(this);
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
    _imagemesh->Draw(glm::mat4(1), GetMVP());
  if(_textmesh)
    {
      auto shader = _textmesh->GetShaderProgram();
      shader->Use();
      shader->SetVec("in_font_color", _textcolor);
      shader->SetFloat("in_font_weight", _text_font_weight);
      glm::mat4 model(1);
      model = glm::translate(model, glm::vec3(_textpadding, 0));
      _textmesh->DrawSameShader(model, GetMVP() * model);
    }
  
  if(_focused && _focused_borders_mesh)
    _focused_borders_mesh->Draw(glm::mat4(1), GetMVP());

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


void Widget::SetOnDestroy(on_destroy_t callback)
{
  _on_destroy = callback;
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
      _imagemesh = new Mesh(Mesh::OPTION_ELEMENT | Mesh::OPTION_COLOR | Mesh::OPTION_TEXTURE | Mesh::OPTION_BLEND);
      assert(_imagemesh);
      _imagemesh->SetShaderProgram(AssetLoader->LoadShaderProgram("Generic-ColorTexture"));

      std::vector<glm::vec2> texcoords
        {
          glm::vec2(1, 0),
          glm::vec2(1, 1),
          glm::vec2(0, 1),
          glm::vec2(0, 0) 
        };
      for(auto tc : texcoords)
        {
          _imagemesh->AddTexCoord(tc);
          _imagemesh->AddColor(_image_color);
        }
      
      std::vector<unsigned int> indices
        {
          0, 1, 3,
          1, 2, 3
        };
      for(auto i : indices)
        _imagemesh->AddElement(i);
    }
  assert(_imagemesh);

  _imagemesh->SetTexture(0, image);
  OnSizeUpdated();
}


void Widget::SetImageColor(const glm::vec4 & color)
{
  _image_color = color;
  if(_imagemesh)
    {
      _imagemesh->ClearColors();
      for(int i = 0; i < 4; i++)
        _imagemesh->AddColor(_image_color);
      Graphics->QueueUpdateGPU(_imagemesh);
    }
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
  _text = text;

  if(_text.length() == 0)
    {
      delete _textmesh;
      _textmesh = nullptr;
      return;
    }
  
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
  _textmesh->Clear();
  _font->Render(_text, *_textmesh, 1);
  Graphics->QueueUpdateGPU(_textmesh);
}


void Widget::SetTextFont(Font * font)
{
  _font = font;
  SetText(_text);
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


void Widget::SetIsFocusable(bool is_focusable)
{
  _focusable = is_focusable;
  if(_focusable)
    OnSizeUpdated(); // To create _focused_borders_mesh.
}


bool Widget::GetIsFocusable() const
{
  return _focusable;
}


void Widget::SetIsFocused(bool is_focused)
{
  if(is_focused)
    assert(_focusable);
  
  _focused = is_focused;
}


bool Widget::GetIsFocused() const
{
  return _focused;
}


Widget * Widget::GetFocusedWidget()
{
  if(_focused)
    return this;

  for(auto c : _children)
    if(c)
      {
        auto w = c->GetFocusedWidget();
        if(w)
          return w;
      }
  
  return nullptr;
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
      
      Graphics->QueueUpdateGPU(_imagemesh);
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
  
  Graphics->QueueUpdateGPU(_focused_borders_mesh);
}


void Widget::SetTextColor(const glm::vec3 & color)
{
  _textcolor = color;
}


void Widget::SetTextFontWeight(float weight)
{
  _text_font_weight = 1.0f - weight;
}


void Widget::SetTextPadding(const glm::vec2 & padding)
{
  _textpadding = padding;
}


void Widget::SetTextPaddingCentered(bool horizontally, bool vertically)
{
  assert(_font);
  if(horizontally)
    _textpadding.x = (static_cast<float>(_size.x) - static_cast<float>(_font->GetWidth(_text))) / 2.0f;

  if(vertically)
    _textpadding.y = (static_cast<float>(_size.y) - static_cast<float>(_font->GetHeight())) / 2.0f;
}


void Widget::DestroyChildren()
{
  for(auto c : _children)
    if(c)
      c->Destroy();
  _children.clear();
}


void Widget::Render()
{
}


Mesh * Widget::GetImageMesh() const
{
  return _imagemesh;
}


void Widget::ClearImages()
{
  _images.clear();
}


void Widget::AddImage(Image * image)
{
  assert(image);
  _images_current = static_cast<unsigned int>(_images.size());
  _images.push_back(image);
  SetImage(image);
}


void Widget::AddImage(const std::string & name)
{
  AddImage(AssetLoader->LoadImage(name));
}

void Widget::SetCurrentImage(unsigned int index)
{
  assert(index < _images.size());
  if(_images_current == index)
    return;

  _images_current = index;
  SetImage(_images[_images_current]);
}


void Widget::NextImage()
{
  if(_images.size() == 0)
    return;
  
  _images_current++;
  if(_images_current >= _images.size())
    _images_current = 0;

  SetImage(_images[_images_current]);
}

