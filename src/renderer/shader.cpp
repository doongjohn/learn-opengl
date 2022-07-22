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

  // TODO: also do uniform block
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
}
ShaderProgram::~ShaderProgram() {
  glDeleteProgram(this->gl_handle);
  this->gl_handle = 0;
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
  uint32_t id = glCreateShader(shader_type);

  const char *src_str = source.c_str();
  glShaderSource(id, 1, &src_str, nullptr);
  glCompileShader(id);

  // get compile result
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    // get message length
    int length = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

    // get message
    string message;
    message.resize(length);
    glGetShaderInfoLog(id, length, &length, &message[0]);

    // print error message
    std::cout << "Failed to compile ";
    switch (shader_type) {
      case GL_VERTEX_SHADER:
        std::cout << "vertex shader!\n";
        break;
      case GL_FRAGMENT_SHADER:
        std::cout << "vertex shader!\n";
        break;
    }
    std::cout << message << std::endl;

    glDeleteShader(id);
    return 0;
  }

  return id;
}

uint32_t ShaderProgram::CreateShader(const string& vertex_shader, const string& fragment_shader) {
  uint32_t id = glCreateProgram();
  uint32_t vs = ShaderProgram::CompileShader(GL_VERTEX_SHADER, vertex_shader);
  uint32_t fs = ShaderProgram::CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

  // compilation failed
  if (vs * fs == 0) {
    return 0;
  }

  glAttachShader(id, vs);
  glAttachShader(id, fs);
  glLinkProgram(id);
  glValidateProgram(id);

  glDeleteShader(vs);
  glDeleteShader(fs);
  return id;
}

int32_t ShaderProgram::GetUniformLocation(const std::string& name) {
  if (this->uniform_cache.find(name) != this->uniform_cache.end()) {
    return this->uniform_cache[name];
  } else {
    std::cout << "Error: Shader Uniform \"" << name << "\" does not exist!\n";
    return -1;
  }
}
