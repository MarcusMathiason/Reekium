#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <iostream>
#include "ShaderUtil.h"
#include <chrono>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

  // Set window icon
  SDL_Surface* icon = SDL_LoadBMP("res/icon.png");
  SDL_SetWindowIcon(window, icon);
  SDL_FreeSurface(icon);

  GLuint shaderProgram = ShaderUtil::createShaderProgram(
    "shaders/vertex.glsl",
    "shaders/fragment.glsl"
  );

  glUseProgram(shaderProgram);

  bool running = true;
  SDL_Event e;

  // Rendering prerequisites
  
  GLuint vao, vbo;

  // Init vertex array object & buffer
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);

  GLfloat vertices[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left;
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Init element array

  GLuint ebo;
  glGenBuffers(1, &ebo);

  GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
  };
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

  // Init textures
  GLuint tex;

  glGenTextures(1, &tex);

  glBindTexture(GL_TEXTURE_2D, tex);

  // pos
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0);

  // col
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(2*sizeof(float)));

  // tex
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));


  // Load texure image
  
  GLuint textures[2];
  glGenTextures(2, textures);
  
  int w, h;
  unsigned char* image;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  image = SOIL_load_image("res/sample.png", &w, &h, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  image = SOIL_load_image("res/sample2.png", &w, &h, 0, SOIL_LOAD_RGB);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  SOIL_free_image_data(image);
  glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  // Transformations

  auto t_start = std::chrono::high_resolution_clock::now();


  // main loop
  while (running) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) running = false;
    }

    
  auto t_now = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::rotate(trans, time * glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  glm::vec4 result = trans * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  printf("%f, %f, %f\n", result.x, result.y, result.z);

  GLint uniTrans = glGetUniformLocation(shaderProgram, "transformation");
  glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

    render();

    SDL_GL_SwapWindow(window);
  }

  // Clean up before closing program
  glDeleteTextures(1, &tex);
  
  glDeleteProgram(shaderProgram);
  ShaderUtil::unloadShaders();
  
  glDeleteBuffers(1, &ebo);
  glDeleteBuffers(1, &vbo);

  glDeleteVertexArrays(1, &vao);

  SDL_GL_DeleteContext(ctx);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}

void render() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
