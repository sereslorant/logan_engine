
#include "lSDL2GLExtLoader.h"

using namespace std;

void lSDL2GLExtLoader::PrintStatus(std::ostream &out)
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

void lSDL2GLExtLoader::LoadGLExtensions(lGLExt *GLExt)
{
	// Program
	GLExt->glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
	if (GLExt->glCreateProgram == nullptr)
	{ExtStatus["glCreateProgram"] = false;}
    else
    {ExtStatus["glCreateProgram"] = true;}

	GLExt->glDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress("glDeleteProgram");
	if (GLExt->glDeleteProgram == nullptr)
	{ExtStatus["glDeleteProgram"] = false;}
    else
    {ExtStatus["glDeleteProgram"] = true;}

	GLExt->glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
	if (GLExt->glUseProgram == nullptr)
	{ExtStatus["glUseProgram"] = false;}
    else
    {ExtStatus["glUseProgram"] = true;}

	GLExt->glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
	if (GLExt->glLinkProgram == nullptr)
	{ExtStatus["glLinkProgram"] = false;}
    else
    {ExtStatus["glLinkProgram"] = true;}

	GLExt->glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
	if (GLExt->glGetProgramiv == nullptr)
	{ExtStatus["glGetProgramiv"] = false;}
    else
    {ExtStatus["glGetProgramiv"] = true;}

	GLExt->glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	if (GLExt->glGetShaderInfoLog == nullptr)
	{ExtStatus["glGetShaderInfoLog"] = false;}
    else
    {ExtStatus["glGetShaderInfoLog"] = true;}

	GLExt->glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
	if (GLExt->glGetUniformLocation == nullptr)
	{ExtStatus["glGetUniformLocation"] = false;}
    else
    {ExtStatus["glGetUniformLocation"] = true;}

	GLExt->glUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
	if (GLExt->glUniform1i == nullptr)
	{ExtStatus["glUniform1i"] = false;}
    else
    {ExtStatus["glUniform1i"] = true;}

	GLExt->glUniform1iv = (PFNGLUNIFORM1IVPROC)SDL_GL_GetProcAddress("glUniform1iv");
	if (GLExt->glUniform1iv == nullptr)
	{ExtStatus["glUniform1iv"] = false;}
    else
    {ExtStatus["glUniform1iv"] = true;}

	GLExt->glUniform2iv = (PFNGLUNIFORM2IVPROC)SDL_GL_GetProcAddress("glUniform2iv");
	if (GLExt->glUniform2iv == nullptr)
	{ExtStatus["glUniform2iv"] = false;}
    else
    {ExtStatus["glUniform2iv"] = true;}

	GLExt->glUniform3iv = (PFNGLUNIFORM3IVPROC)SDL_GL_GetProcAddress("glUniform3iv");
	if (GLExt->glUniform3iv == nullptr)
	{ExtStatus["glUniform3iv"] = false;}
    else
    {ExtStatus["glUniform3iv"] = true;}

	GLExt->glUniform4iv = (PFNGLUNIFORM4IVPROC)SDL_GL_GetProcAddress("glUniform4iv");
	if (GLExt->glUniform4iv == nullptr)
	{ExtStatus["glUniform4iv"] = false;}
    else
    {ExtStatus["glUniform4iv"] = true;}

	GLExt->glUniform1f = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
	if (GLExt->glUniform1f == nullptr)
	{ExtStatus["glUniform1f"] = false;}
    else
    {ExtStatus["glUniform1f"] = true;}

	GLExt->glUniform1fv = (PFNGLUNIFORM1FVPROC)SDL_GL_GetProcAddress("glUniform1fv");
	if (GLExt->glUniform1fv == nullptr)
	{ExtStatus["glUniform1fv"] = false;}
    else
    {ExtStatus["glUniform1fv"] = true;}

	GLExt->glUniform2fv = (PFNGLUNIFORM2FVPROC)SDL_GL_GetProcAddress("glUniform2fv");
	if (GLExt->glUniform2fv == nullptr)
	{ExtStatus["glUniform2fv"] = false;}
    else
    {ExtStatus["glUniform2fv"] = true;}

	GLExt->glUniform3fv = (PFNGLUNIFORM3FVPROC)SDL_GL_GetProcAddress("glUniform3fv");
	if (GLExt->glUniform3fv == nullptr)
	{ExtStatus["glUniform3fv"] = false;}
    else
    {ExtStatus["glUniform3fv"] = true;}

	GLExt->glUniform4fv = (PFNGLUNIFORM4FVPROC)SDL_GL_GetProcAddress("glUniform4fv");
	if (GLExt->glUniform4fv == nullptr)
	{ExtStatus["glUniform4fv"] = false;}
    else
    {ExtStatus["glUniform4fv"] = true;}

	GLExt->glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)SDL_GL_GetProcAddress("glUniformMatrix3fv");
	if (GLExt->glUniformMatrix3fv == nullptr)
	{ExtStatus["glUniformMatrix3fv"] = false;}
    else
    {ExtStatus["glUniformMatrix3fv"] = true;}

	GLExt->glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)SDL_GL_GetProcAddress("glUniformMatrix4fv");
	if (GLExt->glUniformMatrix4fv == nullptr)
	{ExtStatus["glUniformMatrix4fv"] = false;}
    else
    {ExtStatus["glUniformMatrix4fv"] = true;}

	GLExt->glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glGetAttribLocation");
	if (GLExt->glGetAttribLocation == nullptr)
	{ExtStatus["glGetAttribLocation"] = false;}
    else
    {ExtStatus["glGetAttribLocation"] = true;}

	GLExt->glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)SDL_GL_GetProcAddress("glVertexAttrib1f");
	if (GLExt->glVertexAttrib1f == nullptr)
	{ExtStatus["glVertexAttrib1f"] = false;}
    else
    {ExtStatus["glVertexAttrib1f"] = true;}

	GLExt->glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)SDL_GL_GetProcAddress("glVertexAttrib1fv");
	if (GLExt->glVertexAttrib1fv == nullptr)
	{ExtStatus["glVertexAttrib1fv"] = false;}
    else
    {ExtStatus["glVertexAttrib1fv"] = true;}

	GLExt->glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)SDL_GL_GetProcAddress("glVertexAttrib2fv");
	if (GLExt->glVertexAttrib2fv == nullptr)
	{ExtStatus["glVertexAttrib2fv"] = false;}
    else
    {ExtStatus["glVertexAttrib2fv"] = true;}

	GLExt->glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)SDL_GL_GetProcAddress("glVertexAttrib3fv");
	if (GLExt->glVertexAttrib3fv == nullptr)
	{ExtStatus["glVertexAttrib3fv"] = false;}
    else
    {ExtStatus["glVertexAttrib3fv"] = true;}

	GLExt->glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)SDL_GL_GetProcAddress("glVertexAttrib4fv");
	if (GLExt->glVertexAttrib4fv == nullptr)
	{ExtStatus["glVertexAttrib4fv"] = false;}
    else
    {ExtStatus["glVertexAttrib4fv"] = true;}

	GLExt->glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)SDL_GL_GetProcAddress("glBindAttribLocation");
	if (GLExt->glBindAttribLocation == nullptr)
	{ExtStatus["glBindAttribLocation"] = false;}
    else
    {ExtStatus["glBindAttribLocation"] = true;}

	// Shader
	GLExt->glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
	if (GLExt->glCreateShader == nullptr)
	{ExtStatus["glCreateShader"] = false;}
    else
    {ExtStatus["glCreateShader"] = true;}

	GLExt->glDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
	if (GLExt->glDeleteShader == nullptr)
	{ExtStatus["glDeleteShader"] = false;}
    else
    {ExtStatus["glDeleteShader"] = true;}

	GLExt->glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
	if (GLExt->glShaderSource == nullptr)
	{ExtStatus["glShaderSource"] = false;}
    else
    {ExtStatus["glShaderSource"] = true;}

	GLExt->glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
	if (GLExt->glCompileShader == nullptr)
	{ExtStatus["glCompileShader"] = false;}
    else
    {ExtStatus["glCompileShader"] = true;}

	GLExt->glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
	if (GLExt->glAttachShader == nullptr)
	{ExtStatus["glAttachShader"] = false;}
    else
    {ExtStatus["glAttachShader"] = true;}

	GLExt->glDetachShader = (PFNGLDETACHSHADERPROC)SDL_GL_GetProcAddress("glDetachShader");
	if (GLExt->glDetachShader == nullptr)
	{ExtStatus["glDetachShader"] = false;}
    else
    {ExtStatus["glDetachShader"] = true;}

	GLExt->glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
	if (GLExt->glGetShaderiv == nullptr)
	{ExtStatus["glGetShaderiv"] = false;}
    else
    {ExtStatus["glGetShaderiv"] = true;}

	// VBO
	GLExt->glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glGenVertexArrays");
	if (GLExt->glGenVertexArrays == nullptr)
	{ExtStatus["glGenVertexArrays"] = false;}
    else
    {ExtStatus["glGenVertexArrays"] = true;}

	GLExt->glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)SDL_GL_GetProcAddress("glBindVertexArray");
	if (GLExt->glBindVertexArray == nullptr)
	{ExtStatus["glBindVertexArray"] = false;}
    else
    {ExtStatus["glBindVertexArray"] = true;}

	GLExt->glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)SDL_GL_GetProcAddress("glDeleteVertexArrays");
	if (GLExt->glDeleteVertexArrays == nullptr)
	{ExtStatus["glDeleteVertexArrays"] = false;}
    else
    {ExtStatus["glDeleteVertexArrays"] = true;}

	GLExt->glGenBuffers = (PFNGLGENBUFFERSPROC)SDL_GL_GetProcAddress("glGenBuffers");
	if (GLExt->glGenBuffers == nullptr)
	{ExtStatus["glGenBuffers"] = false;}
    else
    {ExtStatus["glGenBuffers"] = true;}

	GLExt->glBindBuffer = (PFNGLBINDBUFFERPROC)SDL_GL_GetProcAddress("glBindBuffer");
	if (GLExt->glBindBuffer == nullptr)
	{ExtStatus["glBindBuffer"] = false;}
    else
    {ExtStatus["glBindBuffer"] = true;}

	GLExt->glBufferData = (PFNGLBUFFERDATAPROC)SDL_GL_GetProcAddress("glBufferData");
	if (GLExt->glBufferData == nullptr)
	{ExtStatus["glBufferData"] = false;}
    else
    {ExtStatus["glBufferData"] = true;}

    GLExt->glBufferSubData = (PFNGLBUFFERSUBDATAPROC)SDL_GL_GetProcAddress("glBufferSubData");
	if (GLExt->glBufferData == nullptr)
	{ExtStatus["glBufferSubData"] = false;}
    else
    {ExtStatus["glBufferSubData"] = true;}

	GLExt->glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteBuffers");
	if (GLExt->glDeleteBuffers == nullptr)
	{ExtStatus["glDeleteBuffers"] = false;}
    else
    {ExtStatus["glDeleteBuffers"] = true;}

	GLExt->glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	if (GLExt->glEnableVertexAttribArray == nullptr)
	{ExtStatus["glEnableVertexAttribArray"] = false;}
    else
    {ExtStatus["glEnableVertexAttribArray"] = true;}

	GLExt->glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	if (GLExt->glDisableVertexAttribArray == nullptr)
	{ExtStatus["glDisableVertexAttribArray"] = false;}
    else
    {ExtStatus["glDisableVertexAttribArray"] = true;}

	GLExt->glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribPointer");
	if (GLExt->glVertexAttribPointer == nullptr)
	{ExtStatus["glVertexAttribPointer"] = false;}
    else
    {ExtStatus["glVertexAttribPointer"] = true;}

    GLExt->glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)SDL_GL_GetProcAddress("glVertexAttribIPointer");
	if (GLExt->glVertexAttribIPointer == nullptr)
	{ExtStatus["glVertexAttribIPointer"] = false;}
    else
    {ExtStatus["glVertexAttribIPointer"] = true;}

	//Framebuffer
	GLExt->glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
	if (GLExt->glGenFramebuffers == nullptr)
	{ExtStatus["glGenFramebuffers"] = false;}
    else
    {ExtStatus["glGenFramebuffers"] = true;}

	GLExt->glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
	if (GLExt->glBindFramebuffer == nullptr)
	{ExtStatus["glBindFramebuffer"] = false;}
    else
    {ExtStatus["glBindFramebuffer"] = true;}

	GLExt->glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glDeleteFramebuffers");
	if (GLExt->glDeleteFramebuffers == nullptr)
	{ExtStatus["glDeleteFramebuffers"] = false;}
    else
    {ExtStatus["glDeleteFramebuffers"] = true;}

	GLExt->glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture1D");
	if (GLExt->glFramebufferTexture1D == nullptr)
	{ExtStatus["glFramebufferTexture1D"] = false;}
    else
    {ExtStatus["glFramebufferTexture1D"] = true;}

	GLExt->glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture2D");
	if (GLExt->glFramebufferTexture2D == nullptr)
	{ExtStatus["glFramebufferTexture2D"] = false;}
    else
    {ExtStatus["glFramebufferTexture2D"] = true;}

	GLExt->glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)SDL_GL_GetProcAddress("glCheckFramebufferStatus");
	if (GLExt->glCheckFramebufferStatus == nullptr)
	{ExtStatus["glCheckFramebufferStatus"] = false;}
    else
    {ExtStatus["glCheckFramebufferStatus"] = true;}
}

bool lSDL2GLExtLoader::GetGLExtStatus(const std::string &Ext)
{
    auto I = ExtStatus.find(Ext);
    if(I != ExtStatus.end())
    {
        return I->second;
    }

    return false;
}

lSDL2GLExtLoader::lSDL2GLExtLoader()
{

}

lSDL2GLExtLoader::~lSDL2GLExtLoader()
{

}

