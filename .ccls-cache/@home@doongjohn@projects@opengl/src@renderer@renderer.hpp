#pragma once

#include "glad/glad.h"
#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"

class Renderer {
private:

public:
  void Clear() const;
  void Draw(const ShaderProgram& shader, const VertexArray& vao, const IndexBuffer& ebo) const;
};
