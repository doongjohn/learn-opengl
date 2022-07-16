#pragma once

#include <array>
#include <tuple>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "glad/glad.h"

#include "glm/glm.hpp"

class ShaderProgram {
private:
  int32_t gl_id;
  std::string file_path;
  std::unordered_map<std::string, int32_t> uniform_cache;

public:
  ShaderProgram(const std::string& file_path);
  ~ShaderProgram();

  void Bind() const;
  void Unbind() const;

  void SetUniform1i(const std::string& name, const int32_t value);
  void SetUniform1f(const std::string& name, const float value);
  void SetUniform4f(const std::string& name, const std::array<float, 4>& value);
  void SetUniformMat4f(const std::string& name, const glm::mat4& mat4);

private:
  static std::tuple<std::string, std::string> ParseShader(const std::string& file_path);
  static uint32_t CompileShader(const uint32_t shader_type, const std::string& source);
  static uint32_t CreateShader(const std::string& vertex_shader, const std::string& fragment_shader);
  int32_t GetUniformLocation(const std::string& name);
};
