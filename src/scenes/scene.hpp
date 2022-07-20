#pragma once

#include <array>

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"

#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"

class Scene {
protected:
  const int& window_w;
  const int& window_h;
  const Renderer& renderer;
  const ImGuiIO& io;

public:
  Scene(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io)
    : window_w(window_w), window_h(window_h), renderer(renderer), io(io) { }
  virtual ~Scene() {}

  virtual void OnUpdate(const float deltaTime) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};
