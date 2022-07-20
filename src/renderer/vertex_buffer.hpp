#pragma once

#include <span>
#include <iostream>

#include "glad/glad.h"

class VertexBuffer {
private:
  uint32_t gl_id;

public:
  VertexBuffer() {}
  VertexBuffer(const void *data, const uint32_t size);
  ~VertexBuffer();

  void Init(const void *data, const uint32_t size);
  void Bind() const;
  void Unbind() const;
};
