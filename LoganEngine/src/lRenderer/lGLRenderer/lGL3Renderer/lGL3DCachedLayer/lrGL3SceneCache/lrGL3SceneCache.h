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

class lrGL3TextureBindingState3D
{
private:
	static constexpr unsigned int NUM_ACTIVE_TEXTURE = 2;
	//
	GLuint BoundTextureId[NUM_ACTIVE_TEXTURE];
	//
public:
	//
	const GLuint ALBEDO_MAP_ACTIVE_TEXTURE = 0;
	const GLuint ENVIRONMENT_MAP_ACTIVE_TEXTURE = 1;
	//
	void Reset()
	{
		for(unsigned int i=0;i < NUM_ACTIVE_TEXTURE;i++)
		{
			BoundTextureId[i] = 0xFFFFFFFF;
		}
	}
	//
	template<class lrGLTextureView_T>
	void BindTexture(GLint texture_location,GLuint active_texture,lrGLTextureView_T &texture)
	{
		if(BoundTextureId[active_texture] != texture.GetTextureId())
		{
			glActiveTexture(GL_TEXTURE0 + active_texture);
			texture.Bind();
			//
			BoundTextureId[active_texture] = texture.GetTextureId();
		}
		//
		glUniform1i(texture_location,active_texture);
	}
	//
	lrGL3TextureBindingState3D()
	{
		Reset();
	}
	//
	~lrGL3TextureBindingState3D()
	{}
	/*
	 * End of class
	 */
};

class lrGL3SceneCache
{
private:
	//
	class lrGL3SceneCacher : public li3DSceneCache
	{
	private:
		lrGLResourceLoader &ResourceLoader;
		lrGL3StaticMeshShader &StaticMeshShader;
		lrGL3SceneCache &SceneCache;
		//
		std::map<std::string,lrGL3InstanceCacher> InstanceCachers;
		//
		lrGL3InstanceCacher &GetInstanceCacher(lrGLMaterialGroupView &MtlGroup,const std::string &texture_group_name,const std::string &material_group_name)
		{
			std::string InstanceCacherKey = texture_group_name + ";" + material_group_name;
			lrGL3InstanceCacher &InstanceCacher = InstanceCachers[InstanceCacherKey];
			//
			if(!InstanceCacher.Initialized())
			{
				lrGL3InstanceSet &MeshInstances = SceneCache.GetInstanceSet(texture_group_name,material_group_name);
				MeshInstances.Construct(StaticMeshShader,MtlGroup);
				//
				InstanceCacher.Initialize(&MeshInstances);
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
				lrGLMaterialGroupView MtlGroup = StaticMesh.GetMaterialGroup(i);
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
				lrGL3InstanceCacher &MeshInstances = GetInstanceCacher(MtlGroup,TextureGroupName,MaterialGroupName);
				//
				MeshInstances.SetNextInstance(SceneCache.ProjectionMatrix,SceneCache.ViewMatrix,Material,mesh);
			}
		}
		//
		virtual void CacheLight(const li3DLight &light) override
		{
			SceneCache.Lights.SetNextLight(light);
		}
		//
		lrGL3SceneCacher(lrGLResourceLoader &resource_loader,lrGL3StaticMeshShader &static_mesh_shader,lrGL3SceneCache &scene_cache)
			:ResourceLoader(resource_loader),StaticMeshShader(static_mesh_shader),SceneCache(scene_cache)
		{
			//
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
	//
	lrGL3SceneCacher SceneCacher;
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
		glUniformMatrix4fv(shader.GetViewMatrixLocation(),1,GL_FALSE,ViewMatrix[0]);
		glUniform3fv(shader.GetCameraPositionLocation(),1,&CameraPosition[0]);
	}
	//
	lrGL3TextureList &GetTextureList(const std::string &texture_group_name)
	{
		return *TextureDictionary[texture_group_name];
	}
	//
	lrGL3InstanceSet &GetInstanceSet(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetInstanceSet(material_group_name);
	}
	//
public:
	//
	li3DSceneCache &GetSceneCacher()
	{
		return SceneCacher;
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
		SetProjectionMatrix(shader);
		SetViewMatrix(shader);
	}
	//
	void DrawMeshes(lrGL3TextureBindingState3D &texture_binding_state,lrGL3StaticMeshShader &shader,bool bind_textures,bool bind_environment_map)
	{
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			if(bind_textures)
			{
				lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
				texture_binding_state.BindTexture(shader.GetAlbedoMapLocation(),texture_binding_state.ALBEDO_MAP_ACTIVE_TEXTURE,AlbedoMap);
			}
			//
			if(bind_environment_map)
			{
				lrGLTextureCubemapView EnvironmentMap = TextureList.GetEnvironmentMap();
				texture_binding_state.BindTexture(shader.GetEnvironmentMapLocation(),texture_binding_state.ENVIRONMENT_MAP_ACTIVE_TEXTURE,EnvironmentMap);
			}
			//
			TextureList.DrawTextureList(shader,shader);
		}
	}
	//
	void DrawPointLights(lrGL3TextureBindingState3D &texture_binding_state,lrGL3StaticMeshShader &shader)
	{
		for(unsigned int i=0;i < Lights.NumLights();i++)
		{
			Lights.ApplyLight(shader,i);
			//
			DrawMeshes(texture_binding_state,shader,true,false);
		}
	}
	//
	lrGL3SceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,lrGL3StaticMeshShader &static_mesh_point_light_shader,/*lrGL3StaticMeshShader &static_mesh_environment_shader,*/lrGLResourceLoader &resource_loader);
	//
	~lrGL3SceneCache()
	{}
};

#endif // LR_GL3_SCENE_CACHE_H
