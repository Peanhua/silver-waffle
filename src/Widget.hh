#ifndef WIDGET_HH_
#define WIDGET_HH_

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

  Widget *   GetWidgetAt(const glm::ivec2 & position);
  glm::ivec2 GetAbsolutePosition() const;
  const glm::ivec2 & GetPosition() const;
  const glm::ivec2 & GetSize() const;

  void    SetImage(Image * image);
  void    SetImage(const std::string & name);
  Image * GetImage() const;

  void SetText(const std::string & text);

  void SetScale(const glm::vec2 & scale);

  void SetIsVisible(bool is_visible);
  bool GetIsVisible() const;
  
  void SetIsFocused(bool is_focused);
  bool GetIsFocused() const;

  virtual void Tick(double deltatime);
  virtual void Draw() const;

  void OnClicked(bool pressed);
  void SetOnClicked(on_clicked_t callback);

protected:
  const glm::mat4 & GetMVP()        const;
  const glm::mat4 & GetProjection() const;
  const glm::mat4 & GetView()       const;
  const glm::mat4 & GetModel()      const;
  
private:
  Widget *              _parent;
  std::vector<Widget *> _children;

  glm::mat4  _projection;
  glm::mat4  _view;
  glm::mat4  _model;
  glm::mat4  _mvp;
  glm::ivec2 _position;
  glm::ivec2 _size;
  glm::vec2  _scale;

  Mesh * _imagemesh;
  Mesh * _focused_borders_mesh;
  Font * _font;
  Mesh * _textmesh;
  
  bool _visible;
  bool _focused;
  bool _activated;

  on_clicked_t _on_clicked;

  void UpdateMVP();
  void OnSizeUpdated();
};

#endif
