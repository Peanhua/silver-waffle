/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "WidgetTeletyper.hh"


WidgetTeletyper::WidgetTeletyper(Widget * parent, const glm::ivec2 & position, const glm::ivec2 & size)
  : Widget(parent, position, size),
    _characters_per_second(3),
    _paused(false),
    _cursor_position(0),
    _timer(0),
    _time_to_purge_line0(-1),
    _purging_timer(0)
{
}


void WidgetTeletyper::SetText(const std::string & text)
{
  _teletypertext = text;
  _cursor_position = 0;
  _timer = 0;
  _purging_timer = 0;
}


void WidgetTeletyper::AppendText(const std::string & text)
{
  _teletypertext += text;
  _timer = 0;
}


void WidgetTeletyper::Tick(double deltatime)
{
  if(_paused)
    return;
  
  bool update_text = false;

  if(_teletypertext.length() > 0 && _time_to_purge_line0 > 0.0)
    {
      _purging_timer += deltatime;
      if(_purging_timer > _time_to_purge_line0)
        {
          auto lfpos = _teletypertext.find_first_of('\n');
          if(lfpos == _teletypertext.npos)
            _teletypertext = "";
          else
            {
              lfpos++; // Include linefeed.
              _teletypertext = _teletypertext.substr(lfpos);
              if(lfpos < _cursor_position)
                _cursor_position -= static_cast<unsigned int>(lfpos);
              else
                _cursor_position = 0;
            }
          update_text = true;

          int linecount = 0;
          for(auto c : _teletypertext)
            if(c == '\n')
              linecount++;
          if(linecount < 20)
            _purging_timer = 0;
          else
            _purging_timer -= _time_to_purge_line0 * 0.1;
        }
    }

  if(_cursor_position <= _teletypertext.length())
    {
      _timer -= deltatime;
      if(_timer < 0.0)
        {
          _timer += 1.0 / _characters_per_second;

          if(_teletypertext[_cursor_position] == '\n')
            _timer += 3.0 * 1.0 / _characters_per_second;

          if(_cursor_position > 0)
            {
              const std::string_view stopchars(".!:#");
              const std::string_view pausechars(",-/$'\"");
              auto c = _teletypertext[_cursor_position - 1];
              
              if(stopchars.find_first_of(c) != stopchars.npos)
                _timer += 6.0 * 1.0 / _characters_per_second;
              else if(pausechars.find_first_of(c) != pausechars.npos)
                _timer += 2.0 * 1.0 / _characters_per_second;
            }
          
          _cursor_position++;
          update_text = true;
        }
    }
  else
    _timer += deltatime;

  if(update_text)
    {
      if(_cursor_position > 0)
        Widget::SetText(_teletypertext.substr(0, _cursor_position - 1) + "_");
      else
        Widget::SetText("_");
    }
}


void WidgetTeletyper::SetCharactersPerSecond(double characters_per_second)
{
  _characters_per_second = characters_per_second;
}


void WidgetTeletyper::SetPurgingTime(double time_to_purge_line0)
{
  _time_to_purge_line0 = time_to_purge_line0;
}


double WidgetTeletyper::GetTime() const
{
  if(_cursor_position > _teletypertext.length())
    return _timer;
  else
    return - (_timer + static_cast<double>(_teletypertext.length() - _cursor_position) / _characters_per_second);
}


void WidgetTeletyper::SetPaused(bool paused)
{
  _paused = paused;
}
