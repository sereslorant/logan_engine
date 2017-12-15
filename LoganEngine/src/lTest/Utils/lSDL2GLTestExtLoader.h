#ifndef LSDL2_GL_TEST_EXT_LOADER_H
#define LSDL2_GL_TEST_EXT_LOADER_H

#include <SDL2/SDL.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

class lSDL2ExtensionLoader : public liGLExtLoader::liExtFunctionLoader
{
public:

	virtual void *LoadFunction(const std::string &function_name) override
	{
		return (void *)SDL_GL_GetProcAddress(function_name.c_str());
	}

	lSDL2ExtensionLoader()
	{}

	virtual ~lSDL2ExtensionLoader() override
	{}
};


#endif // LSDL2_GL_TEST_EXT_LOADER_H
