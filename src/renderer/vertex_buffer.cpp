#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(const void *data, const uint32_t size) {
  glGenBuffers(1, &this->gl_handle);
  glBindBuffer(GL_ARRAY_BUFFER, this->gl_handle);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  this->Unbind();
}
VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &this->gl_handle);
  this->gl_handle = 0;
  VertexBuffer::Unbind();
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, this->gl_handle);
}
void VertexBuffer::Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
