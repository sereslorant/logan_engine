#ifndef L_GL_EXT_H
#define L_GL_EXT_H

#include <GL/gl.h>

/** \file lGLExt.h
 * A loadolható OpenGL Extensionok függvénypointereit megosztó header.
 *
 * \author Seres Lóránt Gábor
 *
 */

namespace glExt
{

//Program
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1IVPROC glUniform1iv;
extern PFNGLUNIFORM2IVPROC glUniform2iv;
extern PFNGLUNIFORM3IVPROC glUniform3iv;
extern PFNGLUNIFORM4IVPROC glUniform4iv;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM1FVPROC glUniform1fv;
extern PFNGLUNIFORM2FVPROC glUniform2fv;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLUNIFORM4FVPROC glUniform4fv;
extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
extern PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;

// Shader
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;

// VBO
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;

//Framebuffer
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;

}

class lGLExt
{
public:
	//Program
	PFNGLCREATEPROGRAMPROC		&glCreateProgram		= glExt::glCreateProgram;
	PFNGLDELETEPROGRAMPROC		&glDeleteProgram		= glExt::glDeleteProgram;
	PFNGLUSEPROGRAMPROC			&glUseProgram			= glExt::glUseProgram;
	PFNGLATTACHSHADERPROC		&glAttachShader			= glExt::glAttachShader;
	PFNGLDETACHSHADERPROC		&glDetachShader			= glExt::glDetachShader;
	PFNGLLINKPROGRAMPROC		&glLinkProgram			= glExt::glLinkProgram;
	PFNGLGETPROGRAMIVPROC		&glGetProgramiv			= glExt::glGetProgramiv;
	PFNGLGETSHADERINFOLOGPROC	&glGetShaderInfoLog 	= glExt::glGetShaderInfoLog;
	PFNGLGETUNIFORMLOCATIONPROC	&glGetUniformLocation	= glExt::glGetUniformLocation;
	PFNGLUNIFORM1IPROC			&glUniform1i			= glExt::glUniform1i;
	PFNGLUNIFORM1IVPROC			&glUniform1iv			= glExt::glUniform1iv;
	PFNGLUNIFORM2IVPROC			&glUniform2iv			= glExt::glUniform2iv;
	PFNGLUNIFORM3IVPROC			&glUniform3iv			= glExt::glUniform3iv;
	PFNGLUNIFORM4IVPROC			&glUniform4iv			= glExt::glUniform4iv;
	PFNGLUNIFORM1FPROC			&glUniform1f			= glExt::glUniform1f;
	PFNGLUNIFORM1FVPROC			&glUniform1fv			= glExt::glUniform1fv;
	PFNGLUNIFORM2FVPROC			&glUniform2fv			= glExt::glUniform2fv;
	PFNGLUNIFORM3FVPROC			&glUniform3fv			= glExt::glUniform3fv;
	PFNGLUNIFORM4FVPROC			&glUniform4fv			= glExt::glUniform4fv;
	PFNGLUNIFORMMATRIX3FVPROC	&glUniformMatrix3fv		= glExt::glUniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC	&glUniformMatrix4fv 	= glExt::glUniformMatrix4fv;
	PFNGLGETATTRIBLOCATIONPROC	&glGetAttribLocation 	= glExt::glGetAttribLocation;
	PFNGLVERTEXATTRIB1FPROC		&glVertexAttrib1f		= glExt::glVertexAttrib1f;
	PFNGLVERTEXATTRIB1FVPROC	&glVertexAttrib1fv		= glExt::glVertexAttrib1fv;
	PFNGLVERTEXATTRIB2FVPROC	&glVertexAttrib2fv		= glExt::glVertexAttrib2fv;
	PFNGLVERTEXATTRIB3FVPROC	&glVertexAttrib3fv		= glExt::glVertexAttrib3fv;
	PFNGLVERTEXATTRIB4FVPROC	&glVertexAttrib4fv		= glExt::glVertexAttrib4fv;
	PFNGLBINDATTRIBLOCATIONPROC	&glBindAttribLocation	= glExt::glBindAttribLocation;

	// Shader
	PFNGLCREATESHADERPROC	&glCreateShader		= glExt::glCreateShader;
	PFNGLDELETESHADERPROC	&glDeleteShader		= glExt::glDeleteShader;
	PFNGLSHADERSOURCEPROC	&glShaderSource		= glExt::glShaderSource;
	PFNGLCOMPILESHADERPROC	&glCompileShader	= glExt::glCompileShader;
	PFNGLGETSHADERIVPROC 	&glGetShaderiv		= glExt::glGetShaderiv;

	// VBO
	PFNGLGENVERTEXARRAYSPROC			&glGenVertexArrays			= glExt::glGenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC			&glBindVertexArray			= glExt::glBindVertexArray;
	PFNGLDELETEVERTEXARRAYSPROC			&glDeleteVertexArrays		= glExt::glDeleteVertexArrays;
	PFNGLGENBUFFERSPROC					&glGenBuffers				= glExt::glGenBuffers;
	PFNGLBINDBUFFERPROC					&glBindBuffer				= glExt::glBindBuffer;
	PFNGLBUFFERDATAPROC					&glBufferData				= glExt::glBufferData;
	PFNGLBUFFERSUBDATAPROC				&glBufferSubData			= glExt::glBufferSubData;
	PFNGLDELETEBUFFERSPROC				&glDeleteBuffers			= glExt::glDeleteBuffers;
	PFNGLENABLEVERTEXATTRIBARRAYPROC	&glEnableVertexAttribArray	= glExt::glEnableVertexAttribArray;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC	&glDisableVertexAttribArray	= glExt::glDisableVertexAttribArray;
	PFNGLVERTEXATTRIBPOINTERPROC		&glVertexAttribPointer		= glExt::glVertexAttribPointer;
	PFNGLVERTEXATTRIBIPOINTERPROC		&glVertexAttribIPointer		= glExt::glVertexAttribIPointer;

	//Framebuffer
	PFNGLGENFRAMEBUFFERSPROC		&glGenFramebuffers			= glExt::glGenFramebuffers;
	PFNGLBINDFRAMEBUFFERPROC		&glBindFramebuffer			= glExt::glBindFramebuffer;
	PFNGLDELETEFRAMEBUFFERSPROC		&glDeleteFramebuffers		= glExt::glDeleteFramebuffers;
	PFNGLFRAMEBUFFERTEXTURE1DPROC	&glFramebufferTexture1D		= glExt::glFramebufferTexture1D;
	PFNGLFRAMEBUFFERTEXTURE2DPROC	&glFramebufferTexture2D		= glExt::glFramebufferTexture2D;
	PFNGLCHECKFRAMEBUFFERSTATUSPROC	&glCheckFramebufferStatus	= glExt::glCheckFramebufferStatus;

	lGLExt(){}
	~lGLExt(){}
};

#endif
