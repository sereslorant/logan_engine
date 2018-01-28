
#include "lGLExt.h"

//Program
PFNGLCREATEPROGRAMPROC glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC glDeleteProgram = nullptr;
PFNGLUSEPROGRAMPROC glUseProgram = nullptr;
PFNGLATTACHSHADERPROC glAttachShader = nullptr;
PFNGLDETACHSHADERPROC glDetachShader = nullptr;
PFNGLLINKPROGRAMPROC glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = nullptr;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1IPROC glUniform1i = nullptr;
PFNGLUNIFORM1IVPROC glUniform1iv = nullptr;
PFNGLUNIFORM2IVPROC glUniform2iv = nullptr;
PFNGLUNIFORM3IVPROC glUniform3iv = nullptr;
PFNGLUNIFORM4IVPROC glUniform4iv = nullptr;
PFNGLUNIFORM1FPROC glUniform1f = nullptr;
PFNGLUNIFORM1FVPROC glUniform1fv = nullptr;
PFNGLUNIFORM2FVPROC glUniform2fv = nullptr;
PFNGLUNIFORM3FVPROC glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = nullptr;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = nullptr;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f = nullptr;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv = nullptr;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv = nullptr;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv = nullptr;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = nullptr;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = nullptr;

// Shader
PFNGLCREATESHADERPROC glCreateShader = nullptr;
PFNGLDELETESHADERPROC glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC glCompileShader = nullptr;
PFNGLGETSHADERIVPROC glGetShaderiv = nullptr;

// VBO
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLGENBUFFERSPROC glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
PFNGLMAPBUFFERPROC glMapBuffer = nullptr;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = nullptr;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = nullptr;

//Framebuffer
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = nullptr;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = nullptr;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = nullptr;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D = nullptr;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = nullptr;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = nullptr;

#include <map>

class lGLExtLoader : public liGLExtLoader
{
private:

	liExtFunctionLoader *FunctionLoader = nullptr;
	std::map<std::string,bool> ExtStatus;

	static lGLExtLoader Instance;

	lGLExtLoader()
	{}

public:

	virtual void SetExtFunctionLoader(liExtFunctionLoader *function_loader) override
	{
		FunctionLoader = function_loader;
	}

