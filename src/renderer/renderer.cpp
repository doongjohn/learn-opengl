#include "renderer.hpp"

Renderer::Renderer(int width, int height)
  : width(width), height(height)
{ }
Renderer::~Renderer() { }

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::SetClearColor(glm::vec3 color) const {
  glClearColor(color.x, color.y, color.z, 1.0f);
}
void Renderer::DrawTriangles(const ShaderProgram& shader, const VertexArray& vao, const IndexBuffer& ebo) const {
  vao.Bind();
  ebo.Bind();
  shader.Bind();
  glDrawElements(GL_TRIANGLES, ebo.GetCount(), GL_UNSIGNED_INT, nullptr);
}
