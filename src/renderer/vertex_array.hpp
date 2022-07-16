#pragma once

#include <span>
#include <vector>

#include "glad/glad.h"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"

struct VertexArrayElement {
  uint32_t type;
  uint32_t count;
  bool normalized = false;
};

struct VertexArrayLayout {
  uint32_t single_vertex_size; // aka stride
  std::vector<VertexArrayElement> elements;
};

class VertexArray {
private:
  uint32_t gl_id;
  std::vector<VertexArrayLayout> layouts;

public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;

  void AttachIndexBuffer(IndexBuffer& ebo);
  void AttachVertexBuffer(VertexBuffer& vbo, std::initializer_list<VertexArrayElement> attribs);
};
