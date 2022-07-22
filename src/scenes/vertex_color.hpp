#pragma once

#include "scene.hpp"

class SceneVertexColor : public Scene {
private:
  VertexBuffer vbo;
  IndexBuffer ebo;
  VertexArray vao;
  ShaderProgram shader;
  glm::vec3 quad_pos;

public:
  SceneVertexColor(Renderer& renderer, ImGuiIO& io);
  ~SceneVertexColor();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

SceneVertexColor::SceneVertexColor(Renderer& renderer, ImGuiIO& io)
  : Scene(renderer, io)
{
  struct Vertex {
    float pos[2];
    float uv[2];
  };

  auto positions = std::array {
    Vertex{ .pos = { -100.0f, -100.0f }, .uv = { 0, 0 } },
    Vertex{ .pos = {  100.0f, -100.0f }, .uv = { 1, 0 } },
    Vertex{ .pos = {  100.0f,  100.0f }, .uv = { 1, 1 } },
    Vertex{ .pos = { -100.0f,  100.0f }, .uv = { 0, 1 } },
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
  vbo.Unbind();
  ebo.Unbind();

  // create shader
  new(&shader) ShaderProgram("./res/shaders/vertex_color_quad.glsl");

  // initialize model position
  quad_pos = glm::vec3(0.0f, 0.0f, 0.0f);
}
SceneVertexColor::~SceneVertexColor() { }

void SceneVertexColor::OnUpdate(const float deltaTime) { }

void SceneVertexColor::OnRender() {
  static float rotation = 0.0f;
  rotation += 0.2f;

  // set mvp matrix
  glm::mat4 proj = glm::ortho(-renderer.width / 2.0f, renderer.width / 2.0f, -renderer.height / 2.0f, renderer.height / 2.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 model_t = glm::translate(glm::mat4(1.0f), quad_pos);
  glm::mat4 model_r = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 mvp = proj * view * (model_t * model_r);

  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  shader.SetUniform3f("u_Color_bl", glm::vec3(1.0f, 0.0f, 0.0f));
  shader.SetUniform3f("u_Color_br", glm::vec3(0.0f, 1.0f, 0.0f));
  shader.SetUniform3f("u_Color_tr", glm::vec3(0.0f, 0.0f, 1.0f));
  shader.SetUniform3f("u_Color_tl", glm::vec3(1.0f, 1.0f, 1.0f));

  renderer.DrawTriangles(shader, vao, ebo);
}

void SceneVertexColor::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);
  ImGui::End();
}
