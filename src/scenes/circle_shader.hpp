#pragma once

#include "scene.hpp"

namespace Scenes {

class CircleShader : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
  ShaderProgram shader;
  std::string shader_file;
  glm::vec3 quad_pos;

public:
  CircleShader(GLFWwindow *window, Renderer &renderer, ImGuiIO &io);
  ~CircleShader();

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

CircleShader::CircleShader(GLFWwindow *window, Renderer &renderer, ImGuiIO &io)
  : Scene(window, renderer, io) {
  this->shader_file = "./res/shaders/circle.glsl";

  struct Vertex {
    float pos[2];
    float uv[2];
  };

  auto positions = std::array {
    Vertex { .pos = { -100.0f, -100.0f }, .uv = { 0, 0 } },
    Vertex { .pos = {  100.0f, -100.0f }, .uv = { 1, 0 } },
    Vertex { .pos = {  100.0f,  100.0f }, .uv = { 1, 1 } },
    Vertex { .pos = { -100.0f,  100.0f }, .uv = { 0, 1 } },
  };

  // index buffer
  // (default winding order is counterclockwise)
  uint32_t indices[6] = {
    0, 1, 2,
    0, 2, 3,
  };

  new(&vbo) VertexBuffer(positions.data(), sizeof(positions));
  new(&ebo) IndexBuffer(indices);

  vao.AttachVertexBuffer(vbo, {
    { .type = GL_FLOAT, .count = 2 },
    { .type = GL_FLOAT, .count = 2 },
  });
  vao.Unbind();

  // create shader
  new(&shader) ShaderProgram(shader_file);

  // initialize model position
  quad_pos = glm::vec3(0.0f, 0.0f, 0.0f);
}
CircleShader::~CircleShader() { }

void CircleShader::OnUpdate(const float delta_time) { }

void CircleShader::OnRender() {
  // set mvp matrix
  glm::mat4 proj = glm::ortho(-renderer.width / 2.0f, renderer.width / 2.0f, -renderer.height / 2.0f, renderer.height / 2.0f);
  glm::mat4 model = glm::translate(glm::mat4(1.0f), quad_pos);
  glm::mat4 mvp = proj * model;

  vao.Bind();
  ebo.Bind();
  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  renderer.DrawTriangles(shader, vao, ebo);
}

void CircleShader::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);

  if (ImGui::Button("reload shader")) {
    shader.~ShaderProgram();
    new(&shader) ShaderProgram(shader_file);
  }
  ImGui::End();
}

}
