#ifndef LI_GL_SHADER_INTERFACES_H
#define LI_GL_SHADER_INTERFACES_H

#include "lGLIncludes.h"

/*
 * 2D shader interfaces
 */

class liGL2DShader
{
public:
	virtual GLint GetCameraLocation() = 0;
	virtual GLint GetTransformLocation() = 0;
	
	virtual GLint GetVertexLocation() = 0;
	
	liGL2DShader()
	{}
	virtual ~liGL2DShader()
	{}
	/*
	 * End of class
	 */
};

class liGL2DSolidColorShader
{
public:
	virtual GLint GetColorLocation() = 0;
	
	liGL2DSolidColorShader()
	{}
	virtual ~liGL2DSolidColorShader()
	{}
	/*
	 * End of class
	 */
};

/*
 * 3D shader interfaces
 */

class liGL3DShader
{
public:
	virtual GLint GetCameraPositionLocation() = 0;
	
	virtual GLint GetProjectionMatrixLocation() = 0;
	virtual GLint GetViewMatrixLocation() = 0;
	virtual GLint GetModelMatrixLocation() = 0;
	virtual GLint GetNormalMatrixLocation() = 0;
	virtual GLint GetMvpMatrixLocation() = 0;
	
	liGL3DShader()
	{}
	virtual ~liGL3DShader()
	{}
	/*
	 * End of class
	 */
};

class liGLPbMatShader
{
public:
	virtual GLint GetAlbedoMapLocation() = 0;
	
	// TODO: Másik interfészben van a helye!
	virtual GLint GetEnvironmentMapLocation() = 0;
	
	virtual GLint GetMatAlbedoLocation() = 0;
	virtual GLint GetMaterial1Location() = 0;
	
	liGLPbMatShader()
	{}
	virtual ~liGLPbMatShader()
	{}
	/*
	 * End of class
	 */
};

class liGLPointLightShader
{
public:
	virtual GLint GetLightCountLocation() = 0;
	
	virtual GLint GetLightPositionLocation() = 0;
	virtual GLint GetLightColorLocation() = 0;
	virtual GLint GetLightIntensityLocation() = 0;
	
	liGLPointLightShader()
	{}
	virtual ~liGLPointLightShader()
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
	
	liGLStaticMeshShader()
	{}
	virtual ~liGLStaticMeshShader()
	{}
	/*
	 * End of class
	 */
};

class liGLSkeletalMeshShader
{
public:
	virtual GLint GetBoneIdLocation() = 0;
	virtual GLint GetWeightLocation() = 0;
	
	// TODO: Másik interfészben van a helye!
	virtual GLint GetBoneTransformLocation() = 0;
	
	liGLSkeletalMeshShader()
	{}
	virtual ~liGLSkeletalMeshShader()
	{}
};

#endif // LI_GL_SHADER_INTERFACES_H
