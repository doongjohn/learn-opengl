#pragma once

#include "scene.hpp"

class SceneVertexColorQuad : public Scene {
private:
  VertexBuffer vbo;
  IndexBuffer ebo;
  VertexArray vao;
  ShaderProgram shader;
  glm::vec3 quad_pos;

public:
  SceneVertexColorQuad(Renderer& renderer, ImGuiIO& io);
  ~SceneVertexColorQuad();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

SceneVertexColorQuad::SceneVertexColorQuad(Renderer& renderer, ImGuiIO& io)
  : Scene(renderer, io)
{
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
  vbo.Unbind();
  ebo.Unbind();

  // create shader
  new(&shader) ShaderProgram("./res/shaders/vertex_color_quad.glsl");
  shader.Bind();
  shader.SetUniform3f("u_Color_bl", glm::vec3(254, 226, 179) / 255.0f);
  shader.SetUniform3f("u_Color_br", glm::vec3(255, 162, 153) / 255.0f);
  shader.SetUniform3f("u_Color_tr", glm::vec3(173, 105, 137) / 255.0f);
  shader.SetUniform3f("u_Color_tl", glm::vec3(86, 35, 73) / 255.0f);
  shader.Unbind();

  // initialize model position
  quad_pos = glm::vec3(0.0f, 0.0f, 0.0f);
}
SceneVertexColorQuad::~SceneVertexColorQuad() { }

void SceneVertexColorQuad::OnUpdate(const float deltaTime) { }

void SceneVertexColorQuad::OnRender() {
  // set mvp matrix
  glm::mat4 proj = glm::ortho(-renderer.width / 2.0f, renderer.width / 2.0f, -renderer.height / 2.0f, renderer.height / 2.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 model_t = glm::translate(glm::mat4(1.0f), quad_pos);
  glm::mat4 mvp = proj * view * model_t;

  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);

  renderer.DrawTriangles(shader, vao, ebo);
}

void SceneVertexColorQuad::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);
  ImGui::End();
}
