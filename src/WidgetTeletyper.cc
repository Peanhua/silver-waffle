#include "WidgetTeletyper.hh"


WidgetTeletyper::WidgetTeletyper(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size),
    _characters_per_second(3),
    _cursor_position(0),
    _timer(0)
{
}


void WidgetTeletyper::SetText(const std::string & text)
{
  _teletypertext = text;
  _cursor_position = 0;
  _timer = 0;
}


void WidgetTeletyper::Tick(double deltatime)
{
  if(_cursor_position > _teletypertext.length())
    {
      _timer += deltatime;
      return;
    }

  _timer -= deltatime;
  if(_timer > 0.0)
    return;

  _timer += 1.0 / _characters_per_second;

  if(_cursor_position < _teletypertext.length() && _teletypertext[_cursor_position] == '\n')
    _timer += 4.0 * 1.0 / _characters_per_second;
  
  _cursor_position++;
  Widget::SetText(_teletypertext.substr(0, _cursor_position - 1) + "_");

}


void WidgetTeletyper::SetCharactersPerSecond(double characters_per_second)
{
  _characters_per_second = characters_per_second;
}


double WidgetTeletyper::GetTime() const
{
  if(_cursor_position > _teletypertext.length())
    return _timer;
  else
    return - (_timer + static_cast<double>(_teletypertext.length() - _cursor_position) / _characters_per_second);
}

