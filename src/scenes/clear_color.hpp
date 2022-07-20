#pragma once

#include "scene.hpp"

class SceneClearColor : public Scene {
private:
  glm::vec3 clear_color;

public:
  SceneClearColor(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io);
  ~SceneClearColor();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};
