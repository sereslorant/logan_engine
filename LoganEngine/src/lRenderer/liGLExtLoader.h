#ifndef LI_GL_EXT_LOADER_H
#define LI_GL_EXT_LOADER_H

#include <string>
#include <ostream>

class liGLExtLoader
{
public:

	class liExtFunctionLoader
	{
	public:
		virtual void *LoadFunction(const std::string &function_name) = 0;

		liExtFunctionLoader(){}
		virtual ~liExtFunctionLoader(){}
	};

	virtual void SetExtFunctionLoader(liExtFunctionLoader *function_loader) = 0;
	virtual void LoadExtensions() = 0;
	virtual void PrintExtStatus(std::ostream &out) = 0;

	liGLExtLoader()
	{}

	virtual ~liGLExtLoader()
	{}
};

#endif // LI_GL_EXT_LOADER_H
