#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "ShaderUtil.h"

GLuint vertexShader, fragmentShader;

GLuint ShaderUtil::compileShader(GLenum type, const std::string& src) {
  GLuint shader = glCreateShader(type);

  const char* csrc = src.c_str();
  glShaderSource(shader, 1, &csrc, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char log[512];
    glGetShaderInfoLog(shader, 512, nullptr, log);
    std::cout << "Shader compile error:\n" << log << std::endl;
  }

  return shader;
}

std::string ShaderUtil::loadFile(const std::string& path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    std::cout << "Failed to open: " << path << std::endl;
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

GLuint ShaderUtil::createShaderProgram(const std::string& vertexPath,
                           const std::string& fragmentPath) {

  std::string vertexSrc = loadFile(vertexPath);
  std::string fragmentSrc = loadFile(fragmentPath);

  vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
  fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);


  GLuint program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glUseProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  if (!success) {
    char log[512];
    glGetProgramInfoLog(program, 512, nullptr, log);
    std::cout << "Program link error:\n" << log << std::endl;
  }


  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

void ShaderUtil::unloadShaders() {
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
}
