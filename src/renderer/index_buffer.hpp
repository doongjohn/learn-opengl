#pragma once

#include <span>

#include "glad/glad.h"

class IndexBuffer {
private:
  uint32_t gl_id;
  uint32_t count;

public:
  IndexBuffer(const std::span<uint32_t> data);
  ~IndexBuffer();

  void Bind() const;
  void Unbind() const;

  inline
  uint32_t GetCount() const {
    return this->count;
  }
};
