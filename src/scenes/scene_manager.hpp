#pragma once

#include <array>
#include <string>

#include "scenes/scene.hpp"
#include "scenes/clear_color.hpp"
#include "scenes/quad.hpp"
#include "scenes/quad_with_texture.hpp"

namespace SceneManager {

using std::array;
using std::string;

static auto default_clear_color = glm::vec3(0.0f, 0.0f, 0.0f);
static auto scene_list = array {
  "clear color",
  "quad",
  "quad with texture",
};

Scene* create(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io, int scene_index) {
  switch (scene_index) {
    case 0:
      return new SceneClearColor
        (window_w, window_h, renderer, io);
    case 1:
      return new SceneQuad
        (window_w, window_h, renderer, io);
    case 2:
      return new SceneQuadWithTexture
        (window_w, window_h, renderer, io);
    default:
      return nullptr;
  }
}

Scene* selection_menu(Scene*& scene, int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io) {
  ImGui::Begin("Scenes");
  int i = 0;
  for (auto& scene : scene_list) {
    if (ImGui::Button(scene)) {
      return SceneManager::create(window_w, window_h, renderer, io, i);
    }
    ++i;
  }
  ImGui::End();
  return nullptr;
}

}
