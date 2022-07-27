#pragma once

#include <array>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glad/glad.h"
#include "imgui/imgui.h"

#include "renderer/renderer.hpp"

class Scene {
protected:
  const Renderer &renderer;
  const ImGuiIO &io;

public:
  Scene(Renderer& renderer, ImGuiIO& io)
    : renderer(renderer), io(io) { }
  virtual ~Scene() {}

  virtual void OnUpdate(const float deltaTime) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};
