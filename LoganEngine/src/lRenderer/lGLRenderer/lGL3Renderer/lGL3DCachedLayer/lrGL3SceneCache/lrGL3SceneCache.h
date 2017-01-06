#ifndef LR_GL3_SCENE_CACHE_H
#define LR_GL3_SCENE_CACHE_H

#include "lrGL3TextureList.h"
#include "lrGLLights.h"


#include "../../../lGLResources/lrGLResourceLoader.h"
#include "../../../../lrUtils.h"

#include "../../../../lr3DSceneReader.h"

#include "../../lrGL3StaticMeshShader.h"

#include <vector>
#include <map>

class lrGL3SceneCache
{
private:
	//
	class lrGL3SceneCacher : public li3DSceneCache
	{
	private:
		lrGLResourceLoader &ResourceLoader;
		lrGL3SceneCache &SceneCache;
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
				//
				lrGL3MeshInstances &MeshInstances = SceneCache.GetMeshInstances(TextureGroupName,MaterialGroupName);
				//
				MeshInstances.Construct(SceneCache.StaticMeshPointLightShader,MtlGroup);
				MeshInstances.SetNextInstance(SceneCache.ProjectionMatrix,SceneCache.ViewMatrix,Material,mesh);
			}
		}
		//
		virtual void CacheLight(const li3DLight &light) override
		{
			SceneCache.Lights.SetNextLight(light);
		}
		//
		lrGL3SceneCacher(lrGLResourceLoader &resource_loader,lrGL3SceneCache &scene_cache)
			:ResourceLoader(resource_loader),SceneCache(scene_cache)
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
	lrGL3StaticMeshShader &StaticMeshPointLightShader;
	lrGL3StaticMeshShader &StaticMeshEnvironmentShader;
	//
	const GLuint ALBEDO_MAP_ACTIVE_TEXTURE = 0;
	//
	const GLuint ENVIRONMENT_MAP_ACTIVE_TEXTURE = 1;
	//
	std::map<std::string,lrGL3TextureList *> TextureDictionary;
	std::vector<lrGL3TextureList> TextureLists;
	//
	lrGL3TextureList &GetTextureList(const std::string &texture_group_name)
	{
		return *TextureDictionary[texture_group_name];
	}
	//
	lrGL3MeshInstances &GetMeshInstances(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetMeshInstances(material_group_name);
	}
	//
	/*
	 * Minden, ami a rendereléshez köthető
	 */
	void Prepare()
	{
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
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
	template<class lrGLTextureView_T>
	void BindTexture(GLint texture_location,GLuint active_texture,lrGLTextureView_T &texture)
	{
		glActiveTexture(GL_TEXTURE0 + active_texture);
		texture.Bind();

		glUniform1i(texture_location,active_texture);
	}
	//
	void DrawMeshes()
	{
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
			BindTexture(StaticMeshPointLightShader.GetAlbedoMapLocation(),ALBEDO_MAP_ACTIVE_TEXTURE,AlbedoMap);
			//
			TextureList.DrawTextureList(StaticMeshPointLightShader,StaticMeshPointLightShader);
			//
			//TODO
			//AlbedoMap.Unbind();
		}
	}
	//
	void PrePass()
	{
		/*
		 * TODO: A shadert ki kell majd cserélni.
		 */
		SetProjectionMatrix(StaticMeshEnvironmentShader);
		SetViewMatrix(StaticMeshEnvironmentShader);
		//
		glDepthFunc(GL_LESS);
		//
		glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		//
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			/*
			 * TODO: A shadert ki kell majd cserélni.
			 */
			TextureList.DrawTextureList(StaticMeshEnvironmentShader,StaticMeshEnvironmentShader);
		}
	}
	//
	void DrawReflections()
	{
		SetProjectionMatrix(StaticMeshEnvironmentShader);
		SetViewMatrix(StaticMeshEnvironmentShader);
		//
		glDepthFunc(GL_LEQUAL);
		//
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		//
		glDisable(GL_BLEND);
		//
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
			BindTexture(StaticMeshEnvironmentShader.GetAlbedoMapLocation(),ALBEDO_MAP_ACTIVE_TEXTURE,AlbedoMap);
			//
			lrGLTextureCubemapView EnvironmentMap = TextureList.GetEnvironmentMap();
			BindTexture(StaticMeshEnvironmentShader.GetEnvironmentMapLocation(),ENVIRONMENT_MAP_ACTIVE_TEXTURE,EnvironmentMap);
			//
			TextureList.DrawTextureList(StaticMeshEnvironmentShader,StaticMeshEnvironmentShader);
		}
	}
	//
	void Draw()
	{
		SetProjectionMatrix(StaticMeshPointLightShader);
		SetViewMatrix(StaticMeshPointLightShader);
		//
		glDepthFunc(GL_LEQUAL);
		//
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		//
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE,GL_ONE);
		//glDisable(GL_BLEND);
		//bool First = true;
		//
		for(unsigned int i=0;i < Lights.NumLights();i++)
		{
			Lights.ApplyLight(StaticMeshPointLightShader,i);
			//
			DrawMeshes();
			//
			/*
			if(First)
			{
				First = false;
				//
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_ONE,GL_ONE);
			}*/
		}
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
	void DrawScene()
	{
		glEnable(GL_DEPTH_TEST);
		//
		StaticMeshEnvironmentShader.UseProgram();
		//
		Prepare();
		//
		PrePass();
		//
		DrawReflections();
		//
		StaticMeshPointLightShader.UseProgram();
		//
		Draw();
		//
		StaticMeshPointLightShader.DisableProgram();
		//
		/*
		 * Ez kell, hogy a clear hatással legyen a depth bufferre.
		 */
		glDepthMask(GL_TRUE);
	}
	//
	lrGL3SceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,lrGL3StaticMeshShader &static_mesh_point_light_shader,lrGL3StaticMeshShader &static_mesh_environment_shader,lrGLResourceLoader &resource_loader);
	//
	~lrGL3SceneCache()
	{}
};

#endif // LR_GL3_SCENE_CACHE_H
