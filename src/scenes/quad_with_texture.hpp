#pragma once

#include "scene.hpp"

class SceneQuadWithTexture : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
  ShaderProgram shader;
  Texture texture;
  glm::vec3 quad_pos;
  glm::vec4 quad_color;

public:
  SceneQuadWithTexture(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io);
  ~SceneQuadWithTexture();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};
