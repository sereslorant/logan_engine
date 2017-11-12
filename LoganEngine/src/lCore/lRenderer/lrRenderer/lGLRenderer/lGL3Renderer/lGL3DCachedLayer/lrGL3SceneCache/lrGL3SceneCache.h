#ifndef LR_GL3_SCENE_CACHE_H
#define LR_GL3_SCENE_CACHE_H

#include "lrGL3TextureList.h"
#include "lrGLLights.h"

#include "../../../lGLResources/lrGLResourceLoader.h"
#include "../../../../lrUtils.h"

#include "../../../../lr3DSceneReader.h"

#include "../../lGL3Shaders/lrGL3StaticMeshShader.h"

#include <vector>
#include <map>

class liGLTextureBindingState3D
{
public:
	//
	virtual void BindAlbedoMap(lrGLTexture2DView &albedo_map) = 0;
	virtual void BindEnvironemntMap(lrGLTextureCubemapView &environment_map) = 0;
	//
	liGLTextureBindingState3D()
	{}
	//
	virtual ~liGLTextureBindingState3D()
	{}
	/*
	 * End of class
	 */
};

class lrGL3SceneCache
{
private:
	//
	lmMatrix4x4 ProjectionMatrix;
	lmMatrix4x4 ViewMatrix;
	//
	lmVector3D CameraPosition;
	//
	lrGLLights Lights;
	//
	std::map<std::string,lrGL3TextureList *> TextureDictionary;
	std::vector<lrGL3TextureList> TextureLists;
	//
	void SetProjectionMatrix(liGLShader &shader)
	{
		glUniformMatrix4fv(shader.GetProjectionMatrixLocation(),1,GL_FALSE,ProjectionMatrix[0]);
	}
	//
	void SetViewMatrix(liGLShader &shader)
	{
		//glUniformMatrix4fv(shader.GetViewMatrixLocation(),1,GL_FALSE,ViewMatrix[0]);
		glUniform3fv(shader.GetCameraPositionLocation(),1,&CameraPosition[0]);
	}
	//
public:
	//
	const lmMatrix4x4 &GetProjectionMatrix()
	{
		return ProjectionMatrix;
	}
	//
	const lmMatrix4x4 &GetViewMatrix()
	{
		return ViewMatrix;
	}
	//
	lrGL3TextureList &GetTextureList(const std::string &texture_group_name)
	{
		return *TextureDictionary[texture_group_name];
	}
	//
	lrGL3InstanceSet &GetStaticMeshInstanceSet(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetStaticMeshInstanceSet(material_group_name);
	}
	//
	lrGL3InstanceSet &GetSkeletalMeshInstanceSet(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetSkeletalMeshInstanceSet(material_group_name);
	}
	//
	lrGLLights &GetLights()
	{
		return Lights;
	}
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	//
	void Print()
	{
		std::cout << "StaticMeshes:" << std::endl;
		for(auto &I : TextureDictionary)
		{
			std::cout << "Texture: " << I.first << "\n";
			lrGL3TextureList &TextureList = *I.second;
			TextureList.Print();
		}
		std::cout << std::endl;
	}
	//
	#endif
	//
	void UploadMatrices(lrGL3StaticMeshShader &shader)
	{
		//SetProjectionMatrix(shader);
		SetViewMatrix(shader);
	}
	//
	void DrawMeshes(liGLTextureBindingState3D &texture_binding_state,lrGL3StaticMeshShader &shader,bool bind_textures,bool bind_environment_map)
	{
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			if(bind_textures)
			{
				lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
				texture_binding_state.BindAlbedoMap(AlbedoMap);
			}
			//
			if(bind_environment_map)
			{
				lrGLTextureCubemapView EnvironmentMap = TextureList.GetEnvironmentMap();
				texture_binding_state.BindEnvironemntMap(EnvironmentMap);
			}
			//
			TextureList.DrawTextureList(shader,shader);
		}
	}
	//
	void DrawPointLights(liGLTextureBindingState3D &texture_binding_state,lrGL3StaticMeshShader &shader)
	{
		for(unsigned int i=0;i < Lights.NumLights();i++)
		{
			Lights.ApplyLight(shader,i);
			//
			DrawMeshes(texture_binding_state,shader,true,false);
		}
	}
	//
	lrGL3SceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,/*lrGL3StaticMeshShader &static_mesh_point_light_shader,*//*lrGL3StaticMeshShader &static_mesh_environment_shader,*/lrGLResourceLoader &resource_loader);
	//
	~lrGL3SceneCache()
	{}
};

