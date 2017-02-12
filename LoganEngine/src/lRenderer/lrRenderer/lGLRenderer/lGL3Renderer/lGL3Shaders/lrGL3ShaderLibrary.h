#ifndef LR_GL_SHADER_LIBRARY_H
#define LR_GL_SHADER_LIBRARY_H

#include "../../lGLIncludes.h"
#include "../../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PRINT_GL_SHADER_COMPILE_STATUS
#include <iostream>
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

extern const char *VertexShaderSource2D;
extern const char *VertexShaderPrepassSource;
extern const char *FragmentShaderSource2D;
extern const char *VertexShaderSource;
extern const char *PbEquationsSource;
extern const char *FwdFragmentShaderSrc;
extern const char *EnvMapShaderSource;
extern const char *FragmentShaderSource;

#include <string>
#include <map>

class lrGL3ShaderLibrary
{
private:
	//
	class lrGL3ShaderObject
	{
	private:
		bool Initialized = false;
		GLuint ShaderId = 0;
		GLenum ShaderType = 0;
		//
	public:
		//
		bool IsInitialized() const
		{
			return Initialized;
		}
		//
		GLuint GetShaderId() const
		{
			return ShaderId;
		}
		//
		GLenum GetShaderType() const
		{
			return ShaderType;
		}
		//
		void Initialize(GLuint shader_id,GLenum shader_type)
		{
			ShaderId = shader_id;
			ShaderType = shader_type;
			Initialized = true;
		}
		//
		lrGL3ShaderObject()
		{}
		//
		~lrGL3ShaderObject()
		{
			if(Initialized)
			{
				glDeleteShader(ShaderId);
			}
		}
	};
	//
	std::map<std::string,lrGL3ShaderObject> ShaderObjects;
	//
public:
	//
	static GLuint LoadShader(const char *src,GLenum shader_type)
	{
		GLuint Id;
		Id = glCreateShader(shader_type);
		//
		glShaderSource(Id,1,&src,NULL);
		glCompileShader(Id);
		//
		#ifdef PRINT_GL_SHADER_COMPILE_STATUS
			char Error[1024];

			glGetShaderInfoLog(Id,1024,NULL,Error);
			std::cout << "Shader compile status:\n" << Error << std::endl;
		#endif
		//
		return Id;
	}
	//
	GLuint GetShader(const std::string &shader_name)
	{
		return ShaderObjects[shader_name].GetShaderId();
	}
	//
	lrGL3ShaderLibrary()
	{
		lrGL3ShaderObject &VertexShader2D = ShaderObjects["VertexShader2D"];
		VertexShader2D.Initialize(LoadShader(VertexShaderSource2D,GL_VERTEX_SHADER),GL_VERTEX_SHADER);
		//
		lrGL3ShaderObject &FragmentShader2D = ShaderObjects["FragmentShader2D"];
		FragmentShader2D.Initialize(LoadShader(FragmentShaderSource2D,GL_FRAGMENT_SHADER),GL_FRAGMENT_SHADER);
		//
		lrGL3ShaderObject &StaticMeshPrepassVertexShader = ShaderObjects["StaticMeshPrepassVertexShader"];
		StaticMeshPrepassVertexShader.Initialize(LoadShader(VertexShaderPrepassSource,GL_VERTEX_SHADER),GL_VERTEX_SHADER);
		//
		lrGL3ShaderObject &StaticMeshVertexShader = ShaderObjects["StaticMeshVertexShader"];
		StaticMeshVertexShader.Initialize(LoadShader(VertexShaderSource,GL_VERTEX_SHADER),GL_VERTEX_SHADER);
		//
		lrGL3ShaderObject &PbEquationsShader = ShaderObjects["PbEquationsShader"];
		PbEquationsShader.Initialize(LoadShader(PbEquationsSource,GL_FRAGMENT_SHADER),GL_FRAGMENT_SHADER);
		//
		lrGL3ShaderObject &FwdRendererShader = ShaderObjects["FwdRendererShader"];
		FwdRendererShader.Initialize(LoadShader(FwdFragmentShaderSrc,GL_FRAGMENT_SHADER),GL_FRAGMENT_SHADER);
		//
		lrGL3ShaderObject &PbEnvMapShaderMain = ShaderObjects["PbEnvMapShaderMain"];
		PbEnvMapShaderMain.Initialize(LoadShader(EnvMapShaderSource,GL_FRAGMENT_SHADER),GL_FRAGMENT_SHADER);
		//
		lrGL3ShaderObject &PbFragmentShaderMain = ShaderObjects["PbFragmentShaderMain"];
		PbFragmentShaderMain.Initialize(LoadShader(FragmentShaderSource,GL_FRAGMENT_SHADER),GL_FRAGMENT_SHADER);
	}
	//
	~lrGL3ShaderLibrary(){}
	//
	/*
	 * End of class
	 */
};

#endif // LR_GL_SHADER_LIBRARY_H
