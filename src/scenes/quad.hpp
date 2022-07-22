#pragma once

#include "scene.hpp"

class SceneQuad : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
  ShaderProgram shader;
  glm::vec3 quad_pos;
  glm::vec4 quad_color;

public:
  SceneQuad(Renderer& renderer, ImGuiIO& io);
  ~SceneQuad();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

SceneQuad::SceneQuad(Renderer& renderer, ImGuiIO& io)
  : Scene(renderer, io)
{
  auto positions = std::array {
    -100.0f, -100.0f,
     100.0f, -100.0f,
     100.0f,  100.0f,
    -100.0f,  100.0f,
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
    { .type = GL_FLOAT, .count = 2 }, // x, y pos
  });
  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();

  // create shader
  new(&shader) ShaderProgram("./res/shaders/basic.glsl");

  // initialize model position
  quad_pos = glm::vec3(0.0f, 0.0f, 0.0f);
  quad_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
SceneQuad::~SceneQuad() { }

void SceneQuad::OnUpdate(const float deltaTime) { }

void SceneQuad::OnRender() {
  static float rotation = 0.0f;
  rotation += 0.2f;

  // set mvp matrix
  glm::mat4 proj = glm::ortho(-renderer.width / 2.0f, renderer.width / 2.0f, -renderer.height / 2.0f, renderer.height / 2.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 model_t = glm::translate(glm::mat4(1.0f), quad_pos);
  glm::mat4 model_r = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 mvp = proj * view * (model_t * model_r);

  vao.Bind();
  ebo.Bind();
  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  shader.SetUniform4f("u_Color", quad_color);
  renderer.DrawTriangles(shader, vao, ebo);
}

void SceneQuad::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);
  ImGui::ColorEdit4("Color", &quad_color[0]);
  ImGui::End();
}
