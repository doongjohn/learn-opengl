#pragma once

#include <array>

#include "scene.hpp"
#include "scenes/clear_color.hpp"
#include "scenes/quad.hpp"
#include "scenes/vertex_color.hpp"
#include "scenes/quad_with_texture.hpp"

namespace SceneManager {

using std::array;

static auto default_clear_color = glm::vec3(0.0f, 0.0f, 0.0f);
static auto scene_list = array {
  "clear color",
  "quad",
  "vertex color",
  "quad with texture",
};

Scene* create_scene(Renderer& renderer, ImGuiIO& io, int scene_index) {
  switch (scene_index) {
    case 0:
      return new SceneClearColor(renderer, io);
    case 1:
      return new SceneQuad(renderer, io);
    case 2:
      return new SceneVertexColor(renderer, io);
    case 3:
      return new SceneQuadWithTexture(renderer, io);
    default:
      return nullptr;
  }
}

Scene* selection_menu(Scene*& scene, Renderer& renderer, ImGuiIO& io) {
  ImGui::Begin("Scenes");
  int i = 0;
  for (auto& scene : scene_list) {
    if (ImGui::Button(scene)) {
      return SceneManager::create_scene(renderer, io, i);
    }
    ++i;
  }
  ImGui::End();
  return nullptr;
}

}
