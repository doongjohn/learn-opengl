#pragma once

#include <array>
#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"

#include "glad/glad.h"
#include "imgui/imgui.h"

#include "renderer/renderer.hpp"

class Scene {
protected:
  GLFWwindow *const window;
  const Renderer &renderer;
  const ImGuiIO &io;

public:
  Scene(GLFWwindow *window, Renderer& renderer, ImGuiIO& io)
    : window(window), renderer(renderer), io(io) { }
  virtual ~Scene() {}

  virtual void OnUpdate(const float delta_time) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};
