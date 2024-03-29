#pragma once

#include <span>
#include <iostream>

#include "glad/glad.h"

class IndexBuffer {
private:
  uint32_t gl_handle;
  uint32_t count;

public:
  IndexBuffer() {}
  IndexBuffer(const std::span<uint32_t> data);
  ~IndexBuffer();

  void Bind() const;
  static void Unbind();

  inline
  uint32_t GetCount() const {
    return this->count;
  }
};
