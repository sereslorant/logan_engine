
#include "lGLExt.h"

namespace glExt
{

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

}

lGLExt GLExt;

extern "C" {

lGLExt *lGLGetGLExt() {return &GLExt;}
/*
void SetGLExtFunctions(lGLExt *gl_ext)
{
	glCreateProgram			= gl_ext->glCreateProgram;
	glDeleteProgram			= gl_ext->glDeleteProgram;
	glUseProgram			= gl_ext->glUseProgram;
	glAttachShader			= gl_ext->glAttachShader;
	glDetachShader			= gl_ext->glDetachShader;
	glLinkProgram			= gl_ext->glLinkProgram;
	glGetProgramiv			= gl_ext->glGetProgramiv;
	glGetShaderInfoLog 		= gl_ext->glGetShaderInfoLog;
	glGetUniformLocation	= gl_ext->glGetUniformLocation;
	glUniform1i				= gl_ext->glUniform1i;
	glUniform1iv			= gl_ext->glUniform1iv;
	glUniform2iv			= gl_ext->glUniform2iv;
	glUniform3iv			= gl_ext->glUniform3iv;
	glUniform4iv			= gl_ext->glUniform4iv;
	glUniform1f				= gl_ext->glUniform1f;
	glUniform1fv			= gl_ext->glUniform1fv;
	glUniform2fv			= gl_ext->glUniform2fv;
	glUniform3fv			= gl_ext->glUniform3fv;
	glUniform4fv			= gl_ext->glUniform4fv;
	glUniformMatrix3fv		= gl_ext->glUniformMatrix3fv;
	glUniformMatrix4fv 		= gl_ext->glUniformMatrix4fv;
	glGetAttribLocation 	= gl_ext->glGetAttribLocation;
	glVertexAttrib1f		= gl_ext->glVertexAttrib1f;
	glVertexAttrib1fv		= gl_ext->glVertexAttrib1fv;
	glVertexAttrib2fv		= gl_ext->glVertexAttrib2fv;
	glVertexAttrib3fv		= gl_ext->glVertexAttrib3fv;
	glVertexAttrib4fv		= gl_ext->glVertexAttrib4fv;
	glBindAttribLocation	= gl_ext->glBindAttribLocation;

	// Shader
	glCreateShader		= gl_ext->glCreateShader;
	glDeleteShader		= gl_ext->glDeleteShader;
	glShaderSource		= gl_ext->glShaderSource;
	glCompileShader		= gl_ext->glCompileShader;
	glGetShaderiv		= gl_ext->glGetShaderiv;

	// VBO
	glGenVertexArrays			= gl_ext->glGenVertexArrays;
	glBindVertexArray			= gl_ext->glBindVertexArray;
	glDeleteVertexArrays		= gl_ext->glDeleteVertexArrays;
	glGenBuffers				= gl_ext->glGenBuffers;
	glBindBuffer				= gl_ext->glBindBuffer;
	glBufferData				= gl_ext->glBufferData;
	glBufferSubData				= gl_ext->glBufferSubData;
	glDeleteBuffers				= gl_ext->glDeleteBuffers;
	glEnableVertexAttribArray	= gl_ext->glEnableVertexAttribArray;
	glDisableVertexAttribArray	= gl_ext->glDisableVertexAttribArray;
	glVertexAttribPointer		= gl_ext->glVertexAttribPointer;
	glVertexAttribIPointer		= gl_ext->glVertexAttribIPointer;

	//Framebuffer
	glGenFramebuffers			= gl_ext->glGenFramebuffers;
	glBindFramebuffer			= gl_ext->glBindFramebuffer;
	glDeleteFramebuffers		= gl_ext->glDeleteFramebuffers;
	glFramebufferTexture1D		= gl_ext->glFramebufferTexture1D;
	glFramebufferTexture2D		= gl_ext->glFramebufferTexture2D;
	glCheckFramebufferStatus	= gl_ext->glCheckFramebufferStatus;
}
*/
}
