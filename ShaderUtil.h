#pragma once

#include <string>
#include <GL/glew.h>

namespace ShaderUtil {
  GLuint compileShader(GLenum type, const std::string& src);

  GLuint createShaderProgram(const std::string& vertexPath,
                             const std::string& fragmentPath);

  std::string loadFile(const std::string& path);

  void unloadShaders();
};
