#ifndef LR_GL_TEXTURE_LIST_H
#define LR_GL_TEXTURE_LIST_H

#include "lrGL3InstanceSet.h"

#include <vector>
#include <map>

class lrGL3TextureList
{
private:
	std::string AlbedoMapName;
	lrGLTexture2DView AlbedoMap;
	//
	std::string EnvironmentMapName;
	lrGLTextureCubemapView EnvironmentMap;
	//
	std::vector<lrGL3InstanceSet> StaticMeshInstanceSets;
	std::vector<lrGL3InstanceSet> SkeletalMeshInstanceSets;
	std::map<std::string,lrGL3InstanceSet *> StaticMeshModelDictionary;
	std::map<std::string,lrGL3InstanceSet *> SkeletalMeshModelDictionary;
	//
public:
	//
	void DrawTextureList(liGLShader &shader,liGLPbMatShader &mat_shader)
	{
		for(lrGL3InstanceSet &InstanceSet : StaticMeshInstanceSets)
		{
			InstanceSet.DrawInstances(shader,mat_shader);
		}
		//
		for(lrGL3InstanceSet &InstanceSet : SkeletalMeshInstanceSets)
		{
			InstanceSet.DrawInstances(shader,mat_shader);
		}
	}
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	//
	void Print()
	{
		for(auto &J : ModelDictionary)
		{
			std::cout << "\tMesh: " << J.first << "\n";
			lrGL3MeshInstances &MeshInstances = *J.second;
			MeshInstances.Print();
		}
	}
	//
	#endif
	//
	const std::string &GetAlbedoMapName()
	{
		return AlbedoMapName;
	}
	//
	lrGLTexture2DView &GetAlbedoMap()
	{
		return AlbedoMap;
	}
	//
	const std::string &GetEnvironmentMapName()
	{
		return EnvironmentMapName;
	}
	//
	lrGLTextureCubemapView &GetEnvironmentMap()
	{
		return EnvironmentMap;
	}
	//
	void Construct(const std::string &albedo_map_name,lrGLTexture2DView albedo_map,
			const std::string &environment_map_name,lrGLTextureCubemapView environment_map,
			unsigned int num_static_mesh_material_groups,unsigned int num_skeletal_mesh_material_groups)
	{
		AlbedoMapName = albedo_map_name;
		AlbedoMap = albedo_map;
		//
		EnvironmentMapName = environment_map_name;
		EnvironmentMap = environment_map;
		//
		StaticMeshInstanceSets.resize(num_static_mesh_material_groups);
		SkeletalMeshInstanceSets.resize(num_skeletal_mesh_material_groups);
	}
	//
	lrGL3InstanceSet &GetStaticMeshInstanceSet(const std::string &material_group_key)
	{
		return *StaticMeshModelDictionary[material_group_key];
	}
	//
	lrGL3InstanceSet &GetSkeletalMeshInstanceSet(const std::string &material_group_key)
	{
		return *SkeletalMeshModelDictionary[material_group_key];
	}
	//
	void InitializeStaticMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lrGL3InstanceSet &MatGroupInstances = StaticMeshInstanceSets[mat_group_id];
		//
		MatGroupInstances.Resize(num_instances);
		StaticMeshModelDictionary[material_group_name] = &MatGroupInstances;
	}
	//
	void InitializeSkeletalMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lrGL3InstanceSet &MatGroupInstances = SkeletalMeshInstanceSets[mat_group_id];
		//
		MatGroupInstances.Resize(num_instances);
		SkeletalMeshModelDictionary[material_group_name] = &MatGroupInstances;
	}
};


#endif // LR_GL_TEXTURE_LIST_H
