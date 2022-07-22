#pragma once

#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glad/glad.h"

#include "imgui/imgui.h"

#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"

class Scene {
protected:
  const Renderer& renderer;
  const ImGuiIO& io;

public:
  Scene(Renderer& renderer, ImGuiIO& io)
    : renderer(renderer), io(io) { }
  virtual ~Scene() {}

  virtual void OnUpdate(const float deltaTime) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};
