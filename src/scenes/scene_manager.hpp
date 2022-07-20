#pragma once

#include <string>

#include "scenes/scene.hpp"
#include "scenes/clear_color.hpp"
#include "scenes/quad.hpp"
#include "scenes/quad_with_texture.hpp"

namespace SceneManager {

enum class Scenes {
  ClearColor,
  Quad,
  QuadWithTexture,
};

Scene* create(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io, Scenes sceneToLoad) {
  switch (sceneToLoad) {
    case Scenes::ClearColor:
      return new SceneClearColor
        (window_w, window_h, renderer, io);
    case Scenes::Quad:
      return new SceneQuad
        (window_w, window_h, renderer, io);
    case Scenes::QuadWithTexture:
      return new SceneQuadWithTexture
        (window_w, window_h, renderer, io);
    default:
      return nullptr;
  }
}

}
