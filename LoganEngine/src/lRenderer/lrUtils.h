#ifndef LFF_GL_UTILS_H
#define LFF_GL_UTILS_H

class lrUtils
{
public:
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
	static void GetModelMatrix(const li3DMesh &mesh,lmMatrix4x4 &dest)
	{
		lmTranslate4x4(dest,mesh.GetPosition());
		lmRotate4x4(dest,mesh.GetOrientation());
		lmScale4x4(dest,mesh.GetScale());
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
