#ifndef FONT_HH_
#define FONT_HH_

#include "glm.hh"
#include <string>
#include <vector>
#include <GL/glew.h>

class Image;
class Mesh;


class Font
{
public:

  Font(const std::string & name, unsigned int font_size);
  ~Font();

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
  };

  struct configchar
  {
    unsigned int x, y;
    unsigned int width;
  };
  
  Image *                    _image;
  unsigned int               _height;
  std::vector<fontcharacter> _characters;
};

#endif
