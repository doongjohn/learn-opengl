#pragma once

#include <array>

#include "renderer/renderer.hpp"
#include "scene.hpp"
#include "scenes/clear_color.hpp"
#include "scenes/quad.hpp"
#include "scenes/vertex_color_triangle.hpp"
#include "scenes/vertex_color_quad.hpp"
#include "scenes/texture_quad.hpp"
#include "scenes/perspective.hpp"
#include "scenes/cube.hpp"
#include "scenes/circle_shader.hpp"

namespace SceneManager {

using std::array;

static constexpr auto default_clear_color = glm::vec3(0.0f);
static constexpr auto scene_list = array {
  "clear color",
  "quad",
  "vertex color triangle",
  "vertex color quad",
  "texture quad",
  "3d perspective",
  "3d cube",
  "circle shader",
};

Scene* create_scene(Renderer &renderer, ImGuiIO &io, int scene_index) {
  switch (scene_index) {
    case 0: return new Scenes::ClearColor(renderer, io);
    case 1: return new Scenes::Quad(renderer, io);
    case 2: return new Scenes::VertexColorTriangle(renderer, io);
    case 3: return new Scenes::VertexColorQuad(renderer, io);
    case 4: return new Scenes::QuadWithTexture(renderer, io);
    case 5: return new Scenes::Perspective(renderer, io);
    case 6: return new Scenes::Cube(renderer, io);
    case 7: return new Scenes::CircleShader(renderer, io);
    default: return nullptr;
  }
}

Scene* draw_menu(Scene *&scene, Renderer &renderer, ImGuiIO &io) {
  ImGui::Begin("Scenes");
  for (int i = 0; i < scene_list.size(); ++i) {
    if (ImGui::Button(scene_list[i])) {
      return SceneManager::create_scene(renderer, io, i);
    }
  }
  ImGui::End();
  return nullptr;
}

void change_scene(Renderer &renderer, Scene *&current_scene, Scene *&next_scene) {
  if (next_scene) {
    delete current_scene;
    current_scene = next_scene;
    renderer.SetClearColor(SceneManager::default_clear_color);
  }
}

}
