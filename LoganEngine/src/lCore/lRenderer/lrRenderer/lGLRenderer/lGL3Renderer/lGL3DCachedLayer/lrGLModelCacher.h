#ifndef LR_GL_MODEL_CACHER_H
#define LR_GL_MODEL_CACHER_H

#include "../../../lr3DSceneReader.h"
#include "../../lGLResources/lrGLResourceLoader.h"

#include "../../../lrUtils.h"

class lrGLModelCacher : public liModelCacher
{
private:
	lrGLResourceLoader &ResourceLoader;
	//
public:
	//
	virtual void IncrementStaticMeshes(const liMaterialLibrary &material_library,const std::string &model_name,liMeshStats &mesh_stats) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(model_name);
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MaterialGroup = StaticMesh.GetMaterialGroup(i);
			//
			std::string TextureGroupKey;
			std::string MaterialGroupKey;
			lrUtils::ComputeTextureGroupKey(material_library.GetMaterial(MaterialGroup.GetMaterial()),TextureGroupKey);
			lrUtils::ComputeMaterialGroupKey(model_name,MaterialGroup.GetMaterial(),MaterialGroupKey);
			//
			mesh_stats.Increment(TextureGroupKey,MaterialGroupKey);
		}
	}
	//
	virtual void IncrementSkeletalMeshes(const liMaterialLibrary &material_library,const std::string &character_name,liMeshStats &mesh_stats) override
	{
		lrGLStaticMeshView SkeletalMesh = ResourceLoader.GetSkeletalMesh(character_name);
		for(unsigned int i=0;i < SkeletalMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MaterialGroup = SkeletalMesh.GetMaterialGroup(i);
			//
			std::string TextureGroupKey;
			std::string MaterialGroupKey;
			lrUtils::ComputeTextureGroupKey(material_library.GetMaterial(MaterialGroup.GetMaterial()),TextureGroupKey);
			lrUtils::ComputeMaterialGroupKey(character_name,MaterialGroup.GetMaterial(),MaterialGroupKey);
			//
			std::cout << MaterialGroupKey << std::endl;
			//
			mesh_stats.Increment(TextureGroupKey,MaterialGroupKey);
		}
	}
	//
	lrGLModelCacher(lrGLResourceLoader &resource_loader)
		:ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGLModelCacher() override
	{}
};

#endif // LR_GL_MODEL_CACHER_H
