#pragma once

#include <span>
#include <iostream>

#include "glad/glad.h"

class VertexBuffer {
private:
  uint32_t gl_handle;

public:
  VertexBuffer() {}
  VertexBuffer(const void *data, const uint32_t size);
  ~VertexBuffer();

  void Bind() const;
  static void Unbind();
};
