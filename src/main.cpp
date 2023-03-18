#include <GLES2/gl2.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_joystick.h>
#include <SDL_video.h>
#include <fstream>
#include <psp2/kernel/processmgr.h>
#include <stdio.h>
#include <vector>
#define WIDTH 960
#define HEIGHT 544

using namespace std;

vector<char> readBinFile(const char *path) {
  vector<char> data;
  fstream fileStream(path, ios::binary | ios::ate | ios::in);
  data.resize(fileStream.tellg());
  fileStream.seekg(0, ios::beg);
  fileStream.read(data.data(), data.size());
  fileStream.flush();
  fileStream.close();
  return data;
}
GLuint loadShaders(const char *vertexShaderPath,
                   const char *fragmentShaderPath) {
  GLuint program;
  vector<char> vertexShaderCode = readBinFile(vertexShaderPath);
  vector<char> fragmentShaderCode = readBinFile(fragmentShaderPath);
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  GLint vertexShaderSize = vertexShaderCode.size();
  GLint fragmentShaderSize = fragmentShaderCode.size();
  char *pVertexShaderCode = vertexShaderCode.data();
  char *pFragmentShaderCode = fragmentShaderCode.data();
  glShaderSource(vertexShader, 1, &pVertexShaderCode, &vertexShaderSize);
  glShaderSource(fragmentShader, 1, &pFragmentShaderCode, &fragmentShaderSize);
  printf("Loaded shaders\n");
  glCompileShader(vertexShader);
  glCompileShader(fragmentShader);
  printf("compiled shaders\n");
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  printf("Linked shaders\n");
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

int main() {
  printf("starting up hello-triangle\n");
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
    printf("Failed to init SDL, err:%s\n", SDL_GetError());
    sceKernelExitProcess(0);
    return 0;
  }

  SDL_Joystick *joystick;

  if (SDL_NumJoysticks() > 0) {
    joystick = SDL_JoystickOpen(0);
  } else {
    printf("Failed to find joystick\n");
    SDL_Quit();
    sceKernelExitProcess(0);
    return 0;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  auto window = SDL_CreateWindow("vita-hello-triangle", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("Failed to create window, err:%s\n", SDL_GetError());
    SDL_Quit();
    sceKernelExitProcess(0);
    return 0;
  }
  auto oglContext = SDL_GL_CreateContext(window);
  if (oglContext == nullptr) {
    printf("Failed to create opengl context, err: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    sceKernelExitProcess(0);
    return 0;
  }
  printf("Created opengl context successfully\n");
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  GLfloat vertexBuffer[] = {
      0.0f, 0.5f,       // vertex 0 position
      -0.5, 0.0f,       // vertex 1 position
      0.5f, 0.0f,       // vertex 2 position
      1.0f, 0.0f, 0.0f, // vertex 0 color
      0.0f, 1.0f, 0.0f, // vertex 1 color
      0.0f, 0.0f, 1.0f  // vertex 2 color
  };
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), vertexBuffer,
               GL_STATIC_DRAW);
  printf("Created VBO\n");
  GLuint shaderProgram =
      loadShaders("app0:shaders/vertex.vert", "app0:shaders/fragment.frag");
  glUseProgram(shaderProgram);
  printf("Loaded and using shader\n");
  GLint posLocation = glGetAttribLocation(shaderProgram, "pos"); //As mentioned in the shader
  GLint colorLocation = glGetAttribLocation(shaderProgram, "color"); //As mentioned in the shader
  glEnableVertexAttribArray(posLocation);
  glEnableVertexAttribArray(colorLocation);
  glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0,
                        (void *)(2 * 3 * sizeof(GLfloat)));
  glDrawArrays(GL_TRIANGLES, 0, 3);
  printf("Finished drawing\n");
  glDisableVertexAttribArray(posLocation);
  glDisableVertexAttribArray(colorLocation);
  printf("Opengl err code %i \n", glGetError());
  SDL_GL_SwapWindow(window); // Only draw a single frame
  while (true) {
    SDL_Event evt;
    SDL_PollEvent(&evt);
    if (evt.type == SDL_JOYBUTTONDOWN) {
      break;
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  sceKernelExitProcess(0);
  return 0;
}
