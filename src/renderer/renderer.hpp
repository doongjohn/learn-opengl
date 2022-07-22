#pragma once

#include "glad/glad.h"
#include "index_buffer.hpp"
#include "vertex_array.hpp"
#include "shader.hpp"

class Renderer {
private:

public:
  int width;
  int height;

  Renderer(int window_w, int window_h);
  ~Renderer();

  void Clear() const;
  void SetClearColor(glm::vec3 color) const;
  void Draw(const ShaderProgram& shader, const VertexArray& vao, const IndexBuffer& ebo) const;
};
