#ifndef LR_GL3_SCENE_CACHE_H
#define LR_GL3_SCENE_CACHE_H

#include "lrGLData/lrGLLightArray.h"
#include "lrGL3TextureList.h"


#include <lRenderer/lrRenderer/lr3DSceneReader.h>

#include <vector>
#include <map>



#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3DTextureBindingState.h>

class lrGL3SceneCache
{
private:
	lmMatrix4x4 ProjectionMatrix;
	lmMatrix4x4 ViewMatrix;
	
	lmVector3D CameraPosition;
	
	lrGLLightArray Lights;
	
	std::map<std::string,lrGL3TextureList *> TextureDictionary;
	std::vector<lrGL3TextureList> TextureLists;
	/*
	void SetProjectionMatrix(liGLShader &shader)
	{
		glUniformMatrix4fv(shader.GetProjectionMatrixLocation(),1,GL_FALSE,ProjectionMatrix[0]);
	}
	*/
	void SetViewMatrix(liGL3DShader &shader)
	{
		//glUniformMatrix4fv(shader.GetViewMatrixLocation(),1,GL_FALSE,ViewMatrix[0]);
		glUniform3fv(shader.GetCameraPositionLocation(),1,&CameraPosition[0]);
	}
	
public:
	
	const lmMatrix4x4 &GetProjectionMatrix()
	{
		return ProjectionMatrix;
	}
	
	const lmMatrix4x4 &GetViewMatrix()
	{
		return ViewMatrix;
	}
	
	lrGL3TextureList &GetTextureList(const std::string &texture_group_name)
	{
		return *TextureDictionary[texture_group_name];
	}
	
	lrGL3StaticInstanceSet &GetStaticMeshInstanceSet(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetStaticMeshInstanceSet(material_group_name);
	}
	
	lrGL3StaticInstanceSet &GetSkeletalMeshInstanceSet(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetSkeletalMeshInstanceSet(material_group_name);
	}
	
	lrGLLightArray &GetLights()
	{
		return Lights;
	}
	
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	
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
	
	#endif
	
	void UploadMatrices(liGL3DShader &shader)
	{
		//SetProjectionMatrix(shader);
		SetViewMatrix(shader);
	}
	
	void DrawMeshes(lrGL3DTextureBindingState &texture_binding_state,liGL3DShader &mat_shader,liGLPbMatShader &shader,bool bind_textures,bool bind_environment_map)
	{
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			if(bind_textures)
			{
				lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
				texture_binding_state.BindAlbedoMap(AlbedoMap);
			}
			
			if(bind_environment_map)
			{
				lrGLTextureCubemapView EnvironmentMap = TextureList.GetEnvironmentMap();
				texture_binding_state.BindEnvironmentMap(EnvironmentMap);
			}
			
			TextureList.DrawTextureList(mat_shader,shader);
		}
	}
	
	void DrawPointLights(lrGL3DTextureBindingState &texture_binding_state,liGL3DShader &mat_shader,liGLPointLightShader &point_light_shader,liGLPbMatShader &shader)
	{
		/*
		constexpr unsigned int MAX_LIGHT_COUNT = 256;
		
		unsigned int NumPasses = Lights.NumLights() / MAX_LIGHT_COUNT;
		if(Lights.NumLights() % MAX_LIGHT_COUNT > 0)
			{NumPasses += 1;}
		
		for(int i=0;i < NumPasses;i++)
		{
			unsigned int Size = std::min<std::size_t>(Lights.NumLights() - i*MAX_LIGHT_COUNT,MAX_LIGHT_COUNT);
			lrGLRenderUtils::UploadUniformLightArray(point_light_shader,
														Lights.GetPositionBegin(i*MAX_LIGHT_COUNT),
														Lights.GetColorBegin(i*MAX_LIGHT_COUNT),
														Lights.GetIntensityBegin(i*MAX_LIGHT_COUNT),
														Size
													);
			
			DrawMeshes(texture_binding_state,mat_shader,shader,true,false);
		}
		*/
		
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
			texture_binding_state.BindAlbedoMap(AlbedoMap);
			
			TextureList.ForeachLightDrawTextureList(mat_shader,shader,point_light_shader,Lights);
		}
		
	}
	
	lrGL3SceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,/*lrGL3StaticMeshShader &static_mesh_point_light_shader,*//*lrGL3StaticMeshShader &static_mesh_environment_shader,*/lrGLResourceLoader &resource_loader);
	
	~lrGL3SceneCache()
	{}
};

#endif // LR_GL3_SCENE_CACHE_H
