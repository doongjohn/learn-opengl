#include "quad_with_texture.hpp"

SceneQuadWithTexture::SceneQuadWithTexture(int& window_w, int& window_h, Renderer& renderer, ImGuiIO& io)
  : Scene(window_w, window_h, renderer, io)
{
  // image source: https://www.freeillustrated.com/illustrations/2018/08/10
  float image_scale = 0.3f;
  float image_w = 1639 * image_scale;
  float image_h = 2048 * image_scale;
  // vertex buffer
  auto positions = std::array {
    /* pos: */ -image_w / 2, -image_h / 2, /* texture coord: */ 0.0f, 0.0f,
    /* pos: */  image_w / 2, -image_h / 2, /* texture coord: */ 1.0f, 0.0f,
    /* pos: */  image_w / 2,  image_h / 2, /* texture coord: */ 1.0f, 1.0f,
    /* pos: */ -image_w / 2,  image_h / 2, /* texture coord: */ 0.0f, 1.0f
  };

  // index buffer
  // (the order must be anti-clockwise)
  uint32_t indices[6] = {
    0, 1, 2,
    0, 2, 3,
  };

  vbo.Init(positions.data(), sizeof(positions));
  ebo.Init(indices);

  vao.Init();
  vao.AttachVertexBuffer(vbo, {
    { .type = GL_FLOAT, .count = 2 }, // x, y pos
    { .type = GL_FLOAT, .count = 2 }, // tex coord
  });
  vao.Unbind();

  vbo.Unbind();
  ebo.Unbind();

  // create shader
  shader.Init("./res/shaders/textured_quad.glsl");
  shader.Bind();

  // create texture
  texture.Init("./res/textures/spoonful.jpg");
  texture.Bind(); // default slot 0

  shader.SetUniform1i("u_Texture", 0); // slot 0
  shader.Unbind(); // removes preformance warning
  texture.Unbind();

  // initialize model position
  quad_pos = glm::vec3(0.0f, 0.0f, 0.0f);
  quad_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
SceneQuadWithTexture::~SceneQuadWithTexture() {
  vao.Unbind();
  vbo.Unbind();
  ebo.Unbind();
  shader.Unbind();
  texture.Unbind();
}

void SceneQuadWithTexture::OnUpdate(const float deltaTime) { }

void SceneQuadWithTexture::OnRender() {
  static float rotation = 0.0f;
  rotation += 0.2f;

  // set mvp matrix
  glm::mat4 proj = glm::ortho(-(float)window_w / 2.0f, (float)window_w / 2.0f, -(float)window_h / 2.0f, (float)window_h / 2.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 model_t = glm::translate(glm::mat4(1.0f), quad_pos);
  glm::mat4 model_r = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 mvp = proj * view * (model_t * model_r);

  vao.Bind();
  ebo.Bind();
  shader.Bind();
  shader.SetUniformMat4f("u_Mvp", mvp);
  shader.SetUniform4f("u_Tint", quad_color);
  texture.Bind();
  renderer.Draw(shader, vao, ebo);
}

void SceneQuadWithTexture::OnImGuiRender() {
  ImGui::Begin("Hello, world!");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
  ImGui::SliderFloat3("model position", &quad_pos[0], -200.0f, 200.0f);
  ImGui::ColorEdit4("Color", &quad_color[0]);
  ImGui::End();
}
