#ifndef L_SDL2_GL_H
#define L_SDL2_GL_H

#include "../../lRenderer/lrGLRenderer/lGLExt.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>
#include <map>
#include <string>

class lSDL2GLExtLoader
{
private:
	std::map<std::string,bool> ExtStatus;

public:
    void PrintStatus(std::ostream &out);

	void LoadGLExtensions(lGLExt *GLExt);
	bool GetGLExtStatus(const std::string &Ext);

	lSDL2GLExtLoader();
	~lSDL2GLExtLoader();
};

#endif
