#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "ShaderUtil.h"
#include <chrono>

void render(GLint uniColor, float timeElapsed);

SDL_Window* window;
SDL_GLContext ctx;

int main() {

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cout << "SDL Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Forward compatible OpenGL context to SDL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  window = SDL_CreateWindow("Reekium", 
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED, 
                            800, 
                            600, 
                            SDL_WINDOW_OPENGL);

  ctx = SDL_GL_CreateContext(window);

  if (!ctx) {
    std::cout << "GL context failed\n";
    return 1;
  }

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW init failed\n";
    return 1;
  }

  GLuint shaderProgram = ShaderUtil::createShaderProgram(
    "shaders/vertex.glsl",
    "shaders/fragment.glsl"
  );

  glUseProgram(shaderProgram);

  bool running = true;
  SDL_Event e;

  // Rendering prerequisites
  
  GLuint vao, vbo;

  float vertices[] = {
    0.0f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  GLint uniColor = glGetUniformLocation(shaderProgram, "triColor");
  glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

  auto t_start = std::chrono::high_resolution_clock::now();
  
  // main loop
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
    }

    auto t_now = std::chrono::high_resolution_clock::now();
    float timeElapsed = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

    render(uniColor, timeElapsed);
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}

void render(GLint uniColor, float timeElapsed) {
  glClear(GL_COLOR_BUFFER_BIT);
  
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glUniform3f(uniColor, (sin(timeElapsed * 4.0f) + 1.0f) / 2.0f, (sin(timeElapsed * 3.0f) + 1.0f), (sin(timeElapsed * 2.0f) + 1.0f));
}
