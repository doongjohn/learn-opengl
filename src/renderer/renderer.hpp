#pragma once

#include <functional>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "vertex_array.hpp"
#include "vertex_buffer.hpp"
#include "index_buffer.hpp"
#include "texture.hpp"
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
  void DrawTriangles(const ShaderProgram &shader, const VertexArray &vao, const IndexBuffer &ebo) const;
};
