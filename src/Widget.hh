#ifndef WIDGET_HH_
#define WIDGET_HH_
/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/

#include "glm.hh"
#include <vector>
#include <functional>

class Font;
class Image;
class Mesh;


class Widget
{
public:
  typedef std::function<void(bool pressed, unsigned int button, const glm::ivec2 & position)> on_clicked_t;
  
  Widget(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size);
  virtual ~Widget();

  void Destroy();
  
  Widget *   GetWidgetAt(const glm::ivec2 & position);
  glm::ivec2 GetAbsolutePosition() const;
  const glm::ivec2 & GetPosition() const;
  const glm::ivec2 & GetSize() const;

  void    SetImage(Image * image);
  void    SetImage(const std::string & name);
  void    SetImageColor(const glm::vec4 & color);
  Image * GetImage() const;

  Font * GetFont() const;
  virtual void SetText(const std::string & text);
  void   SetTextFont(Font * font);
  void   SetTextColor(const glm::vec3 & color);
  void   SetTextFontWeight(float weight);
  void   SetTextPadding(const glm::vec2 & padding);
  void   SetTextPaddingCentered(bool horizontally, bool vertically);

  void SetScale(const glm::vec2 & scale);

  void SetIsVisible(bool is_visible);
  bool GetIsVisible() const;
  
  void SetIsFocused(bool is_focused);
  bool GetIsFocused() const;

  void SetIsFocusable(bool is_focusable);
  bool GetIsFocusable() const;

  virtual void Tick(double deltatime);
  virtual void Draw() const;

  void OnClicked(bool pressed);
  void SetOnClicked(on_clicked_t callback);

  void DestroyChildren();

protected:
  const glm::mat4 & GetMVP()        const;
  const glm::mat4 & GetProjection() const;
  const glm::mat4 & GetView()       const;
  const glm::mat4 & GetModel()      const;
  
private:
  Widget *              _parent;
  std::vector<Widget *> _children;
  std::vector<Widget *> _destroyed_children;

  glm::mat4  _projection;
  glm::mat4  _view;
  glm::mat4  _model;
  glm::mat4  _mvp;
  glm::ivec2 _position;
  glm::ivec2 _size;
  glm::vec2  _scale;

  Mesh *    _imagemesh;
  glm::vec4 _image_color;
  
  Mesh * _focused_borders_mesh;

  Font *      _font;
  std::string _text;
  Mesh *      _textmesh;
  glm::vec3   _textcolor;
  glm::vec2   _textpadding;
  float       _text_font_weight;
  
  bool _visible;
  bool _focused;
  bool _focusable;
  bool _activated;

  on_clicked_t _on_clicked;

  void UpdateMVP();
  void OnSizeUpdated();
};

#endif
