#include "renderer.hpp"

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const ShaderProgram& shader, const VertexArray& vao, const IndexBuffer& ebo) const {
  shader.Bind();
  vao.Bind();
  ebo.Bind();
  glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr);
}
