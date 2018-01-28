#ifndef LR_GL3_SCENE_CACHER_H
#define LR_GL3_SCENE_CACHER_H

#include "lrGL3LightCacher.h"
#include "lrGL3InstanceCacher.h"

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGL3SceneCache.h>

class lrGL3SceneCacher : public li3DSceneCache
{
private:
	lrGLResourceLoader &ResourceLoader;
	liGLStaticMeshShader &StaticMeshShader;
	
	lrGL3SceneCache &SceneCache;
	
	lrGL3LightCacher LightCacher;
	std::map<std::string,lrGL3InstanceCacher> StaticMeshInstanceCachers;
	std::map<std::string,lrGL3InstanceCacher> SkeletalMeshInstanceCachers;
	
	lrGL3InstanceCacher &GetStaticMeshInstanceCacher(lrGLStaticMaterialGroupView &MtlGroup,const std::string &texture_group_name,const std::string &material_group_name)
	{
		std::string InstanceCacherKey = texture_group_name + ";" + material_group_name;
		lrGL3InstanceCacher &InstanceCacher = StaticMeshInstanceCachers[InstanceCacherKey];
		
		if(!InstanceCacher.Initialized())
		{
			lrGL3StaticInstanceSet &MeshInstances = SceneCache.GetStaticMeshInstanceSet(texture_group_name,material_group_name);
			MeshInstances.Construct(StaticMeshShader,MtlGroup);
			
			InstanceCacher.Initialize(&SceneCache.GetProjectionMatrix(),&SceneCache.GetViewMatrix(),&MeshInstances);
		}
		
		return InstanceCacher;
	}
	
	lrGL3InstanceCacher &GetSkeletalMeshInstanceCacher(lrGLStaticMaterialGroupView &MtlGroup,const std::string &texture_group_name,const std::string &material_group_name)
	{
		std::string InstanceCacherKey = texture_group_name + ";" + material_group_name;
		lrGL3InstanceCacher &InstanceCacher = SkeletalMeshInstanceCachers[InstanceCacherKey];
		
		if(!InstanceCacher.Initialized())
		{
			lrGL3StaticInstanceSet &MeshInstances = SceneCache.GetSkeletalMeshInstanceSet(texture_group_name,material_group_name);
			MeshInstances.Construct(StaticMeshShader,MtlGroup);
			
			InstanceCacher.Initialize(&SceneCache.GetProjectionMatrix(),&SceneCache.GetViewMatrix(),&MeshInstances);
		}
		
		return InstanceCacher;
	}
	
public:
	
	virtual void CacheStaticMesh(const li3DMesh &mesh,const li3DStaticMesh &static_mesh) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
		const liMaterialLibrary &MaterialLibrary = mesh.GetMaterialLibrary();
		
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MtlGroup = StaticMesh.GetMaterialGroup(i);
			
			const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
			
			std::string TextureGroupName;
			std::string MaterialGroupName;
			lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
			lrUtils::ComputeMaterialGroupKey(static_mesh.GetModelName(),MtlGroup.GetMaterial(),MaterialGroupName);
			
			lrGL3InstanceCacher &MeshInstances = GetStaticMeshInstanceCacher(MtlGroup,TextureGroupName,MaterialGroupName);
			
			MeshInstances.SetNextInstance(Material,mesh);
		}
	}
	
	virtual void CacheSkeletalMesh(const li3DMesh &mesh,const li3DSkeletalMesh &skeletal_mesh) override
	{
		lrGLStaticMeshView SkeletalMesh = ResourceLoader.GetSkeletalMesh(skeletal_mesh.GetCharacterName());
		const liMaterialLibrary &MaterialLibrary = mesh.GetMaterialLibrary();
		
		for(unsigned int i=0;i < SkeletalMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MtlGroup = SkeletalMesh.GetMaterialGroup(i);
			
			const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
			
			std::string TextureGroupName;
			std::string MaterialGroupName;
			lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
			lrUtils::ComputeMaterialGroupKey(skeletal_mesh.GetCharacterName(),MtlGroup.GetMaterial(),MaterialGroupName);
			
			lrGL3InstanceCacher &MeshInstances = GetSkeletalMeshInstanceCacher(MtlGroup,TextureGroupName,MaterialGroupName);
			
			MeshInstances.SetNextInstance(Material,mesh);
		}
	}
	
	virtual void CacheLight(const li3DLight &light) override
	{
		LightCacher.SetNextLight(light);
	}
	
	lrGL3SceneCacher(lrGLResourceLoader &resource_loader,liGLStaticMeshShader &static_mesh_shader,lrGL3SceneCache &scene_cache)
		:ResourceLoader(resource_loader),StaticMeshShader(static_mesh_shader),SceneCache(scene_cache)
	{
		LightCacher.Initialize(&scene_cache.GetLights());
	}
	
	virtual ~lrGL3SceneCacher() override
	{
		
	}
	
	/*
	 * End of class
	 */
};

#endif // LR_GL3_SCENE_CACHER_H
