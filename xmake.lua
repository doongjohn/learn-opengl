add_rules("mode.debug", "mode.release")

set_defaultplat("mingw")
set_defaultarchs("x86_64")

add_requires("glfw")

target("glad")
  set_kind("static")
  set_languages("c17")
  add_includedirs("src/vendor/glad/include")
  add_files("src/vendor/glad/src/*.c")
target_end()

target("stb")
  set_kind("static")
  set_languages("c17")
  add_files("src/vendor/stb/**.c")
target_end()

target("imgui")
  set_kind("static")
  add_packages("glfw")
  set_languages("cxx11")
  add_includedirs("src/vendor/imgui")
  add_files(
    "src/vendor/imgui/*.cpp",
    "src/vendor/imgui/backends/imgui_impl_glfw.cpp",
    "src/vendor/imgui/backends/imgui_impl_opengl3.cpp"
  )
target_end()

target("opengl")
  set_kind("binary")
  add_packages(
    "glfw"
  )
  add_deps(
    "glad",
    "stb",
    "imgui"
  )

  set_languages("cxx20")
  add_includedirs(
    "src",
    "src/vendor",
    "src/vendor/glm",
    "src/vendor/glad/include",
    "src/vendor/imgui"
  )
  add_files(
    "src/renderer/*.cpp",
    "src/main.cpp"
  )
  add_ldflags("-static")

  -- custom run function
  on_run(function (target)
    os.cd(os.projectdir())
    os.exec(target:targetfile())
  end)
target_end()
