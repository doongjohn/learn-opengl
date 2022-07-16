#pragma once

#include <string>

#include "glad/glad.h"
#include "stb/stb_image.h"

class Texture {
private:
  uint32_t gl_id;
  std::string file_path;
  uint8_t *local_buffer;
  int width;
  int height;
  int bits_per_pixel;

public:
  Texture(const std::string& path);
  ~Texture();

  void Bind(uint64_t slot = 0) const;
  void Unbind() const;

  inline int GetWidth() const { return this->width; }
  inline int GetHeight() const { return this->height; }
};
