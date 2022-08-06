#include <string>
#include <iostream>
#include <chrono>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "renderer/debugmessage.hpp"
#include "renderer/renderer.hpp"
#include "scene_manager.hpp"

using std::string;

static Renderer renderer(600, 800);

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  renderer.width = width;
  renderer.height = height;
  glViewport(0, 0, width, height);
}

int main(int argc, char **argv) {
  // TODO: rotate camera using mouse
  // https://www.youtube.com/watch?v=MZuYmG1GBFk&list=PLA0dXqQjCx0S04ntJKUftl6OaOgsiwHjA&index=17
  // https://stackoverflow.com/questions/4431637/hiding-mouse-cursor-with-glfw

  glfwInit();

  // set opengl version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

  // set opengl profile to core
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // create window
  GLFWwindow *window = glfwCreateWindow(renderer.width, renderer.height, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return 1;
  }

  // create opengl context
  glfwMakeContextCurrent(window);

  // enable vsync
  glfwSwapInterval(1);

  // load opengl using glad
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return 1;
  }

  // debug message handler
  init_gl_debug_msg_handler();

  // print opengl version
  std::cout << glGetString(GL_VERSION) << std::endl;

  // set viewport size
  glViewport(0, 0, renderer.width, renderer.height);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // enable depth testing
  glEnable(GL_DEPTH_TEST);

  // enable blend mode
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

  // create scene
  Scene* scene = SceneManager::create_scene(window, renderer, io, 0);

  // main loop
  float frame_last_time = (float)glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    // calculate delta time
    float frame_start_time = (float)glfwGetTime();
    float delta_time = (frame_start_time - frame_last_time) / 1000.0f;
    frame_last_time = frame_start_time;

    // clear screen
    renderer.Clear();

    // scene update
    scene->OnUpdate(delta_time);
    scene->OnRender();

    // prepare imgui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // scene ui
    scene->OnImGuiRender();

    // change scene
    auto next_scene = SceneManager::draw_menu(scene, window, renderer, io);
    SceneManager::change_scene(renderer, scene, next_scene);

    // draw imgui
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
