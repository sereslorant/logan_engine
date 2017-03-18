#ifndef LR_GL3D_CACHED_LAYER_H
#define LR_GL3D_CACHED_LAYER_H

#include "../../../lrLayer.h"

#include "../../lGLResources/lrGLResourceLoader.h"


#include "lrGL3SceneCache/lrGL3SceneCache.h"

#include "../lGL3Shaders/lrGL3StaticMeshShader.h"



class lrGL3TextureBindingState3D : public liGLTextureBindingState3D
{
private:
	liGLPbMatShader *Shader = nullptr;
	//
	static constexpr unsigned int NUM_ACTIVE_TEXTURE = 2;
	//
	GLuint BoundTextureId[NUM_ACTIVE_TEXTURE];
	//
	const GLuint ALBEDO_MAP_ACTIVE_TEXTURE = 0;
	const GLuint ENVIRONMENT_MAP_ACTIVE_TEXTURE = 1;
	//
public:
	//
	void SetShader(liGLPbMatShader *shader)
	{
		Shader = shader;
	}
	//
	void Reset()
	{
		for(unsigned int i=0;i < NUM_ACTIVE_TEXTURE;i++)
		{
			BoundTextureId[i] = 0;
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
	virtual void BindAlbedoMap(lrGLTexture2DView &albedo_map) override
	{
		if(Shader != nullptr)
		{
			BindTexture(Shader->GetAlbedoMapLocation(),ALBEDO_MAP_ACTIVE_TEXTURE,albedo_map);
		}
	}
	//
	virtual void BindEnvironemntMap(lrGLTextureCubemapView &environment_map) override
	{
		if(Shader != nullptr)
		{
			BindTexture(Shader->GetEnvironmentMapLocation(),ENVIRONMENT_MAP_ACTIVE_TEXTURE,environment_map);
		}
	}
	//
	lrGL3TextureBindingState3D()
	{
		Reset();
	}
	//
	virtual ~lrGL3TextureBindingState3D() override
	{
		//
	}
	/*
	 * End of class
	 */
};

#include <chrono>

class lrGL3RenderState3D
{
private:
	lrGL3StaticMeshShader &StaticMeshPrePassShader;
	lrGL3StaticMeshShader &StaticMeshPointLightShader;
	lrGL3StaticMeshShader &StaticMeshEnvironmentShader;
	//
	lrGL3TextureBindingState3D TextureBindingState;
	//
	/*
	 * A renderelés egyes lépéseit megvalósító függvények
	 */
	void Prepare()
	{
		/*
		 * Setting up depth testing
		 */
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE); // Ez kell, hogy a clear hatással legyen a depth bufferre.
		//
		/*
		 * Setting up backface culling
		 */
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		//
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	//
	void PreparePrePass()
	{
		glDepthFunc(GL_LESS);
		//
		glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
	}
	//
	void PrePass(lrGL3SceneCache &scene_cache)
	{
		StaticMeshPrePassShader.UseProgram();
		TextureBindingState.SetShader(&StaticMeshPrePassShader);
		//
		scene_cache.UploadMatrices(StaticMeshPrePassShader);
		scene_cache.DrawMeshes(TextureBindingState,StaticMeshPrePassShader,false,false);
	}
	//
	void PrepareDraw()
	{
		glDepthFunc(GL_LEQUAL);
		//
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		//
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE,GL_ONE);
	}
	//
	void DrawPointLights(lrGL3SceneCache &scene_cache)
	{
		StaticMeshPointLightShader.UseProgram();
		TextureBindingState.SetShader(&StaticMeshPointLightShader);
		//
		scene_cache.UploadMatrices(StaticMeshPointLightShader);
		scene_cache.DrawPointLights(TextureBindingState,StaticMeshPointLightShader);
	}
	//
	void DrawReflections(lrGL3SceneCache &scene_cache)
	{
		StaticMeshEnvironmentShader.UseProgram();
		TextureBindingState.SetShader(&StaticMeshEnvironmentShader);
		//
		scene_cache.UploadMatrices(StaticMeshEnvironmentShader);
		scene_cache.DrawMeshes(TextureBindingState,StaticMeshEnvironmentShader,true,true);
	}
	//
public:
	lrGL3StaticMeshShader &GetStaticMeshPointLightShader()
	{
		return StaticMeshPointLightShader;
	}
	//
	lrGL3StaticMeshShader &GetStaticMeshEnvironmentShader()
	{
		return StaticMeshEnvironmentShader;
	}
	//
	void DrawScene(lrGL3SceneCache &scene_cache)
	{
		TextureBindingState.Reset();
		Prepare();
		//
		PreparePrePass();
		auto Before = std::chrono::steady_clock::now();
		PrePass(scene_cache);
		auto After = std::chrono::steady_clock::now();
		//
		std::chrono::nanoseconds Difference = After - Before;
		std::cout << "Pre pass time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		//
		PrepareDraw();
		//
		Before = std::chrono::steady_clock::now();
		DrawPointLights(scene_cache);
		After = std::chrono::steady_clock::now();
		//
		Difference = After - Before;
		std::cout << "Point light time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		//
		Before = std::chrono::steady_clock::now();
		DrawReflections(scene_cache);
		After = std::chrono::steady_clock::now();
		//
		Difference = After - Before;
		std::cout << "EnvMap time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
	}
	//
	lrGL3RenderState3D(lrGL3StaticMeshShader &static_mesh_pre_pass_shader,lrGL3StaticMeshShader &static_mesh_point_light_shader,lrGL3StaticMeshShader &static_mesh_environment_shader)
		:StaticMeshPrePassShader(static_mesh_pre_pass_shader),StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader)
	{}
	//
	~lrGL3RenderState3D()
	{}
	/*
	 * End of class
	 */
};

class lrGL3DCachedLayer : public lr3DLayer
{
private:
	lrGL3RenderState3D &RenderState3D;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual void DrawScene() override;
	//
public:
	//
	lrGL3DCachedLayer(lrGL3RenderState3D &render_state_3D,lrGLResourceLoader &resource_loader)
		:RenderState3D(render_state_3D),ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGL3DCachedLayer() override
	{}
	/*
	 * End of class
	 */
};


#endif // LR_GL3D_CACHED_LAYER_H
