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
#include "scenes/camera.hpp"
#include "scenes/circle_shader.hpp"
#include "scenes/heart_shader.hpp"

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
  "3d camera",
  "circle shader",
  "heart shader",
};

Scene* create_scene(GLFWwindow *window, Renderer &renderer, ImGuiIO &io, int scene_index) {
  switch (scene_index) {
    case 0: return new Scenes::ClearColor(window, renderer, io);
    case 1: return new Scenes::Quad(window, renderer, io);
    case 2: return new Scenes::VertexColorTriangle(window, renderer, io);
    case 3: return new Scenes::VertexColorQuad(window, renderer, io);
    case 4: return new Scenes::QuadWithTexture(window, renderer, io);
    case 5: return new Scenes::Perspective(window, renderer, io);
    case 6: return new Scenes::Cube(window, renderer, io);
    case 7: return new Scenes::Camera(window, renderer, io);
    case 8: return new Scenes::CircleShader(window, renderer, io);
    case 9: return new Scenes::HeartShader(window, renderer, io);
    default: return nullptr;
  }
}

int selection_buttons() {
  ImGui::Begin("Scenes");
  for (int i = 0; i < scene_list.size(); ++i) {
    if (ImGui::Button(scene_list[i])) {
      return i;
    }
  }
  ImGui::End();
  return -1;
}

void change_scene(GLFWwindow *window, Renderer &renderer, ImGuiIO &io, Scene *&current_scene, int next_scene_index) {
  if (next_scene_index < 0)
    return;

  delete current_scene;
  Scene *next_scene = SceneManager::create_scene(window, renderer, io, next_scene_index);
  current_scene = next_scene;
  renderer.SetClearColor(SceneManager::default_clear_color);
}

}
