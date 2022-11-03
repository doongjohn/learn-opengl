#pragma once

#include "scene.hpp"

namespace Scenes {

class VertexColorTriangle : public Scene {
private:
  VertexBuffer vbo;
  IndexBuffer ebo;
  VertexArray vao;
  ShaderProgram shader;
  glm::vec3 quad_pos;

public:
  VertexColorTriangle(GLFWwindow *window, Renderer& renderer, ImGuiIO& io);
  ~VertexColorTriangle();

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

inline VertexColorTriangle::VertexColorTriangle(GLFWwindow *window, Renderer &renderer, ImGuiIO &io)
  : Scene(window, renderer, io) {
  struct Vertex {
    float pos[2];
    float color[3];
  };

  const float height = sqrtf(10000 - 2500);
  auto positions = std::array {
    Vertex{ .pos = { -100.0f, -100.0f }, .color = { 255 / 255.0f, 128 / 255.0f, 128 / 255.0f } },
    Vertex{ .pos = {  100.0f, -100.0f }, .color = { 255 / 255.0f, 220 / 255.0f, 188 / 255.0f } },
    Vertex{ .pos = {  0.0f,    height }, .color = { 192 / 255.0f, 255 / 255.0f, 166 / 255.0f } },
  };

  // index buffer
  // (default winding order is counterclockwise)
  uint32_t indices[6] = {
    0, 1, 2,
  };

  new(&vbo) VertexBuffer(positions.data(), sizeof(positions));
  new(&ebo) IndexBuffer(indices);

  vao.AttachVertexBuffer(vbo, {
    { .type = GL_FLOAT, .count = 2 },
    { .type = GL_FLOAT, .count = 3 },
  });
  vao.Unbind();

  // create shader
  new(&shader) ShaderProgram("./res/shaders/vertex_color.glsl");

  // initialize model position
  quad_pos = glm::vec3(0.0f, 0.0f, 0.0f);
}
inline VertexColorTriangle::~VertexColorTriangle() { }

inline void VertexColorTriangle::OnUpdate(const float delta_time) { }

inline void VertexColorTriangle::OnRender() {
  // set mvp matrix
  glm::mat4 proj = glm::ortho(-renderer.width / 2.0f, renderer.width / 2.0f, -renderer.height / 2.0f, renderer.height / 2.0f);
  glm::mat4 model = glm::translate(glm::mat4(1.0f), quad_pos);
  glm::mat4 mvp = proj * model;

  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);

  renderer.DrawTriangles(shader, vao, ebo);
}

inline void VertexColorTriangle::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);
  ImGui::End();
}

}
