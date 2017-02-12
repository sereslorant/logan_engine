#ifndef L_GL_INCLUDES_H
#define L_GL_INCLUDES_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
  #include <SDL2/SDL_opengl.h>
#else
  #include <GL/gl.h>
#endif

#endif // L_GL_INCLUDES_H
