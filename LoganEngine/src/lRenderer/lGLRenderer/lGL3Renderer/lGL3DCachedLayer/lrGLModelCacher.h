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
	virtual void IncrementMaterialGroups(const liMaterialLibrary &material_library,const std::string &model_name,liStaticMeshStats &static_mesh_stats) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(model_name);
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLMaterialGroupView MaterialGroup = StaticMesh.GetMaterialGroup(i);
			//
			std::string TextureGroupKey;
			std::string MaterialGroupKey;
			lrUtils::ComputeTextureGroupKey(material_library.GetMaterial(MaterialGroup.GetMaterial()),TextureGroupKey);
			lrUtils::ComputeMaterialGroupKey(model_name,MaterialGroup.GetMaterial(),MaterialGroupKey);
			//
			static_mesh_stats.Increment(TextureGroupKey,MaterialGroupKey);
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
