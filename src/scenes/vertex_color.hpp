#pragma once

#include "scene.hpp"

class SceneVertexColor : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
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
  // https://stackoverflow.com/questions/14971282/applying-color-to-single-vertices-in-a-quad-in-opengl
  auto positions = std::array {
    -100.0f, -100.0f, /* color: */ 1.0f, 0.0f, 1.0f,
     100.0f, -100.0f, /* color: */ 0.0f, 1.0f, 0.5f,
     100.0f,  100.0f, /* color: */ 1.0f, 1.0f, 0.0f,
    -100.0f,  100.0f, /* color: */ 0.0f, 0.5f, 1.0f,
  };

  // index buffer
  // (the order must be anti-clockwise)
  uint32_t indices[6] = {
    0, 1, 2,
    0, 2, 3,
  };

  new(&vbo) VertexBuffer(positions.data(), sizeof(positions));
  new(&ebo) IndexBuffer(indices);

  vao.AttachVertexBuffer(vbo, {
    { .type = GL_FLOAT, .count = 2 }, // x, y pos
    { .type = GL_FLOAT, .count = 3 }, // color
  });
  vao.Unbind();

  vbo.Unbind();
  ebo.Unbind();

  // create shader
  new(&shader) ShaderProgram("./res/shaders/vertex_color.glsl");

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

  vao.Bind();
  ebo.Bind();
  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  renderer.Draw(shader, vao, ebo);
}

void SceneVertexColor::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);
  ImGui::End();
}
