#ifndef FONT_HH_
#define FONT_HH_
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
#include <string>
#include <vector>
#include <GL/glew.h>
#include <json11.hpp>

class Image;
class Mesh;


class Font
{
public:

  Font(const std::string & name, unsigned int font_size);
  ~Font();

  bool         Generate();
  bool         Load();
  void         Save() const;
  
  unsigned int GetHeight() const;
  unsigned int GetWidth(const std::string & text) const;
  unsigned int GetNWidth(const std::string & text, unsigned int textlen) const;

  GLuint       GetTexture() const;
  void         Render(const std::string & text, Mesh & mesh, float size) const;
  void         Render(int x, int y, const std::string & text, Mesh & mesh) const;
  void         Render(const glm::vec3 & position, const std::string & text, Mesh & mesh, float size) const;

  

private:
  struct fontcharacter
  {
    unsigned int width;
    float        texture_coordinates[4];

    json11::Json to_json() const
    {
      return json11::Json::object {
        { "width",               static_cast<int>(width) },
        { "texture_coordinates", json11::Json::array {
            texture_coordinates[0],
            texture_coordinates[1],
            texture_coordinates[2],
            texture_coordinates[3]
          }
        }
      };
    }
  };

  struct configchar
  {
    unsigned int x, y;
    unsigned int width;
  };
  
  std::string                _name;
  unsigned int               _font_size;
  unsigned int               _height;
  Image *                    _image;
  bool                       _image_allocated; // todo: Instead of manually doing this, use smart pointer for _image.
  std::vector<fontcharacter> _characters;
};

#endif
