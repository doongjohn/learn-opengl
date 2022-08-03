#pragma once

#include "scene.hpp"

namespace Scenes {

class Camera : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
  ShaderProgram shader;
  glm::vec3 cam_pos;
  glm::vec3 cube_pos;

public:
  Camera(GLFWwindow *window, Renderer& renderer, ImGuiIO& io);
  ~Camera();

  void OnUpdate(const float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

Camera::Camera(GLFWwindow *window, Renderer &renderer, ImGuiIO &io)
  : Scene(window, renderer, io) {
  struct Vertex {
    float pos[3];
    float uv[2];
  };

  auto positions = std::array {
    Vertex { .pos = { -1.0f, -1.0f, -1.0f }, .uv = { 0, 0 } },
    Vertex { .pos = {  1.0f, -1.0f, -1.0f }, .uv = { 1, 0 } },
    Vertex { .pos = {  1.0f,  1.0f, -1.0f }, .uv = { 1, 1 } },
    Vertex { .pos = { -1.0f,  1.0f, -1.0f }, .uv = { 0, 1 } },
    Vertex { .pos = { -1.0f, -1.0f,  1.0f }, .uv = { 0, 0 } },
    Vertex { .pos = {  1.0f, -1.0f,  1.0f }, .uv = { 1, 0 } },
    Vertex { .pos = {  1.0f,  1.0f,  1.0f }, .uv = { 1, 1 } },
    Vertex { .pos = { -1.0f,  1.0f,  1.0f }, .uv = { 0, 1 } },
  };

  uint32_t indices[6 * 3 * 2] = {
    0, 1, 3, 3, 1, 2,
    1, 5, 2, 2, 5, 6,
    5, 4, 6, 6, 4, 7,
    4, 0, 7, 7, 0, 3,
    3, 2, 7, 7, 2, 6,
    4, 5, 0, 0, 5, 1
  };

  new(&vbo) VertexBuffer(positions.data(), sizeof(positions));
  new(&ebo) IndexBuffer(indices);

  vao.AttachVertexBuffer(vbo, {
    { .type = GL_FLOAT, .count = 3 },
    { .type = GL_FLOAT, .count = 2 },
  });
  vao.Unbind();

  // create shader
  new(&shader) ShaderProgram("./res/shaders/vertex_color_quad.glsl");
  // set colors
  shader.Bind();
  shader.SetUniform3f("u_Color_00", glm::vec3(254, 226, 179) / 255.0f);
  shader.SetUniform3f("u_Color_10", glm::vec3(255, 162, 153) / 255.0f);
  shader.SetUniform3f("u_Color_11", glm::vec3(173, 105, 137) / 255.0f);
  shader.SetUniform3f("u_Color_01", glm::vec3(86, 35, 73) / 255.0f);
  shader.Unbind();

  // initialize position
  cam_pos = glm::vec3(0.0f, 0.0f, 0.0f);
  cube_pos = glm::vec3(0.0f, 0.0f, -10.0f);
}
Camera::~Camera() { }

void Camera::OnUpdate(const float deltaTime) {
  int w = glfwGetKey(this->window, GLFW_KEY_W);
  int s = glfwGetKey(this->window, GLFW_KEY_S);
  int a = glfwGetKey(this->window, GLFW_KEY_A);
  int d = glfwGetKey(this->window, GLFW_KEY_D);
  // forward
  if (w == GLFW_PRESS && s != GLFW_PRESS)
  {
    cam_pos.z -= 0.1f;
  }
  // backward
  if (s == GLFW_PRESS && w != GLFW_PRESS)
  {
    cam_pos.z += 0.1f;
  }
  // left
  if (a == GLFW_PRESS && d != GLFW_PRESS)
  {
    cam_pos.x -= 0.1f;
  }
  // right
  if (d == GLFW_PRESS && a != GLFW_PRESS)
  {
    cam_pos.x += 0.1f;
  }

  int arrow_up = glfwGetKey(this->window, GLFW_KEY_UP);
  int arrow_down = glfwGetKey(this->window, GLFW_KEY_DOWN);
  // up
  if (arrow_up == GLFW_PRESS && arrow_down != GLFW_PRESS)
  {
    cam_pos.y += 0.1f;
  }
  // down
  if (arrow_down == GLFW_PRESS && arrow_up != GLFW_PRESS)
  {
    cam_pos.y -= 0.1f;
  }
}

void Camera::OnRender() {
  static float rotation = 0.0f;
  rotation += 0.2f;

  // set mvp matrix
  // Perspective Projection - Part 1 https://youtu.be/LhQ85bPCAJ8
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)renderer.width / renderer.height, 0.1f, 100.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), cam_pos);
  glm::mat4 model =
    glm::translate(glm::mat4(1.0f), cube_pos) *
    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f, 0.0f, 1.0f));
  glm::mat4 mvp = proj * glm::inverse(view) * model;
  //                     ^^^^^^^^^^^^^^^^^
  //                     └-> inverse the camera transform to create an illusion of moving the camera

  vao.Bind();
  ebo.Bind();
  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  renderer.DrawTriangles(shader, vao, ebo);
}

void Camera::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::Text("Press W, A, S, D to move a camera.");
  ImGui::SliderFloat3("model position", &cube_pos[0], -10.0f, 10.0f);
  ImGui::End();
}

}
