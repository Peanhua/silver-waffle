#include "Image.hh"
#include <cstdio>
#include <cassert>
#include <png.h>
#include <zlib.h>

bool Image::Save(const std::string & filename)
{
  bool success;
  FILE * fp;

  assert(_width > 0);
  assert(_height > 0);
  assert(_bytes_per_pixel > 0);

  success = false;
  fp = std::fopen((filename + ".png").c_str(), "wb");
  assert(fp != NULL);
  if(fp != NULL)
    {
      png_structp png;

      png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); //user_error_ptr, user_error_fn, user_warning_fn);
      assert(png);
      if(png)
        {
          png_infop info;

          info = png_create_info_struct(png);
          assert(info);
          if(info)
            {
              png_init_io(png, fp);

              png_set_compression_level(png, Z_BEST_SPEED);

              int ctype;

              if(_bytes_per_pixel == 1)
                {
                  assert(!_alpha);
                  ctype = PNG_COLOR_TYPE_GRAY;
                }
              else
                {
                  if(_alpha)
                    {
                      assert(_bytes_per_pixel == 4);
                      ctype = PNG_COLOR_TYPE_RGB_ALPHA;
                    }
                  else
                    {
                      assert(_bytes_per_pixel == 3);
                      ctype = PNG_COLOR_TYPE_RGB;
                    }
                }

              png_set_IHDR(png, info, _width, _height, 8, ctype,
                           PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

              png_bytepp row_pointers;

              row_pointers = static_cast<png_bytepp>(png_malloc(png, _height * sizeof(png_bytep)));
              assert(row_pointers != NULL);
              if(row_pointers != NULL)
                {
                  for(unsigned int i = 0; i < _height; i++)
                    row_pointers[i] = _data + (_height - i - 1) * _width * _bytes_per_pixel;
                  png_set_rows(png, info, row_pointers);
                              
                  png_write_png(png, info, 0, NULL);
                  png_write_end(png, info);
                              
                  png_free(png, row_pointers);

                  png_destroy_write_struct(&png, &info);
                }
            }
          else
            png_destroy_write_struct(&png, NULL);
        }
                    
      fclose(fp);
      success = true;
    }

  return success;
}
