#include "index_buffer.hpp"

IndexBuffer::IndexBuffer(const std::span<uint32_t> data)
  : count(data.size())
{
  glGenBuffers(1, &this->gl_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), &data.front(), GL_STATIC_DRAW);
  this->Unbind();
}
IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &this->gl_id);
  this->gl_id = 0;
}

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->gl_id);
}
void IndexBuffer::Unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
