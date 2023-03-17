#include<stdio.h>
#include<psp2/kernel/processmgr.h>
#include<SDL.h>
#include<SDL_opengl.h>
#include<SDL_video.h>
#define WIDTH 960
#define HEIGHT 544
int main() {
  printf("starting up hello-triangle\n");
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Failed to init SDL, err:%s\n",SDL_GetError());
    sceKernelExitProcess(0);
    return 0;
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
  glClearColor(1,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  SDL_Delay(4000);
  SDL_DestroyWindow(window);
  SDL_Quit();
  sceKernelExitProcess(0);
  return 0;
}
