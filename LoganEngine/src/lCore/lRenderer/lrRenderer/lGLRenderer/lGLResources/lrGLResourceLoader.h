#ifndef LR_GL_RESOURCE_LOADER_H
#define LR_GL_RESOURCE_LOADER_H

#include "lrGLMesh/lrGLMeshResources/lrGLStaticMeshResource.h"
#include "lrGLMesh/lrGLMeshResources/lrGLSkeletalMeshResource.h"

#include "lrGLTexture/lrGLTextureResource.h"

#include "lrGLMesh/lrGLMeshViews/lrGLStaticMeshView.h"
#include "lrGLMesh/lrGLMeshViews/lrGLSkeletalMeshView.h"

#include "lrGLTexture/lrGLTextureViews/lrGLTexture2DView.h"
#include "lrGLTexture/lrGLTextureViews/lrGLTextureCubemapView.h"

#include <lResourceManager/liResourceManager.h>

#include <map>

class lrGLResourceLoader
{
private:
	liResourceManager *ResourceManager = nullptr;
	
	std::map<std::string,lrGLStaticMeshResource> StaticMeshes;
	std::map<std::string,lrGLStaticMeshResource> SkeletalMeshes;
	std::map<std::string,lrGLTextureResource> Textures;
	std::map<std::string,lrGLTextureResource> Cubemaps;
	
	lrmStaticMesh *LoadStaticMesh(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			return ResourceManager->GetStaticMesh(resource_id);
		}
		else
		{
			return nullptr;
		}
	}
	
	lrmStaticMesh *LoadSkeletalMesh(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			lrmSkeletalMesh *SkeletalMesh = ResourceManager->GetSkeletalMesh(resource_id);
			if(SkeletalMesh != nullptr)
			{
				return &SkeletalMesh->BindPoseMesh;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}
	
	liBitmap2D *LoadBitmap(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			return ResourceManager->GetBitmap(resource_id);
		}
		else
		{
			return nullptr;
		}
	}
	
	liCubemap *LoadCubemap(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			return ResourceManager->GetCubemap(resource_id);
		}
		else
		{
			return nullptr;
		}
	}
	
public:
	
	lrGLStaticMeshView GetStaticMesh(const std::string &resource_id)
	{
		auto I = StaticMeshes.find(resource_id);
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *LoadedMesh = LoadStaticMesh(resource_id);
			
			lrGLStaticMeshResource &StaticMeshResource = StaticMeshes[resource_id];
			if(LoadedMesh != nullptr)
			{
				StaticMeshResource.Initialize(*LoadedMesh);
			}
			
			return lrGLStaticMeshView(&StaticMeshes[resource_id]);
		}
		
		return lrGLStaticMeshView(&I->second);
	}
	
	lrGLStaticMeshView GetSkeletalMesh(const std::string &resource_id)
	{
		auto I = SkeletalMeshes.find(resource_id);
		if(I == SkeletalMeshes.end())
		{
			lrmStaticMesh *LoadedMesh = LoadSkeletalMesh(resource_id);
			
			lrGLStaticMeshResource &StaticMeshResource = SkeletalMeshes[resource_id];
			if(LoadedMesh != nullptr)
			{
				StaticMeshResource.Initialize(*LoadedMesh);
			}
			
			return lrGLStaticMeshView(&SkeletalMeshes[resource_id]);
		}
		
		return lrGLStaticMeshView(&I->second);
	}
	
	lrGLTexture2DView GetTexture(const std::string &resource_id);
	
	lrGLTextureCubemapView GetCubemap(const std::string &resource_id);
	
	void SetResourceManager(liResourceManager *resource_manager)
	{
		ResourceManager = resource_manager;
	}
	
	lrGLResourceLoader()
	{}
	
	~lrGLResourceLoader()
	{}
};

#endif // LR_GL_RESOURCE_LOADER_H
