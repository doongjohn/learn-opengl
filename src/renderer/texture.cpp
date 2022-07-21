#include "texture.hpp"

Texture::Texture(const std::string& path)
 : gl_handle(0), file_path(path), local_buffer(nullptr), width(0), height(0), bits_per_pixel(0)
{
  // opengl texture [0,0] is bottom left
  // but most image formats [0,0] is top left
  stbi_set_flip_vertically_on_load(1);
  this->local_buffer = stbi_load(path.c_str(), &this->width, &this->height, &this->bits_per_pixel, 4);

  glGenTextures(1, &this->gl_handle);
  glBindTexture(GL_TEXTURE_2D, this->gl_handle);

  // these 4 params are mendatory
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER); // x (horizontal) wrapping mode
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER); // y (vertical) wrapping mode

  // send texture to opengl
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->local_buffer);

  // unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);

  // free local_buffer
  if (this->local_buffer) {
    stbi_image_free(this->local_buffer);
  }
}
Texture::~Texture() {
  glDeleteTextures(1, &this->gl_handle);
  Texture::Unbind();
}

void Texture::Bind(uint64_t slot /* = 0 */) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, this->gl_handle);
}
void Texture::Unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}
