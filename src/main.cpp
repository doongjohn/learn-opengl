#include <array>
#include <tuple>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer/debugmessage.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"

using std::string;
using std::array;

int window_w = 800;
int window_h = 900;

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  window_w = width;
  window_h = height;
  glViewport(0, 0, width, height);
}

int main(int argc, char **argv) {
  glfwInit();

  // set opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  // set opengl profile to core
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow *window = glfwCreateWindow(window_w, window_h, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // create opengl context
  glfwMakeContextCurrent(window);

  // enable vsync
  glfwSwapInterval(1);

  // load opengl using glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // debug message handler
  init_gl_debug_msg_handler();

  // print opengl version
  std::cout << glGetString(GL_VERSION) << std::endl;

  // set viewport size
  glViewport(0, 0, window_w, window_h);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // set blend mode
  // https://www.youtube.com/watch?v=o1_yJ60UIxs&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=18
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //          src           dest
  //          │             └> the color that is already in the buffer
  //          the color to draw

  // image source: https://www.freeillustrated.com/illustrations/2018/08/10
  float image_scale = 0.3f;
  float image_w = 1639 * image_scale;
  float image_h = 2048 * image_scale;
  // vertex buffer
  auto positions = array {
    /* pos: */ -image_w / 2, -image_h / 2, /* texture coord: */ 0.0f, 0.0f,
    /* pos: */  image_w / 2, -image_h / 2, /* texture coord: */ 1.0f, 0.0f,
    /* pos: */  image_w / 2,  image_h / 2, /* texture coord: */ 1.0f, 1.0f,
    /* pos: */ -image_w / 2,  image_h / 2, /* texture coord: */ 0.0f, 1.0f
  };
  VertexBuffer vbo(positions.data(), sizeof(positions));

  // index buffer
  // (the order must be anti-clockwise)
  uint32_t indices[6] = {
    0, 1, 2,
    0, 2, 3,
  };
  IndexBuffer ebo(indices);

  VertexArray vao;
  vao.AttachVertexBuffer(vbo, {
    { .type = GL_FLOAT, .count = 2 }, // pos
    { .type = GL_FLOAT, .count = 2 }, // tex coord
  });
  vao.Unbind();

  vbo.Unbind();
  ebo.Unbind();

  // create shader
  ShaderProgram shader("./res/shaders/basic.glsl");
  shader.Bind();

  // create texture
  Texture texture("./res/textures/spoonful.jpg");
  texture.Bind(); // default slot 0

  shader.SetUniform1i("u_Texture", 0); // slot 0
  shader.Unbind(); // removes preformance warning

  // create renderer
  Renderer renderer;

  float rotation = 0.0f;

  // render loop
  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();

    vao.Bind();
    shader.Bind();

    // set mvp matrix
    rotation += 0.2f;
    glm::mat4 proj = glm::ortho(-(float)window_w / 2.0f, (float)window_w / 2.0f, -(float)window_h / 2.0f, (float)window_h / 2.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 mvp = proj * view * model;
    shader.SetUniformMat4f("u_Mvp", mvp);

    renderer.Draw(shader, vao, ebo);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
