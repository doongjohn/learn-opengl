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
  SceneQuad(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io);
  ~SceneQuad();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};
