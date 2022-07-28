#include "shader.hpp"

using std::array;
using std::tuple;
using std::string;
using std::string_view;

// file path of vertex and fragment shader combined
ShaderProgram::ShaderProgram(const std::string& file_path)
  : gl_handle(0), file_path(file_path)
{
  auto [vertex_src, fragment_src] = this->ParseShader(file_path);
  this->gl_handle = ShaderProgram::CreateShader(vertex_src, fragment_src);

  auto props = array {
    GL_NAME_LENGTH,
    GL_TYPE,
    GL_ARRAY_SIZE,
  };
  array<GLint, 3> prop_data {0};

  // TODO: also get all uniform blocks
  // https://stackoverflow.com/questions/71506455/get-names-of-uniforms-in-uniform-block
  // https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade
  int uniform_count = 0;
  glGetProgramInterfaceiv(this->gl_handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &uniform_count);
  for (int i = 0; i < uniform_count; ++i) {
    glGetProgramResourceiv(this->gl_handle, GL_UNIFORM, i, 3, (GLenum*)props.data(), 3, nullptr, prop_data.data());

    string name;
    name.resize(prop_data[0] - 1); // exclude last '\0'
    glGetProgramResourceName(this->gl_handle, GL_UNIFORM, i, name.size() + 1, nullptr, (GLchar*)name.c_str());

    int location = glGetUniformLocation(this->gl_handle, name.c_str());
    this->uniform_cache[name] = location;
  }

  ShaderProgram::Unbind();
}
ShaderProgram::~ShaderProgram() {
  glDeleteProgram(this->gl_handle);
  ShaderProgram::Unbind();
}

void ShaderProgram::Bind() const {
  glUseProgram(this->gl_handle);
}
void ShaderProgram::Unbind() {
  glUseProgram(0);
}

void ShaderProgram::SetUniform1i(const std::string& name, const int32_t value) {
  glUniform1i(this->GetUniformLocation(name), value);
}
void ShaderProgram::SetUniform1f(const std::string& name, const float value) {
  glUniform1i(this->GetUniformLocation(name), value);
}
void ShaderProgram::SetUniform3f(const std::string& name, const glm::vec3& value) {
  glUniform3fv(this->GetUniformLocation(name), 1, &value[0]);
}
void ShaderProgram::SetUniform3f(const std::string& name, const std::array<float, 3>& value) {
  glUniform3fv(this->GetUniformLocation(name), 1, &value[0]);
}
void ShaderProgram::SetUniform4f(const std::string& name, const glm::vec4& value) {
  glUniform4fv(this->GetUniformLocation(name), 1, &value[0]);
}
void ShaderProgram::SetUniform4f(const std::string& name, const std::array<float, 4>& value) {
  glUniform4fv(this->GetUniformLocation(name), 1, &value[0]);
}
void ShaderProgram::SetUniformMat4f(const std::string& name, const glm::mat4& mat) {
  glUniformMatrix4fv(this->GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

// split vertex and fragment shader
tuple<string, string> ShaderProgram::ParseShader(const string& file_path) {
  enum class ShaderType { None = -1, Vertex = 0, Fragment = 1 };

  std::ifstream stream(file_path);
  std::stringstream ss[2];
  auto type = ShaderType::None;

  string line;
  while (getline(stream, line)) {
    if (line.starts_with("#type_vertex")) {
      type = ShaderType::Vertex;
    } else if (line.starts_with("#type_fragment")) {
      type = ShaderType::Fragment;
    } else {
      if (type != ShaderType::None)
        ss[(int)type] << line << '\n';
    }
  }

  return std::make_tuple(
    ss[0].str(),
    ss[1].str()
  );
}

uint32_t ShaderProgram::CompileShader(const uint32_t shader_type, const string& source) {
  uint32_t shader_handle = glCreateShader(shader_type);
  if (shader_handle == 0) {
    std::cout << "Error: failed to create a shader!\n";
    exit(1);
  }

  const char *src_str = source.c_str();
  glShaderSource(shader_handle, 1, &src_str, nullptr);
  glCompileShader(shader_handle);

  // get compile error
  int result;
  glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    // get message length
    int length = 0;
    glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &length);

    // get message
    string message;
    message.resize(length);
    glGetShaderInfoLog(shader_handle, length, &length, &message[0]);

    // print error message
    std::cout << "Failed to compile ";
    switch (shader_type) {
      case GL_VERTEX_SHADER:
        std::cout << "vertex shader!\n";
        break;
      case GL_FRAGMENT_SHADER:
        std::cout << "fragment shader!\n";
        break;
    }
    std::cout << message << '\n';

    // delete shader
    glDeleteShader(shader_handle);

    exit(1);
  }

  return shader_handle;
}

uint32_t ShaderProgram::CreateShader(const string& vertex_shader, const string& fragment_shader) {
  uint32_t program_handle = glCreateProgram();
  if (program_handle == 0) {
    std::cout << "Error: failed to create a shader program!\n";
    exit(1);
  }

  uint32_t vs_handle = ShaderProgram::CompileShader(GL_VERTEX_SHADER, vertex_shader);
  uint32_t fs_handle = ShaderProgram::CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  glAttachShader(program_handle, vs_handle);
  glAttachShader(program_handle, fs_handle);
  glLinkProgram(program_handle);
  glValidateProgram(program_handle);

  glDeleteShader(vs_handle);
  glDeleteShader(fs_handle);
  return program_handle;
}

int32_t ShaderProgram::GetUniformLocation(const std::string& name) {
  if (this->uniform_cache.find(name) != this->uniform_cache.end()) {
    return this->uniform_cache[name];
  } else {
    std::cout << "Error: Cannot find Shader Uniform \"" << name << "\"\n";
    return -1;
  }
}
