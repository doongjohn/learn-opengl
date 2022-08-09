#pragma once

#include "scene.hpp"

namespace Scenes {

class Camera : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
  ShaderProgram shader;
  glm::vec3 cube_pos;
  glm::vec3 cam_pos;
  glm::vec3 cam_forward;
  glm::vec3 cam_up;
  glm::vec3 cam_right;

public:
  Camera(GLFWwindow *window, Renderer& renderer, ImGuiIO& io);
  ~Camera();

  void OnUpdate(const float delta_time) override;
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

void Camera::OnUpdate(const float delta_time) {
  int w = glfwGetKey(this->window, GLFW_KEY_W);
  int s = glfwGetKey(this->window, GLFW_KEY_S);
  int a = glfwGetKey(this->window, GLFW_KEY_A);
  int d = glfwGetKey(this->window, GLFW_KEY_D);
  glm::vec3 move_dir = glm::vec3(0);
  // forward
  if (w == GLFW_PRESS && s != GLFW_PRESS)
  {
    move_dir.z = 1;
  }
  // backward
  if (s == GLFW_PRESS && w != GLFW_PRESS)
  {
    move_dir.z = -1;
  }
  // left
  if (a == GLFW_PRESS && d != GLFW_PRESS)
  {
    move_dir.x = -1;
  }
  // right
  if (d == GLFW_PRESS && a != GLFW_PRESS)
  {
    move_dir.x = 1;
  }

  int arrow_up = glfwGetKey(this->window, GLFW_KEY_UP);
  int arrow_down = glfwGetKey(this->window, GLFW_KEY_DOWN);
  // up
  if (arrow_up == GLFW_PRESS && arrow_down != GLFW_PRESS)
  {
    move_dir.y = 1;
  }
  // down
  if (arrow_down == GLFW_PRESS && arrow_up != GLFW_PRESS)
  {
    move_dir.y = -1;
  }

  cam_pos += cam_forward * (move_dir.z * 0.1f);
  cam_pos += cam_right * (move_dir.x * 0.1f);
  cam_pos += cam_up * (move_dir.y * 0.1f);
}

void Camera::OnRender() {
  static float rotation = 0.0f;
  rotation += 0.2f;

  // create projection matrix
  const float aspect_ratio = (float)renderer.width / renderer.height;
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);

  // create view matrix
  glm::mat4 view;
  glm::mat4 view_pos = glm::translate(glm::mat4(1.0f), cam_pos);
  glm::quat view_quat = glm::quat(glm::vec3(0.0f, glm::radians(15.0f), 0.0f)); // euler angles to quaternion

  // cam_forward = view_quat * glm::vec3(0.0f, 0.0f, -1.0f); // calc forward vector
  // cam_right = view_quat * glm::vec3(1.0f, 0.0f, 0.0f); // calc right vector
  // cam_up = view_quat * glm::vec3(0.0f, 1.0f, 0.0f); // calc up vector
  glm::mat4 view_rot = glm::mat4_cast(view_quat);
  // how to get direction vector from transform matrix
  // https://community.khronos.org/t/get-direction-from-transformation-matrix-or-quat/65502/2
  cam_forward = -glm::vec3(view_rot[2]); // get forward vector
  cam_right = glm::vec3(view_rot[0]); // get right vector
  cam_up = glm::vec3(view_rot[1]); // get up vector
  // is this same as this?
  // https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
  // more about rotation
  // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#foreword-rotation-vs-orientation
  view = view_pos * view_rot;
  view = glm::inverse(view);
  //     ^^^^^^^^^^^^^^^^^^
  //     └-> inverse the view matrix to create an illusion of moving the camera

  glm::mat4 model =
    glm::translate(glm::mat4(1.0f), cube_pos) *
    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f, 0.0f, 1.0f));

  // create mvp
  glm::mat4 mvp = proj * view * model;

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
