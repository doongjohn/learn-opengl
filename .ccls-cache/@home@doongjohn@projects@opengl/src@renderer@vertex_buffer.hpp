#pragma once

#include <span>

#include "glad/glad.h"

class VertexBuffer {
private:
  uint32_t gl_id;

public:
  VertexBuffer(const void *data, const uint32_t size);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};
