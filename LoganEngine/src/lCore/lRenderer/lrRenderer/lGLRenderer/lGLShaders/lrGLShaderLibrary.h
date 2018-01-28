#ifndef LR_GL_SHADER_LIBRARY_H
#define LR_GL_SHADER_LIBRARY_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

#include <string>
#include <map>

class lrGLShaderLibrary
{
private:
	
	class lrGLShaderObject
	{
	private:
		bool Initialized = false;
		GLuint ShaderId = 0;
		GLenum ShaderType = 0;
		
	public:
		
		bool IsInitialized() const
		{
			return Initialized;
		}
		
		GLuint GetShaderId() const
		{
			return ShaderId;
		}
		
		GLenum GetShaderType() const
		{
			return ShaderType;
		}
		
		void Initialize(GLuint shader_id,GLenum shader_type)
		{
			ShaderId = shader_id;
			ShaderType = shader_type;
			Initialized = true;
		}
		
		lrGLShaderObject()
		{}
		
		~lrGLShaderObject()
		{
			if(Initialized)
			{
				glDeleteShader(ShaderId);
			}
		}
	};
	
	std::map<std::string,lrGLShaderObject> ShaderObjects;
	
public:
	
	static GLuint LoadShader(const char *src,GLenum shader_type,bool print_status);
	
	GLuint GetShader(const std::string &shader_name)
	{
		return ShaderObjects[shader_name].GetShaderId();
	}
	
	lrGLShaderLibrary();
	
	~lrGLShaderLibrary()
	{}
	
	/*
	 * End of class
	 */
};

#endif // LR_GL_SHADER_LIBRARY_H
