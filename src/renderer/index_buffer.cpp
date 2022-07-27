#include "index_buffer.hpp"

IndexBuffer::IndexBuffer(const std::span<uint32_t> data)
  : count(data.size())
{
  glGenBuffers(1, &this->gl_handle);
  if (this->gl_handle == 0) {
    std::cout << "Error: failed to create a buffer!\n";
    exit(1);
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_handle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &data.front(), GL_STATIC_DRAW);
  IndexBuffer::Unbind();
}
IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &this->gl_handle);
  IndexBuffer::Unbind();
}

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_handle);
}
void IndexBuffer::Unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
