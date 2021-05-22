#include "Widget.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <iostream>


Widget::Widget(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : _parent(parent),
    _position(position),
    _size(size),
    _imagemesh(nullptr),
    _focused_borders_mesh(nullptr),
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


Widget * Widget::GetWidgetAt(const glm::ivec2 & position)
{
  Widget * rv = nullptr;
  for(auto it = _children.cbegin(); !rv && it != _children.cend(); it++)
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
    c->Tick(deltatime);
}


void Widget::Draw() const
{
  if(_imagemesh)
    _imagemesh->Draw(glm::mat4(1), GetView(), GetProjection(), GetMVP());

  if(_focused && _focused_borders_mesh)
    _focused_borders_mesh->Draw(glm::mat4(1), GetView(), GetProjection(), GetMVP());
  
  for(auto c : _children)
    c->Draw();
}


void Widget::OnClicked(bool pressed)
{
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
  auto pos = GetAbsolutePosition();
  _projection = glm::ortho(0.0, 1024.0 - 1.0, 768.0 - 1.0, 0.0, -1.0, 1.0);
  _view = glm::translate(glm::mat4(1), glm::vec3(pos.x, pos.y, 0));
  _mvp = _projection * _view;
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
      _focused_borders_mesh = new Mesh(Mesh::OPTION_COLOR | Mesh::OPTION_BLEND);
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

