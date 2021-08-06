/*
  Silver Waffle
  Copyright (C) 2021  Steve Joni Yrjänä <joniyrjana@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Complete license can be found in the LICENSE file.
*/
#include "Font.hh"
#include "Image.hh"
#include "Mesh.hh"
#include "SubsystemAssetLoader.hh"
#include <cassert>
#include <cerrno>
#include <fstream>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>




Font::Font(const std::string & name, unsigned int font_size)
  : _name           (name),
    _font_size      (font_size),
    _height         (0),
    _image          (nullptr),
    _image_allocated(false)
{
}


bool Font::Generate()
{
  configchar fontdata[256];

  FT_Library ft;

  _height = _font_size;
  if(FT_Init_FreeType(&ft) == 0)
    {
      FT_Face face;
 
      if(FT_New_Face(ft, _name.c_str(), 0, &face) == 0)
        {
          FT_Set_Pixel_Sizes(face, 0, _font_size);

          unsigned int x = 0;
          _image = new Image(1, _height, 1, false);
          _image_allocated = true;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wuseless-cast"
          FT_Int32 ftloadflags = FT_LOAD_RENDER | FT_LOAD_TARGET_MONO;
#pragma GCC diagnostic pop
          for(unsigned int c = '!'; c <= '~'; c++)
            if(FT_Load_Char(face, c, ftloadflags) == 0)
              { /* Update configuration. */
                if(face->glyph->bitmap.width == 0)
                  std::cerr << "Error in font: glyph '" << static_cast<char>(c) << "' (index=" << FT_Get_Char_Index(face, c) << ") rendered width is 0." << std::endl;
                assert(face->glyph->bitmap.width > 0);

                unsigned int fdpos = c - '!';
                fontdata[fdpos].x = x;
                fontdata[fdpos].y = 0;
                // todo: take advantage of face->glyph->advance.x
                        
                /* Copy the current glyph graphics. */
                Image glyph(face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, false);
                if(face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
                  glyph.CopyData(face->glyph->bitmap.buffer);
                else if(face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
                  {
                    std::vector<uint8_t> tmp;
                    tmp.resize(face->glyph->bitmap.width * face->glyph->bitmap.rows);

                    for(unsigned int yy = 0; yy < face->glyph->bitmap.rows; yy++)
                      for(unsigned int xx = 0; xx < face->glyph->bitmap.width; xx++)
                        {
                          unsigned int byt = xx / 8 + yy * static_cast<unsigned int>(face->glyph->bitmap.pitch);
                          unsigned int bit = 7 - (xx % 8);

                          if(face->glyph->bitmap.buffer[byt] & (1<<bit))
                            tmp[xx + yy * face->glyph->bitmap.width] = 0xff;
                          else
                            tmp[xx + yy * face->glyph->bitmap.width] = 0x00;
                          }
                    glyph.CopyData(tmp.data());
                  }
                else
                  assert(false);

                /* Add some padding to it. */
                const unsigned int padding = 2;
                unsigned int padding_left = padding + static_cast<unsigned int>(face->glyph->bitmap_left);
                unsigned int padding_top  = _font_size - static_cast<unsigned int>(face->glyph->bitmap_top);
                Image padded_glyph(glyph.GetWidth() + padding_left + padding, glyph.GetHeight() + padding_top + padding, 1, false);
                padded_glyph.Clear();
                padded_glyph.Blit(padding_left, padding_top, glyph);

                padded_glyph.ToSignedDistanceField(0.5, 0.0);

                /* Expand the image containing all the glyphs. */
                _image->Expand(x + padded_glyph.GetWidth(),
                               std::max(_image->GetHeight(), padded_glyph.GetHeight()));
                        
                /* Append current glyph to the image. */
                _image->Blit(x, 0, padded_glyph);
                x += padded_glyph.GetWidth();
                fontdata[fdpos].width = x - fontdata[fdpos].x - 1;

                /* Update font height. */
                if(_height < padded_glyph.GetHeight())
                  _height = padded_glyph.GetHeight();
              }
            else
              std::cout << "Failed to load character '" << c << "' from font '" << _name << "'" << std::endl;
          FT_Done_Face(face);
        }
      else
        std::cout << "Failed to open font '" << _name << "'" << std::endl;

      FT_Done_FreeType(ft);
    }

  assert(_image);
  if(_image)
    {
      unsigned int ccount = 0;
      for(int i = '!'; i <= '~'; i++)
        ccount++;

      _characters.resize(ccount);
      for(unsigned int i = '!'; i <= '~'; i++)
        {
          unsigned int c = i - '!';
                  
          auto fc = &_characters[c];
                  
          fc->width = fontdata[c].width;

          fc->texture_coordinates[0] = static_cast<float>(fontdata[c].x) / static_cast<float>(_image->GetWidth());
          fc->texture_coordinates[1] = static_cast<float>((_image->GetHeight() - fontdata[c].y - _height)) / static_cast<float>(_image->GetHeight());
          fc->texture_coordinates[2] = static_cast<float>(fontdata[c].x + fontdata[c].width) / static_cast<float>(_image->GetWidth());
          fc->texture_coordinates[3] = static_cast<float>((_image->GetHeight() - fontdata[c].y)) / static_cast<float>(_image->GetHeight());
        }
    }
      
  { /* Make sure the image width*bpp%4==0. */
    unsigned int padding = (_image->GetWidth() * _image->GetBytesPerPixel()) % 4;
    if(padding > 0)
      _image->Expand(_image->GetWidth() + 4 - padding, _image->GetHeight());
  }
      
  _image->UpdateGPU(false, true);

  _height = _image->GetHeight();

  return true;
}


Font::~Font()
{
  if(_image_allocated)
    delete _image;
  _image = nullptr;
}


void Font::Render(int x, int y, const std::string & text, Mesh & mesh) const
{
  Render(glm::vec3(x, y, 0), text, mesh, 1.0f);
}


void Font::Render(const std::string & text, Mesh & mesh, float size) const
{
  Render(glm::vec3(0, 0, 0), text, mesh, size);
}


void Font::Render(const glm::vec3 & position, const std::string & text, Mesh & mesh, float size) const
{
  assert(mesh.GetPrimitiveType() == GL_TRIANGLES);
  assert(mesh.GetShaderProgram() == AssetLoader->LoadShaderProgram("Font"));

  float height = (static_cast<float>(_height) - 0.0f) * size;
  glm::vec3 pos(position);

  for(unsigned int i = 0; i < text.length(); i++)
    {
      unsigned int ch;

      ch = static_cast<unsigned int>(text[i]);
      if(ch == ' ')
        pos.x += height / 3.0f;
      else if(ch == '\n')
        {
          pos.x = position.x;
          pos.y += static_cast<float>(GetHeight());
        }
      else if(ch >= '!' && ch <= '~')
        {
          ch -= '!';

          float width = static_cast<float>(_characters[ch].width) * size;

          mesh.AddVertexT(pos,                               glm::vec2(_characters[ch].texture_coordinates[0], _characters[ch].texture_coordinates[1]));
          mesh.AddVertexT(pos + glm::vec3(width, height, 0), glm::vec2(_characters[ch].texture_coordinates[2], _characters[ch].texture_coordinates[3]));
          mesh.AddVertexT(pos + glm::vec3(width, 0,      0), glm::vec2(_characters[ch].texture_coordinates[2], _characters[ch].texture_coordinates[1])); 
          
          mesh.AddVertexT(pos,                               glm::vec2(_characters[ch].texture_coordinates[0], _characters[ch].texture_coordinates[1]));
          mesh.AddVertexT(pos + glm::vec3(0,     height, 0), glm::vec2(_characters[ch].texture_coordinates[0], _characters[ch].texture_coordinates[3]));
          mesh.AddVertexT(pos + glm::vec3(width, height, 0), glm::vec2(_characters[ch].texture_coordinates[2], _characters[ch].texture_coordinates[3]));

          pos.x += width;
        }
    }
  mesh.SetTexture(0, _image);
}


unsigned int Font::GetWidth(const std::string & text) const
{
  unsigned int len = 0;

  for(unsigned int i = 0; i < text.length(); i++)
    if(text[i] == ' ')
      len += this->_height / 3;
    else if(text[i] >= '!' && text[i] <= '~')
      len += this->_characters[static_cast<unsigned int>(text[i] - '!')].width;

  return len;
}


unsigned int Font::GetHeight() const
{
  return _height;
}




bool Font::Load()
{
  std::string filename("Font-" + std::to_string(_font_size));
  auto config = AssetLoader->LoadJson(std::string(DATADIR) + "/Data/" + filename);
  if(!config || !config->is_object())
    return false;

  auto & conf = *config;
  assert(conf["name"].is_string());
  assert(conf["font_size"].is_number());
  assert(conf["characters"].is_array());
  assert(_name == conf["name"].string_value());
  assert(_font_size == static_cast<unsigned int>(conf["font_size"].int_value()));

  _image = AssetLoader->LoadImage(filename);
  _image_allocated = false;
  if(!_image)
    return false;

  _height = static_cast<unsigned int>(conf["height"].int_value());

  auto & chars = conf["characters"].array_items();
  _characters.resize(chars.size());
  
  unsigned int i = 0;
  for(auto c : chars)
    {
      assert(c["width"].is_number());
      assert(c["texture_coordinates"].is_array());
      auto fc = &_characters[i++];
      fc->width = static_cast<unsigned int>(c["width"].int_value());
      auto & coords = c["texture_coordinates"].array_items();
      for(unsigned int j = 0; j < 4; j++)
        {
          assert(coords[j].is_number());
          fc->texture_coordinates[j] = static_cast<float>(coords[j].number_value());
        }
    }

  _image->UpdateGPU(false, true);
  _height = _image->GetHeight();
  
  return true;
}


void Font::Save() const
{
  json11::Json config = json11::Json::object {
    { "name",       _name                        },
    { "font_size",  static_cast<int>(_font_size) },
    { "characters", _characters                  }
  };

  std::string filename("Font-" + std::to_string(_font_size));

  if(!_image->Save("Images/" + filename))
    {
      std::cerr << "Warning, failed to save font.\n";
      return;
    }
  
  std::ofstream file("Data/" + filename + ".json");
  if(!file)
    {
      std::cerr << "Warning, failed to save font.\n";
      return;
    }
  file << config.dump();
  std::cout << "Cached font data: " << filename << "\n";
}
