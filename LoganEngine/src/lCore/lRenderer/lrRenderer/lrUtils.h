#ifndef LFF_GL_UTILS_H
#define LFF_GL_UTILS_H

#include <lScene/l2DScene/li2DScene.h>
#include <lScene/l3DScene/li3DScene.h>

class lrUtils
{
public:
	static void GetCameraMatrix(const li2DCamera &camera,lmMatrix3x3 &dest)
	{
		lmOrtho3x3(dest,camera.GetPosition()[1],camera.GetPosition()[1] + camera.GetHeight(),camera.GetPosition()[0],camera.GetPosition()[0] + camera.GetWidth());
	}
	
	static void GetTransformMatrix(const li2DRectangle &rectangle,lmMatrix3x3 &dest)
	{
		lmTranslate3x3(dest,rectangle.GetPosition());
		lmScale3x3(dest,rectangle.GetWidth(),rectangle.GetHeight());
	}
	
	/*
	 * TODO: Ideiglenesen van csak itt, majd később kikerül külső függvénybe.
	 */
	static void GetProjectionMatrix(const liFrustum &frustum,lmMatrix4x4 &dest)
	{
		lmFrustum4x4(dest,	frustum.GetTop(),frustum.GetBottom(),
							frustum.GetLeft(),frustum.GetRight(),
							frustum.GetFar(),frustum.GetNear());
	}
	/*
	 * TODO: Ideiglenesen van csak itt, majd később kikerül külső függvénybe.
	 */
	static void GetViewMatrix(const li3DCamera &camera,lmMatrix4x4 &dest)
	{
		lmRotate_X4x4(dest,camera.GetPitch());
		lmRotate_Y4x4(dest,camera.GetYaw());
		lmTranslate4x4(dest,-1.0*camera.GetPosition());
	}
	/*
	 * TODO: Ideiglenesen van csak itt, majd később kikerül külső függvénybe.
	 */
	static void GetModelMatrix(const lmVector3D &position,const lmQuaternion &orientation,const lmVector3D &scale,lmMatrix4x4 &result)
	{
		lmTranslate4x4(result,position);
		lmRotate4x4(result,orientation);
		lmScale4x4(result,scale);
	}
	/*
	 * TODO: Ideiglenesen van csak itt, majd később kikerül külső függvénybe.
	 */
	static void GetModelMatrix(const li3DMesh &mesh,lmMatrix4x4 &result)
	{
		GetModelMatrix(mesh.GetPosition(),mesh.GetOrientation(),mesh.GetScale(),result);
	}

	static void ComputeTextureGroupKey(const liMaterial &material,std::string &dest)
	{
		dest = material.GetAlbedoMap();
	}

	static void ComputeMaterialGroupKey(const std::string &mesh_name,const std::string &mat_group_name,std::string &dest)
	{
		dest = mesh_name + "/" + mat_group_name;
	}
};

#endif
