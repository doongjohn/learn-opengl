#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(const void *data, const uint32_t size) {
  glGenBuffers(1, &this->gl_handle);
  if (this->gl_handle == 0) {
    std::cout << "Error: failed to create a buffer!\n";
    exit(1);
  }

  glBindBuffer(GL_ARRAY_BUFFER, this->gl_handle);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  VertexBuffer::Unbind();
}
VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &this->gl_handle);
  VertexBuffer::Unbind();
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, this->gl_handle);
}
void VertexBuffer::Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
