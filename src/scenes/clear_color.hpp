#pragma once

#include "scene.hpp"

namespace Scenes {

class ClearColor : public Scene {
private:
  glm::vec3 clear_color;

public:
  ClearColor(Renderer& renderer, ImGuiIO& io);
  ~ClearColor();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

ClearColor::ClearColor(Renderer& renderer, ImGuiIO& io)
  : Scene(renderer, io), clear_color(glm::vec3(1.0f, 1.0f, 1.0f)) { }
ClearColor::~ClearColor() { }

void ClearColor::OnUpdate(const float deltaTime) { }

void ClearColor::OnRender() {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, 1.0f);
}

void ClearColor::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, ImGui::GetIO().Framerate);
  ImGui::ColorEdit3("clear color", (float*)&clear_color);
  ImGui::End();
}

}
