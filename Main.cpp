#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "ShaderUtil.h"

void render();

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

  
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
    }

    render();
    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}

void render() {
  glClear(GL_COLOR_BUFFER_BIT);
  
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
