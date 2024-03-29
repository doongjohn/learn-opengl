#pragma once

#include "scene.hpp"

namespace Scenes {

class Cube : public Scene {
private:
  VertexBuffer vbo;
  VertexArray vao;
  IndexBuffer ebo;
  ShaderProgram shader;
  glm::vec3 cube_pos;

public:
  Cube(GLFWwindow *window, Renderer& renderer, ImGuiIO& io);
  ~Cube();

  void OnUpdate(const float delta_time) override;
  void OnRender() override;
  void OnImGuiRender() override;
};

inline Cube::Cube(GLFWwindow *window, Renderer &renderer, ImGuiIO &io)
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

  // initialize model position
  cube_pos = glm::vec3(0.0f, 0.0f, -10.0f);

  // calculating inverse transform
  // https://stackoverflow.com/questions/69639800/opengl-the-inverse-of-the-transformations
  glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(1, 2, 3));
  glm::mat4 r = glm::rotate(glm::mat4(1.0f), glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 s = glm::scale(glm::mat4(1.0f), glm::vec3(3, 4, 5));
  glm::mat4 m1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1, -2, -3)) * glm::transpose(r) * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/3.0f, 1.0f/4.0f, 1.0f/5.0f));
  glm::mat4 m2 = glm::inverse(s * r * t);
  std::cout << glm::to_string(m1) << '\n';
  std::cout << glm::to_string(m2) << '\n';
}
inline Cube::~Cube() { }

inline void Cube::OnUpdate(const float delta_time) { }

inline void Cube::OnRender() {
  static float rotation = 0.0f;
  rotation += 0.2f;

  // set mvp matrix
  // Perspective Projection - Part 1 https://youtu.be/LhQ85bPCAJ8
  glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)renderer.width / renderer.height, 0.1f, 100.0f);
  glm::mat4 model =
    glm::translate(glm::mat4(1.0f), cube_pos) *
    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(1.0f, 0.0f, 1.0f));
  glm::mat4 mvp = proj * model;

  vao.Bind();
  ebo.Bind();
  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  renderer.DrawTriangles(shader, vao, ebo);
}

inline void Cube::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &cube_pos[0], -10.0f, 10.0f);
  ImGui::End();
}

}
