set_defaultplat("mingw")
set_defaultarchs("x86_64")

add_rules("mode.debug", "mode.release")

add_requires("glfw")

target("glad")
  set_kind("static")
  set_languages("c17")
  add_includedirs("glad/include")
  add_files("glad/src/*.c")
target_end()

target("stb")
  set_kind("static")
  set_languages("c17")
  add_includedirs("src/vendor")
  add_files("src/vendor/**.c")
target_end()

target("opengl")
  set_kind("binary")

  add_deps(
    "glad",
    "stb"
  )
  add_packages(
    "glfw"
  )

  set_languages("c++20")
  add_includedirs(
    "glad/include",
    "src/vendor",
    "src/vendor/glm"
  )
  add_files("src/**.cpp|vendor/glm/**.cpp")
  add_ldflags("-static")

  -- custom run function
  on_run(function (target)
    os.cd(os.projectdir())
    os.exec(target:targetfile())
  end)
target_end()
