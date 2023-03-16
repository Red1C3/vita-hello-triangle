#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_video.h>
#define WIDTH 1280
#define HEIGHT 720
int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Failed to init SDL2");
    return -1;
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  auto window = SDL_CreateWindow("vita-hello-triangle", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                 SDL_WINDOW_OPENGL);
  if (window == nullptr) {
    printf("Failed to create sdl window");
    return -1;
  }
  auto oglContext = SDL_GL_CreateContext(window);
  if (oglContext == nullptr) {
    printf("Failed to create opengl context");
    return -1;
  }
  glClearColor(1,0,0,1);
  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(window);
  SDL_Delay(4000);
}
