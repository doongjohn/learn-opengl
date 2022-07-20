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

SceneClearColor::SceneClearColor(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io)
  : Scene(window_w, window_h, renderer, io), clear_color(glm::vec3(1.0f, 1.0f, 1.0f)) { }
SceneClearColor::~SceneClearColor() { }

void SceneClearColor::OnUpdate(const float deltaTime) { }

void SceneClearColor::OnRender() {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
}

void SceneClearColor::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, ImGui::GetIO().Framerate);
  ImGui::ColorEdit3("clear color", (float*)&clear_color);
  ImGui::End();
}
