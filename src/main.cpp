#include <array>
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "renderer/debugmessage.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"
#include "renderer/index_buffer.hpp"
#include "renderer/shader.hpp"
#include "renderer/texture.hpp"
#include "renderer/renderer.hpp"

#include "scene_manager.hpp"

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

  // imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 460 core");

  // create renderer
  Renderer renderer;

  // create scene
  Scene* scene = SceneManager::create(window_w, window_h, renderer, io, 0);

  // render loop
  while (!glfwWindowShouldClose(window)) {
    renderer.Clear();
    scene->OnRender();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    scene->OnImGuiRender();

    auto nextScene = SceneManager::selection_menu(scene, window_w, window_h, renderer, io);
    if (nextScene) {
      VertexArray::Unbind();
      VertexBuffer::Unbind();
      IndexBuffer::Unbind();
      ShaderProgram::Unbind();
      Texture::Unbind();
      delete scene;
      scene = nextScene;
      renderer.SetClearColor(SceneManager::default_clear_color);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete scene;

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
