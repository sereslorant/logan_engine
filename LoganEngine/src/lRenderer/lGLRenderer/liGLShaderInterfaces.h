#ifndef LI_GL_SHADER_INTERFACES_H
#define LI_GL_SHADER_INTERFACES_H

#include "lGLIncludes.h"

class liGLShader
{
public:
	/*
	 * Ezeket majd valszeg ki kéne szedni külön interfészbe
	 */
	virtual GLint GetLightPositionLocation() = 0;
	virtual GLint GetLightColorLocation() = 0;
	virtual GLint GetLightIntensityLocation() = 0;
	//
	virtual GLint GetCameraPositionLocation() = 0;
	//
	virtual GLint GetProjectionMatrixLocation() = 0;
	virtual GLint GetViewMatrixLocation() = 0;
	virtual GLint GetModelMatrixLocation() = 0;
	virtual GLint GetNormalMatrixLocation() = 0;
	virtual GLint GetMaterial0Location() = 0;
	virtual GLint GetMaterial1Location() = 0;
	//
	liGLShader()
	{}
	//
	virtual ~liGLShader()
	{}
	/*
	 * End of class
	 */
};

class liGLStaticMeshShader
{
public:
	virtual GLint GetVertexLocation() = 0;
	virtual GLint GetNormalLocation() = 0;
	virtual GLint GetTangentLocation() = 0;
	virtual GLint GetBitangentLocation() = 0;
	virtual GLint GetTexCoordLocation() = 0;
	//
	liGLStaticMeshShader()
	{}
	//
	virtual ~liGLStaticMeshShader()
	{}
	/*
	 * End of class
	 */
};

#endif // LI_GL_SHADER_INTERFACES_H
