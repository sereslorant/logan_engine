#ifndef LSDL2_GL_EXT_FUNCTION_LOADER_H
#define LSDL2_GL_EXT_FUNCTION_LOADER_H

#include <SDL2/SDL.h>

#include <lRenderer/liGLExtLoader.h>

class lSDL2_GLExtFunctionLoader : public liGLExtLoader::liExtFunctionLoader
{
public:
	//
	virtual void *LoadFunction(const std::string &function_name) override
	{
		return SDL_GL_GetProcAddress(function_name.c_str());
	}
	//
	lSDL2_GLExtFunctionLoader()
	{}
	//
	virtual ~lSDL2_GLExtFunctionLoader()override
	{}
};

#endif // LSDL2_GL_EXT_FUNCTION_LOADER_H
