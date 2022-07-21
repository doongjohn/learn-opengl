#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(const void *data, const uint32_t size) {
  glGenBuffers(1, &this->gl_id);
  glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  this->Unbind();
}
VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &this->gl_id);
  this->gl_id = 0;
}

void VertexBuffer::Bind() const {
  glBindBuffer(GL_ARRAY_BUFFER, this->gl_id);
}
void VertexBuffer::Unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
