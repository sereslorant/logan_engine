#ifndef L_GL_FUNCTIONS_H
#define L_GL_FUNCTIONS_H

#include "lGLExt.h"

using namespace glExt;

#include "../../lInterfaces/lResourceManager/liBitmap.h"

GLuint lGL_CreateEmptyTexture(unsigned int width,unsigned int height,bool is_depth_texture = false);
void lGL_GetImageInfo(liBitmap *Bmp,GLenum *InternalFormat,GLenum *Type,bool *AlphaBlending);

#endif // L_GL_FUNCTIONS_H