	virtual void LoadExtensions() override
	{
		// Program
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)FunctionLoader->LoadFunction("glCreateProgram");
		if (glCreateProgram == nullptr)
		{ExtStatus["glCreateProgram"] = false;}
		else
		{ExtStatus["glCreateProgram"] = true;}

		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)FunctionLoader->LoadFunction("glDeleteProgram");
		if (glDeleteProgram == nullptr)
		{ExtStatus["glDeleteProgram"] = false;}
		else
		{ExtStatus["glDeleteProgram"] = true;}

		glUseProgram = (PFNGLUSEPROGRAMPROC)FunctionLoader->LoadFunction("glUseProgram");
		if (glUseProgram == nullptr)
		{ExtStatus["glUseProgram"] = false;}
		else
		{ExtStatus["glUseProgram"] = true;}

		glLinkProgram = (PFNGLLINKPROGRAMPROC)FunctionLoader->LoadFunction("glLinkProgram");
		if (glLinkProgram == nullptr)
		{ExtStatus["glLinkProgram"] = false;}
		else
		{ExtStatus["glLinkProgram"] = true;}

		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)FunctionLoader->LoadFunction("glGetProgramiv");
		if (glGetProgramiv == nullptr)
		{ExtStatus["glGetProgramiv"] = false;}
		else
		{ExtStatus["glGetProgramiv"] = true;}

		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)FunctionLoader->LoadFunction("glGetShaderInfoLog");
		if (glGetShaderInfoLog == nullptr)
		{ExtStatus["glGetShaderInfoLog"] = false;}
		else
		{ExtStatus["glGetShaderInfoLog"] = true;}

		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)FunctionLoader->LoadFunction("glGetUniformLocation");
		if (glGetUniformLocation == nullptr)
		{ExtStatus["glGetUniformLocation"] = false;}
		else
		{ExtStatus["glGetUniformLocation"] = true;}

		glUniform1i = (PFNGLUNIFORM1IPROC)FunctionLoader->LoadFunction("glUniform1i");
		if (glUniform1i == nullptr)
		{ExtStatus["glUniform1i"] = false;}
		else
		{ExtStatus["glUniform1i"] = true;}

		glUniform1iv = (PFNGLUNIFORM1IVPROC)FunctionLoader->LoadFunction("glUniform1iv");
		if (glUniform1iv == nullptr)
		{ExtStatus["glUniform1iv"] = false;}
		else
		{ExtStatus["glUniform1iv"] = true;}

		glUniform2iv = (PFNGLUNIFORM2IVPROC)FunctionLoader->LoadFunction("glUniform2iv");
		if (glUniform2iv == nullptr)
		{ExtStatus["glUniform2iv"] = false;}
		else
		{ExtStatus["glUniform2iv"] = true;}

		glUniform3iv = (PFNGLUNIFORM3IVPROC)FunctionLoader->LoadFunction("glUniform3iv");
		if (glUniform3iv == nullptr)
		{ExtStatus["glUniform3iv"] = false;}
		else
		{ExtStatus["glUniform3iv"] = true;}

		glUniform4iv = (PFNGLUNIFORM4IVPROC)FunctionLoader->LoadFunction("glUniform4iv");
		if (glUniform4iv == nullptr)
		{ExtStatus["glUniform4iv"] = false;}
		else
		{ExtStatus["glUniform4iv"] = true;}

		glUniform1f = (PFNGLUNIFORM1FPROC)FunctionLoader->LoadFunction("glUniform1f");
		if (glUniform1f == nullptr)
		{ExtStatus["glUniform1f"] = false;}
		else
		{ExtStatus["glUniform1f"] = true;}

		glUniform1fv = (PFNGLUNIFORM1FVPROC)FunctionLoader->LoadFunction("glUniform1fv");
		if (glUniform1fv == nullptr)
		{ExtStatus["glUniform1fv"] = false;}
		else
		{ExtStatus["glUniform1fv"] = true;}

		glUniform2fv = (PFNGLUNIFORM2FVPROC)FunctionLoader->LoadFunction("glUniform2fv");
		if (glUniform2fv == nullptr)
		{ExtStatus["glUniform2fv"] = false;}
		else
		{ExtStatus["glUniform2fv"] = true;}

		glUniform3fv = (PFNGLUNIFORM3FVPROC)FunctionLoader->LoadFunction("glUniform3fv");
		if (glUniform3fv == nullptr)
		{ExtStatus["glUniform3fv"] = false;}
		else
		{ExtStatus["glUniform3fv"] = true;}

		glUniform4fv = (PFNGLUNIFORM4FVPROC)FunctionLoader->LoadFunction("glUniform4fv");
		if (glUniform4fv == nullptr)
		{ExtStatus["glUniform4fv"] = false;}
		else
		{ExtStatus["glUniform4fv"] = true;}

		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)FunctionLoader->LoadFunction("glUniformMatrix3fv");
		if (glUniformMatrix3fv == nullptr)
		{ExtStatus["glUniformMatrix3fv"] = false;}
		else
		{ExtStatus["glUniformMatrix3fv"] = true;}

		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)FunctionLoader->LoadFunction("glUniformMatrix4fv");
		if (glUniformMatrix4fv == nullptr)
		{ExtStatus["glUniformMatrix4fv"] = false;}
		else
		{ExtStatus["glUniformMatrix4fv"] = true;}

		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)FunctionLoader->LoadFunction("glGetAttribLocation");
		if (glGetAttribLocation == nullptr)
		{ExtStatus["glGetAttribLocation"] = false;}
		else
		{ExtStatus["glGetAttribLocation"] = true;}

		glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)FunctionLoader->LoadFunction("glVertexAttrib1f");
		if (glVertexAttrib1f == nullptr)
		{ExtStatus["glVertexAttrib1f"] = false;}
		else
		{ExtStatus["glVertexAttrib1f"] = true;}

		glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)FunctionLoader->LoadFunction("glVertexAttrib1fv");
		if (glVertexAttrib1fv == nullptr)
		{ExtStatus["glVertexAttrib1fv"] = false;}
		else
		{ExtStatus["glVertexAttrib1fv"] = true;}

		glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)FunctionLoader->LoadFunction("glVertexAttrib2fv");
		if (glVertexAttrib2fv == nullptr)
		{ExtStatus["glVertexAttrib2fv"] = false;}
		else
		{ExtStatus["glVertexAttrib2fv"] = true;}

		glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)FunctionLoader->LoadFunction("glVertexAttrib3fv");
		if (glVertexAttrib3fv == nullptr)
		{ExtStatus["glVertexAttrib3fv"] = false;}
		else
		{ExtStatus["glVertexAttrib3fv"] = true;}

		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)FunctionLoader->LoadFunction("glVertexAttrib4fv");
		if (glVertexAttrib4fv == nullptr)
		{ExtStatus["glVertexAttrib4fv"] = false;}
		else
		{ExtStatus["glVertexAttrib4fv"] = true;}

		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)FunctionLoader->LoadFunction("glBindAttribLocation");
		if (glBindAttribLocation == nullptr)
		{ExtStatus["glBindAttribLocation"] = false;}
		else
		{ExtStatus["glBindAttribLocation"] = true;}

		// Shader
		glCreateShader = (PFNGLCREATESHADERPROC)FunctionLoader->LoadFunction("glCreateShader");
		if (glCreateShader == nullptr)
		{ExtStatus["glCreateShader"] = false;}
		else
		{ExtStatus["glCreateShader"] = true;}

		glDeleteShader = (PFNGLDELETESHADERPROC)FunctionLoader->LoadFunction("glDeleteShader");
		if (glDeleteShader == nullptr)
		{ExtStatus["glDeleteShader"] = false;}
		else
		{ExtStatus["glDeleteShader"] = true;}

		glShaderSource = (PFNGLSHADERSOURCEPROC)FunctionLoader->LoadFunction("glShaderSource");
		if (glShaderSource == nullptr)
		{ExtStatus["glShaderSource"] = false;}
		else
		{ExtStatus["glShaderSource"] = true;}

		glCompileShader = (PFNGLCOMPILESHADERPROC)FunctionLoader->LoadFunction("glCompileShader");
		if (glCompileShader == nullptr)
		{ExtStatus["glCompileShader"] = false;}
		else
		{ExtStatus["glCompileShader"] = true;}

		glAttachShader = (PFNGLATTACHSHADERPROC)FunctionLoader->LoadFunction("glAttachShader");
		if (glAttachShader == nullptr)
		{ExtStatus["glAttachShader"] = false;}
		else
		{ExtStatus["glAttachShader"] = true;}

		glDetachShader = (PFNGLDETACHSHADERPROC)FunctionLoader->LoadFunction("glDetachShader");
		if (glDetachShader == nullptr)
		{ExtStatus["glDetachShader"] = false;}
		else
		{ExtStatus["glDetachShader"] = true;}

		glGetShaderiv = (PFNGLGETSHADERIVPROC)FunctionLoader->LoadFunction("glGetShaderiv");
		if (glGetShaderiv == nullptr)
		{ExtStatus["glGetShaderiv"] = false;}
		else
		{ExtStatus["glGetShaderiv"] = true;}

		// VBO
		glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)FunctionLoader->LoadFunction("glGenVertexArrays");
		if (glGenVertexArrays == nullptr)
		{ExtStatus["glGenVertexArrays"] = false;}
		else
		{ExtStatus["glGenVertexArrays"] = true;}

		glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)FunctionLoader->LoadFunction("glBindVertexArray");
		if (glBindVertexArray == nullptr)
		{ExtStatus["glBindVertexArray"] = false;}
		else
		{ExtStatus["glBindVertexArray"] = true;}

		glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)FunctionLoader->LoadFunction("glDeleteVertexArrays");
		if (glDeleteVertexArrays == nullptr)
		{ExtStatus["glDeleteVertexArrays"] = false;}
		else
		{ExtStatus["glDeleteVertexArrays"] = true;}

		glGenBuffers = (PFNGLGENBUFFERSPROC)FunctionLoader->LoadFunction("glGenBuffers");
		if (glGenBuffers == nullptr)
		{ExtStatus["glGenBuffers"] = false;}
		else
		{ExtStatus["glGenBuffers"] = true;}

		glBindBuffer = (PFNGLBINDBUFFERPROC)FunctionLoader->LoadFunction("glBindBuffer");
		if (glBindBuffer == nullptr)
		{ExtStatus["glBindBuffer"] = false;}
		else
		{ExtStatus["glBindBuffer"] = true;}

		glBufferData = (PFNGLBUFFERDATAPROC)FunctionLoader->LoadFunction("glBufferData");
		if (glBufferData == nullptr)
		{ExtStatus["glBufferData"] = false;}
		else
		{ExtStatus["glBufferData"] = true;}

		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)FunctionLoader->LoadFunction("glBufferSubData");
		if (glBufferData == nullptr)
		{ExtStatus["glBufferSubData"] = false;}
		else
		{ExtStatus["glBufferSubData"] = true;}

		glMapBuffer = (PFNGLMAPBUFFERPROC)FunctionLoader->LoadFunction("glMapBuffer");
		if (glMapBuffer == nullptr)
		{ExtStatus["glMapBuffer"] = false;}
		else
		{ExtStatus["glMapBuffer"] = true;}

		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)FunctionLoader->LoadFunction("glUnmapBuffer");
		if (glUnmapBuffer == nullptr)
		{ExtStatus["glUnmapBuffer"] = false;}
		else
		{ExtStatus["glUnmapBuffer"] = true;}

		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)FunctionLoader->LoadFunction("glDeleteBuffers");
		if (glDeleteBuffers == nullptr)
		{ExtStatus["glDeleteBuffers"] = false;}
		else
		{ExtStatus["glDeleteBuffers"] = true;}

		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)FunctionLoader->LoadFunction("glEnableVertexAttribArray");
		if (glEnableVertexAttribArray == nullptr)
		{ExtStatus["glEnableVertexAttribArray"] = false;}
		else
		{ExtStatus["glEnableVertexAttribArray"] = true;}

		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)FunctionLoader->LoadFunction("glDisableVertexAttribArray");
		if (glDisableVertexAttribArray == nullptr)
		{ExtStatus["glDisableVertexAttribArray"] = false;}
		else
		{ExtStatus["glDisableVertexAttribArray"] = true;}

		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)FunctionLoader->LoadFunction("glVertexAttribPointer");
		if (glVertexAttribPointer == nullptr)
		{ExtStatus["glVertexAttribPointer"] = false;}
		else
		{ExtStatus["glVertexAttribPointer"] = true;}

		glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)FunctionLoader->LoadFunction("glVertexAttribIPointer");
		if (glVertexAttribIPointer == nullptr)
		{ExtStatus["glVertexAttribIPointer"] = false;}
		else
		{ExtStatus["glVertexAttribIPointer"] = true;}

		//Framebuffer
		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)FunctionLoader->LoadFunction("glGenFramebuffers");
		if (glGenFramebuffers == nullptr)
		{ExtStatus["glGenFramebuffers"] = false;}
		else
		{ExtStatus["glGenFramebuffers"] = true;}

		glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)FunctionLoader->LoadFunction("glBindFramebuffer");
		if (glBindFramebuffer == nullptr)
		{ExtStatus["glBindFramebuffer"] = false;}
		else
		{ExtStatus["glBindFramebuffer"] = true;}

		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)FunctionLoader->LoadFunction("glDeleteFramebuffers");
		if (glDeleteFramebuffers == nullptr)
		{ExtStatus["glDeleteFramebuffers"] = false;}
		else
		{ExtStatus["glDeleteFramebuffers"] = true;}

		glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)FunctionLoader->LoadFunction("glFramebufferTexture1D");
		if (glFramebufferTexture1D == nullptr)
		{ExtStatus["glFramebufferTexture1D"] = false;}
		else
		{ExtStatus["glFramebufferTexture1D"] = true;}

		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)FunctionLoader->LoadFunction("glFramebufferTexture2D");
		if (glFramebufferTexture2D == nullptr)
		{ExtStatus["glFramebufferTexture2D"] = false;}
		else
		{ExtStatus["glFramebufferTexture2D"] = true;}

		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)FunctionLoader->LoadFunction("glCheckFramebufferStatus");
		if (glCheckFramebufferStatus == nullptr)
		{ExtStatus["glCheckFramebufferStatus"] = false;}
		else
		{ExtStatus["glCheckFramebufferStatus"] = true;}
	}

	virtual void PrintExtStatus(std::ostream &out) override
	{
		for(auto i = ExtStatus.begin();i!= ExtStatus.end();i++)
		{
			if(i->second)
			{
				out << "Extension successfully loaded: " << i->first << "\n";
			}
			else
			{
				out << "Couldn't load extension " << i->first << "\n";
			}
		}
	}

	static lGLExtLoader &GetInstance()
	{
		return Instance;
	}

	virtual ~lGLExtLoader() override
	{

	}
};

lGLExtLoader lGLExtLoader::Instance;

liGLExtLoader &lGetGLExtLoader()
{
	return lGLExtLoader::GetInstance();
}
