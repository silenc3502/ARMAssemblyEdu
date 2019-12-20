#ifndef __OGL_SDL_SUPPORT_H__
#define __OGL_SDL_SUPPORT_H__

#include <SDL.h>
#include <GL/gl.h>
SDL_Surface* surface;
 
#define VERTEX_IDX 0
#define COORD_IDX 1
#define WIDTH 512
#define HEIGHT 512
 
typedef struct AppData {
    SDL_Window* window;
    SDL_GLContext gl;
 
    GLuint program;
    GLuint textureID;
} AppData;
 
GLuint LoadShader(const char *, GLenum);
GLuint compile_shaders(void);
void GLInit(AppData *);
void drawGL(AppData *);
int initGL(AppData *);

#endif
