#include "vertex_array.hpp"

VertexArray::VertexArray() {
  glGenVertexArrays(1, &this->gl_id);
  glBindVertexArray(this->gl_id);
}
VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &this->gl_id);
}

void VertexArray::Bind() const {
  glBindVertexArray(this->gl_id);
}
void VertexArray::Unbind() {
  glBindVertexArray(0);
}

static int32_t sizeof_gl_type(GLenum type) {
  switch (type) {
    case GL_UNSIGNED_BYTE:
      return sizeof(uint8_t);
    case GL_UNSIGNED_INT:
      return sizeof(uint32_t);
    case GL_FLOAT:
      return sizeof(float);
  }
  return 0;
}

void VertexArray::AttachIndexBuffer(IndexBuffer& ebo) {
  ebo.Bind();
}

// stride = size of single vertex
void VertexArray::AttachVertexBuffer(VertexBuffer& vbo, std::initializer_list<VertexArrayElement> attribs) {
  vbo.Bind();
  VertexArrayLayout layout;

  // caculate stride
  uint32_t stride = 0;
  for (auto a : attribs) {
    stride += a.count * sizeof_gl_type(a.type);
  }

  uint64_t layout_index = 0;
  uint64_t offset = 0;
  for (auto a : attribs) {
    glVertexAttribPointer(layout_index, a.count, a.type, (uint32_t)a.normalized, stride, (void*)offset);
    glEnableVertexAttribArray(layout_index);
    ++layout_index;
    offset += a.count * sizeof_gl_type(a.type);
    layout.elements.push_back(a);
  }

  this->layouts.push_back(layout);
}
