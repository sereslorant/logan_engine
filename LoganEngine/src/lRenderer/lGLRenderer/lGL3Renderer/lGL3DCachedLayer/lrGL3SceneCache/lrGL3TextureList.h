#ifndef LR_GL_TEXTURE_LIST_H
#define LR_GL_TEXTURE_LIST_H

#include "lrGL3MeshInstances.h"

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
	std::vector<lrGL3MeshInstances> MeshInstances;
	std::map<std::string,lrGL3MeshInstances *> ModelDictionary;
	//
public:
	//
	void DrawTextureList(liGLShader &shader,liGLPbMatShader &mat_shader)
	{
		for(lrGL3MeshInstances &MeshInstances_Puszcsy : MeshInstances)
		{
			MeshInstances_Puszcsy.DrawInstances(shader,mat_shader);
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
			unsigned int num_material_groups)
	{
		AlbedoMapName = albedo_map_name;
		AlbedoMap = albedo_map;
		//
		EnvironmentMapName = environment_map_name;
		EnvironmentMap = environment_map;
		//
		MeshInstances.resize(num_material_groups);
	}
	//
	lrGL3MeshInstances &GetMeshInstances(const std::string &material_group_key)
	{
		return *ModelDictionary[material_group_key];
	}
	//
	void InitializeMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lrGL3MeshInstances &MatGroupInstances = MeshInstances[mat_group_id];
		//
		MatGroupInstances.Resize(num_instances);
		ModelDictionary[material_group_name] = &MatGroupInstances;
	}
};


#endif // LR_GL_TEXTURE_LIST_H
