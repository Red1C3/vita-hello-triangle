#include <SDL_joystick.h>
#include<stdio.h>
#include<GLES2/gl2.h>
#include<psp2/kernel/processmgr.h>
#include<SDL.h>
#include<SDL_video.h>
#include<vector>
#include <SDL_events.h>
#include<fstream>
#define WIDTH 960
#define HEIGHT 544

using namespace std;

vector<char> readBinFile(const char* path){
    vector<char> data;
    fstream fileStream(path, ios::binary | ios::ate | ios::in);
    if(!fileStream.is_open()){
        printf("Failed to open file at %s", path);
    }
    data.resize(fileStream.tellg());
    fileStream.seekg(0, ios::beg);
    fileStream.read(data.data(), data.size());
    fileStream.flush();
    fileStream.close();
    return data;
}
GLuint loadShaders(const char* vertexShaderPath,const char* fragmentShaderPath) {
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
  GLint infoLogLen;
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
  vector<char> infoLog(infoLogLen);
  glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
  infoLog.push_back('\0');
  printf("%s", infoLog.data());
  infoLog.resize(0);
  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLen);
  infoLog.resize(infoLogLen);
  glGetShaderInfoLog(fragmentShader, infoLog.size(), nullptr, infoLog.data());
  infoLog.push_back('\0');
  printf("%s", infoLog.data());
  infoLog.resize(0);
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  printf("Linked shaders\n");
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
  infoLog.resize(infoLogLen);
  glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog.data());
  infoLog.push_back('\0');
  printf("%s", infoLog.data());
  //glDetachShader(program, vertexShader);
  //glDetachShader(program, fragmentShader);
  //glDeleteShader(vertexShader);
  //glDeleteShader(fragmentShader);

  return program;
}

int main() {
  printf("starting up hello-triangle\n");
  if (SDL_Init(SDL_INIT_VIDEO| SDL_INIT_JOYSTICK) < 0) {
    printf("Failed to init SDL, err:%s\n",SDL_GetError());
    sceKernelExitProcess(0);
    return 0;
  }

  SDL_Joystick* joystick;

  if(SDL_NumJoysticks()>0){
    joystick=SDL_JoystickOpen(0);
  }else{
    printf("Failed to find joystick\n");
  }

  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  auto window = SDL_CreateWindow("vita-hello-triangle", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                 SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("Failed to create window, err:%s\n",SDL_GetError());
    SDL_Quit();
    sceKernelExitProcess(0);
    return 0;
  }
  auto oglContext = SDL_GL_CreateContext(window);
  if (oglContext == nullptr) {
    printf("Failed to create opengl context, err: %s\n",SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    sceKernelExitProcess(0);
    return 0;
  }
  printf("Created opengl context successfully\n");
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  GLfloat vertexBuffer[] = {0.0f, 0.5f, -0.5, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(GLfloat), vertexBuffer,
               GL_STATIC_DRAW);
  printf("Created VBO\n");
  GLuint shaderProgram=loadShaders("app0:vertex.vert","app0:fragment.frag");
  glUseProgram(shaderProgram);
  printf("Loaded and using shader\n");
  GLint posLocation=glGetAttribLocation(shaderProgram,"pos");
  GLint colorLocation=glGetAttribLocation(shaderProgram,"color");
  glEnableVertexAttribArray(posLocation);
  glEnableVertexAttribArray(colorLocation);
  glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0,
                        (void *)(2 * 3 * sizeof(GLfloat)));
  glDrawArrays(GL_TRIANGLES, 0, 3);
  printf("Finished drawing\n");
  glDisableVertexAttribArray(posLocation);
  glDisableVertexAttribArray(colorLocation);
  printf("Opengl err code %i \n",glGetError());
  SDL_GL_SwapWindow(window);
  while(true){
    SDL_Event evt;
    SDL_PollEvent(&evt);
    if(evt.type==SDL_KEYUP && evt.key.keysym.scancode==SDL_SCANCODE_Q){
      break;
    }
    if(evt.type==SDL_JOYBUTTONDOWN){
      break;
    }
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  sceKernelExitProcess(0);
  return 0;
}