class lrGL3InstanceCacher
{
private:
	const lmMatrix4x4 *ProjectionMatrix = nullptr;
	const lmMatrix4x4 *ViewMatrix = nullptr;
	//
	liGL3InstanceSet *InstanceSet= nullptr;
	unsigned int Next;
	//
public:
	//
	bool Initialized()
	{
		return InstanceSet != nullptr;
	}
	//
	bool Finished()
	{
		if(InstanceSet != nullptr)
		{
			return Next == InstanceSet->NumInstances();
		}
		//
		return false;
	}
	//
	void Initialize(const lmMatrix4x4 *projection_matrix,const lmMatrix4x4 *view_matrix,liGL3InstanceSet *instance_set)
	{
		ProjectionMatrix = projection_matrix;
		ViewMatrix = view_matrix;
		//
		InstanceSet = instance_set;
		Next = 0;
	}
	//
	void SetNextInstance(const liMaterial &material,const li3DMesh &mesh)
	{
		if(Initialized())
		{
			InstanceSet->SetInstance(material,mesh,Next);
			Next++;
			//
			if(Finished())
			{
				if(ProjectionMatrix != nullptr && ViewMatrix != nullptr)
				{
					InstanceSet->InitializeMvpMatrices(*ProjectionMatrix,*ViewMatrix);
				}
			}
		}
	}
	//
	lrGL3InstanceCacher()
	{}
	//
	~lrGL3InstanceCacher()
	{}
	/*
	 * End of class
	 */
};

class lrGL3LightCacher
{
private:
	lrGLLights *Lights= nullptr;
	unsigned int Next;
	//
public:
	//
	bool Initialized()
	{
		return Lights != nullptr;
	}
	//
	void Initialize(lrGLLights *lights)
	{
		Lights = lights;
		Next = 0;
	}
	//
	void SetNextLight(const li3DLight &light)
	{
		if(Initialized())
		{
			Lights->SetLight(light,Next);
			Next++;
		}
	}
	//
	lrGL3LightCacher()
	{
		//
	}
	//
	~lrGL3LightCacher()
	{
		//
	}
	/*
	 * End of class
	 */
};

