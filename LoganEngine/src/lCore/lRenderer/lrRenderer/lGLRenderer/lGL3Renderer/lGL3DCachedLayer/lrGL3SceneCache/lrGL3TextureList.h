#ifndef LR_GL_TEXTURE_LIST_H
#define LR_GL_TEXTURE_LIST_H

#include "lrGL3InstanceSet.h"

#include <vector>
#include <map>

#include "lrGLData/lrGLLightArray.h"

class lrGL3TextureList
{
private:
	std::string AlbedoMapName;
	lrGLTexture2DView AlbedoMap;
	//
	std::string EnvironmentMapName;
	lrGLTextureCubemapView EnvironmentMap;
	//
	std::vector<lrGL3StaticInstanceSet> StaticMeshInstanceSets;
	std::vector<lrGL3StaticInstanceSet> SkeletalMeshInstanceSets;
	std::map<std::string,lrGL3StaticInstanceSet *> StaticMeshModelDictionary;
	std::map<std::string,lrGL3StaticInstanceSet *> SkeletalMeshModelDictionary;
	
public:
	
	void DrawTextureList(liGL3DShader &shader,liGLPbMatShader &mat_shader)
	{
		for(lrGL3StaticInstanceSet &InstanceSet : StaticMeshInstanceSets)
		{
			InstanceSet.Bind();
			InstanceSet.DrawInstances(shader,mat_shader);
		}
		
		for(lrGL3StaticInstanceSet &InstanceSet : SkeletalMeshInstanceSets)
		{
			InstanceSet.Bind();
			InstanceSet.DrawInstances(shader,mat_shader);
		}
	}
	
	void ForeachLightDrawTextureList(liGL3DShader &shader,liGLPbMatShader &mat_shader,liGLPointLightShader &point_light_shader,lrGLLightArray &lights)
	{
		constexpr unsigned int MAX_LIGHT_COUNT = 256;
		
		unsigned int NumPasses = lights.NumLights() / MAX_LIGHT_COUNT;
		if(lights.NumLights() % MAX_LIGHT_COUNT > 0)
			{NumPasses += 1;}
		
		for(lrGL3StaticInstanceSet &InstanceSet : StaticMeshInstanceSets)
		{
			InstanceSet.Bind();
			for(int i=0;i < NumPasses;i++)
			{
				unsigned int Size = std::min<std::size_t>(lights.NumLights() - i*MAX_LIGHT_COUNT,MAX_LIGHT_COUNT);
				lrGLRenderUtils::UploadUniformLightArray(point_light_shader,
														 lights.GetPositionBegin(i*MAX_LIGHT_COUNT),
														 lights.GetColorBegin(i*MAX_LIGHT_COUNT),
														 lights.GetIntensityBegin(i*MAX_LIGHT_COUNT),
														 Size
														);
				
				InstanceSet.DrawInstances(shader,mat_shader);
			}
		}
		
		for(lrGL3StaticInstanceSet &InstanceSet : SkeletalMeshInstanceSets)
		{
			InstanceSet.Bind();
			for(int i=0;i < NumPasses;i++)
			{
				unsigned int Size = std::min<std::size_t>(lights.NumLights() - i*MAX_LIGHT_COUNT,MAX_LIGHT_COUNT);
				lrGLRenderUtils::UploadUniformLightArray(point_light_shader,
														 lights.GetPositionBegin(i*MAX_LIGHT_COUNT),
														 lights.GetColorBegin(i*MAX_LIGHT_COUNT),
														 lights.GetIntensityBegin(i*MAX_LIGHT_COUNT),
														 Size
														);
				
				InstanceSet.DrawInstances(shader,mat_shader);
			}
		}
	}
	
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	
	void Print()
	{
		for(auto &J : ModelDictionary)
		{
			std::cout << "\tMesh: " << J.first << "\n";
			lrGL3MeshInstances &MeshInstances = *J.second;
			MeshInstances.Print();
		}
	}
	
	#endif
	
	const std::string &GetAlbedoMapName()
	{
		return AlbedoMapName;
	}
	
	lrGLTexture2DView &GetAlbedoMap()
	{
		return AlbedoMap;
	}
	
	const std::string &GetEnvironmentMapName()
	{
		return EnvironmentMapName;
	}
	
	lrGLTextureCubemapView &GetEnvironmentMap()
	{
		return EnvironmentMap;
	}
	
	void Construct(const std::string &albedo_map_name,lrGLTexture2DView albedo_map,
			const std::string &environment_map_name,lrGLTextureCubemapView environment_map,
			unsigned int num_static_mesh_material_groups,unsigned int num_skeletal_mesh_material_groups)
	{
		AlbedoMapName = albedo_map_name;
		AlbedoMap = albedo_map;
		
		EnvironmentMapName = environment_map_name;
		EnvironmentMap = environment_map;
		
		StaticMeshInstanceSets.resize(num_static_mesh_material_groups);
		SkeletalMeshInstanceSets.resize(num_skeletal_mesh_material_groups);
	}
	
	lrGL3StaticInstanceSet &GetStaticMeshInstanceSet(const std::string &material_group_key)
	{
		return *StaticMeshModelDictionary[material_group_key];
	}
	
	lrGL3StaticInstanceSet &GetSkeletalMeshInstanceSet(const std::string &material_group_key)
	{
		return *SkeletalMeshModelDictionary[material_group_key];
	}
	
	void InitializeStaticMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lrGL3StaticInstanceSet &MatGroupInstances = StaticMeshInstanceSets[mat_group_id];
		
		MatGroupInstances.Resize(num_instances);
		StaticMeshModelDictionary[material_group_name] = &MatGroupInstances;
	}
	
	void InitializeSkeletalMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lrGL3StaticInstanceSet &MatGroupInstances = SkeletalMeshInstanceSets[mat_group_id];
		
		MatGroupInstances.Resize(num_instances);
		SkeletalMeshModelDictionary[material_group_name] = &MatGroupInstances;
	}
};


#endif // LR_GL_TEXTURE_LIST_H
