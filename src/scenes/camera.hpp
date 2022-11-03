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
  glm::vec3 cam_forward;
  glm::vec3 cam_right;
  glm::vec3 cam_up;
  glm::vec3 cam_rot;

  glm::vec3 cube_pos;

  double cursor_x_prev = 0.0;
  double cursor_y_prev = 0.0;
  float cursor_dx = 0.0f;
  float cursor_dy = 0.0f;

public:
  Camera(GLFWwindow *window, Renderer& renderer, ImGuiIO& io);
  ~Camera();

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

inline Camera::Camera(GLFWwindow *window, Renderer &renderer, ImGuiIO &io)
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
  cam_pos = glm::vec3(0.0f);
  cam_rot = glm::vec3(0.0f);
  cube_pos = glm::vec3(0.0f, 0.0f, -10.0f);

  // setup mouse input
  // https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  if (glfwRawMouseMotionSupported())
    glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

  // init cursor state
  glfwGetCursorPos(window, &cursor_x_prev, &cursor_y_prev);
  cursor_dx = 0.0f;
  cursor_dy = 0.0f;


  // const float aspect_ratio = (float)renderer.width / renderer.height;
  // glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);
  // std::cout << glm::to_string(proj) << '\n';
  // mat4x4(
  // (3.218951, 0.000000, 0.000000, 0.000000),
  // (0.000000, 2.414213, 0.000000, 0.000000),
  // (0.000000, 0.000000, -1.002002, -1.000000),
  // (0.000000, 0.000000, -0.200200, 0.000000))
}
inline Camera::~Camera() {
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

inline void Camera::OnUpdate(const float delta_time) {
  // get mouse position
  double cursor_x;
  double cursor_y;
  glfwGetCursorPos(window, &cursor_x, &cursor_y);

  // show mouse when esc is pressed
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursor_x_prev = cursor_x;
    cursor_y_prev = cursor_y;
  }

  // get rotation from mouse input
  float mouse_speed = 0.06f;
  cursor_dx = (float)floor(cursor_x_prev - cursor_x);
  cursor_dy = (float)floor(cursor_y_prev - cursor_y);
  cam_rot.x += cursor_dy * mouse_speed;
  cam_rot.y += cursor_dx * mouse_speed;

  // camear move dir
  glm::vec3 move_dir = glm::vec3(0);

  int w = glfwGetKey(window, GLFW_KEY_W);
  int s = glfwGetKey(window, GLFW_KEY_S);
  int a = glfwGetKey(window, GLFW_KEY_A);
  int d = glfwGetKey(window, GLFW_KEY_D);
  if (w == GLFW_PRESS && s != GLFW_PRESS)
  {
    move_dir.z = 1;
  }
  if (s == GLFW_PRESS && w != GLFW_PRESS)
  {
    move_dir.z = -1;
  }
  if (a == GLFW_PRESS && d != GLFW_PRESS)
  {
    move_dir.x = -1;
  }
  if (d == GLFW_PRESS && a != GLFW_PRESS)
  {
    move_dir.x = 1;
  }

  int q = glfwGetKey(window, GLFW_KEY_Q);
  int e = glfwGetKey(window, GLFW_KEY_E);
  if (q == GLFW_PRESS && e != GLFW_PRESS)
  {
    move_dir.y = -1; // down
  }
  if (e == GLFW_PRESS && q != GLFW_PRESS)
  {
    move_dir.y = 1; // up
  }

  // move camera
  if (move_dir != glm::vec3(0, 0, 0)) {
    move_dir = glm::normalize(move_dir);
  }
  cam_pos += cam_forward * (move_dir.z * 0.1f);
  cam_pos += cam_right * (move_dir.x * 0.1f);
  cam_pos += cam_up * (move_dir.y * 0.1f);

  cursor_x_prev = cursor_x;
  cursor_y_prev = cursor_y;
}

void Camera::OnRender() {
  // create projection matrix
  const float aspect_ratio = (float)renderer.width / renderer.height;
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 100.0f);

  // create view matrix
  glm::mat4 view;
  glm::mat4 view_pos = glm::translate(glm::mat4(1.0f), cam_pos);
  glm::quat view_quat = glm::quat(glm::vec3(glm::radians(cam_rot.x), glm::radians(cam_rot.y), 0.0f)); // euler angles to quaternion

  // cam_forward = view_quat * glm::vec3(0.0f, 0.0f, -1.0f); // calc forward vector
  // cam_right = view_quat * glm::vec3(1.0f, 0.0f, 0.0f); // calc right vector
  // cam_up = view_quat * glm::vec3(0.0f, 1.0f, 0.0f); // calc up vector

  glm::mat4 view_rot = glm::mat4_cast(view_quat);
  // how to get direction vector from transform matrix
  // https://community.khronos.org/t/get-direction-from-transformation-matrix-or-quat/65502/2
  cam_forward = -glm::vec3(view_rot[2]); // get forward vector (use left hand coordinate because camera forward is left handed)
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

  glm::mat4 model = glm::translate(glm::mat4(1.0f), cube_pos);

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