class lrGL3SceneCacher : public li3DSceneCache
{
private:
	lrGLResourceLoader &ResourceLoader;
	lrGL3StaticMeshShader &StaticMeshShader;
	//
	lrGL3SceneCache &SceneCache;
	//
	lrGL3LightCacher LightCacher;
	std::map<std::string,lrGL3InstanceCacher> StaticMeshInstanceCachers;
	std::map<std::string,lrGL3InstanceCacher> SkeletalMeshInstanceCachers;
	//
	lrGL3InstanceCacher &GetStaticMeshInstanceCacher(lrGLStaticMaterialGroupView &MtlGroup,const std::string &texture_group_name,const std::string &material_group_name)
	{
		std::string InstanceCacherKey = texture_group_name + ";" + material_group_name;
		lrGL3InstanceCacher &InstanceCacher = StaticMeshInstanceCachers[InstanceCacherKey];
		//
		if(!InstanceCacher.Initialized())
		{
			lrGL3InstanceSet &MeshInstances = SceneCache.GetStaticMeshInstanceSet(texture_group_name,material_group_name);
			MeshInstances.Construct(StaticMeshShader,MtlGroup);
			//
			InstanceCacher.Initialize(&SceneCache.GetProjectionMatrix(),&SceneCache.GetViewMatrix(),&MeshInstances);
		}
		//
		return InstanceCacher;
	}
	//
	lrGL3InstanceCacher &GetSkeletalMeshInstanceCacher(lrGLStaticMaterialGroupView &MtlGroup,const std::string &texture_group_name,const std::string &material_group_name)
	{
		std::string InstanceCacherKey = texture_group_name + ";" + material_group_name;
		lrGL3InstanceCacher &InstanceCacher = SkeletalMeshInstanceCachers[InstanceCacherKey];
		//
		if(!InstanceCacher.Initialized())
		{
			lrGL3InstanceSet &MeshInstances = SceneCache.GetSkeletalMeshInstanceSet(texture_group_name,material_group_name);
			MeshInstances.Construct(StaticMeshShader,MtlGroup);
			//
			InstanceCacher.Initialize(&SceneCache.GetProjectionMatrix(),&SceneCache.GetViewMatrix(),&MeshInstances);
		}
		//
		return InstanceCacher;
	}
	//
public:
	//
	virtual void CacheStaticMesh(const li3DMesh &mesh,const li3DStaticMesh &static_mesh) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
		const liMaterialLibrary &MaterialLibrary = mesh.GetMaterialLibrary();
		//
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MtlGroup = StaticMesh.GetMaterialGroup(i);
			//
			const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
			//
			std::string TextureGroupName;
			std::string MaterialGroupName;
			lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
			lrUtils::ComputeMaterialGroupKey(static_mesh.GetModelName(),MtlGroup.GetMaterial(),MaterialGroupName);
			/*
			  //
			std::string TextureGroupName;
			std::string MaterialGroupName;
			lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
			lrUtils::ComputeMaterialGroupKey(static_mesh.GetModelName(),MtlGroup.GetMaterial(),MaterialGroupName);
			//
			lrGL3MeshInstances &MeshInstances = SceneCache.GetMeshInstances(TextureGroupName,MaterialGroupName);
			//
			MeshInstances.Construct(SceneCache.StaticMeshPointLightShader,MtlGroup);
			MeshInstances.SetNextInstance(SceneCache.ProjectionMatrix,SceneCache.ViewMatrix,Material,mesh);
			  */
			//
			lrGL3InstanceCacher &MeshInstances = GetStaticMeshInstanceCacher(MtlGroup,TextureGroupName,MaterialGroupName);
			//
			MeshInstances.SetNextInstance(Material,mesh);
		}
	}
	//
	virtual void CacheSkeletalMesh(const li3DMesh &mesh,const li3DSkeletalMesh &skeletal_mesh) override
	{
		lrGLStaticMeshView SkeletalMesh = ResourceLoader.GetSkeletalMesh(skeletal_mesh.GetCharacterName());
		const liMaterialLibrary &MaterialLibrary = mesh.GetMaterialLibrary();
		//
		for(unsigned int i=0;i < SkeletalMesh.NumMtlGroups();i++)
		{
			lrGLStaticMaterialGroupView MtlGroup = SkeletalMesh.GetMaterialGroup(i);
			//
			const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
			//
			std::string TextureGroupName;
			std::string MaterialGroupName;
			lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
			lrUtils::ComputeMaterialGroupKey(skeletal_mesh.GetCharacterName(),MtlGroup.GetMaterial(),MaterialGroupName);
			/*
			  //
			std::string TextureGroupName;
			std::string MaterialGroupName;
			lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
			lrUtils::ComputeMaterialGroupKey(static_mesh.GetModelName(),MtlGroup.GetMaterial(),MaterialGroupName);
			//
			lrGL3MeshInstances &MeshInstances = SceneCache.GetMeshInstances(TextureGroupName,MaterialGroupName);
			//
			MeshInstances.Construct(SceneCache.StaticMeshPointLightShader,MtlGroup);
			MeshInstances.SetNextInstance(SceneCache.ProjectionMatrix,SceneCache.ViewMatrix,Material,mesh);
			  */
			//
			lrGL3InstanceCacher &MeshInstances = GetSkeletalMeshInstanceCacher(MtlGroup,TextureGroupName,MaterialGroupName);
			//
			MeshInstances.SetNextInstance(Material,mesh);
		}
	}
	//
	virtual void CacheLight(const li3DLight &light) override
	{
		LightCacher.SetNextLight(light);
	}
	//
	lrGL3SceneCacher(lrGLResourceLoader &resource_loader,lrGL3StaticMeshShader &static_mesh_shader,lrGL3SceneCache &scene_cache)
		:ResourceLoader(resource_loader),StaticMeshShader(static_mesh_shader),SceneCache(scene_cache)
	{
		LightCacher.Initialize(&scene_cache.GetLights());
	}
	//
	virtual ~lrGL3SceneCacher() override
	{
		//
	}
	//
	/*
	 * End of class
	 */
};

#endif // LR_GL3_SCENE_CACHE_H
