#ifndef LR_GL3D_CACHED_LAYER_H
#define LR_GL3D_CACHED_LAYER_H

#include <lRenderer/lrRenderer/lrLayer.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>


#include "lrGL3SceneCache/lrGL3SceneCache.h"
#include "lrGL3SceneCacher/lrGL3SceneCacher.h"


#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGL3DShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLPhysicallyBasedShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLPointLightShader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DShaders/lrGLStaticMeshShader.h>



#include <chrono>

class lrGL3RenderState3D
{
private:
	lrGLShader &StaticMeshPrePassShader;
	lrGLShader &StaticMeshPointLightShader;
	lrGLShader &StaticMeshEnvironmentShader;
	
	lrGL3DTextureBindingState TextureBindingState;
	
	void PrePass(lrGL3SceneCache &scene_cache)
	{
		StaticMeshPrePassShader.UseProgram();
		
		lrGL3DShader TransformShader(StaticMeshPrePassShader);
		lrGLPhysicallyBasedShader PhysicallyBasedShader(StaticMeshPrePassShader);
		TextureBindingState.SetShader(&PhysicallyBasedShader);
		
		scene_cache.UploadMatrices(TransformShader);
		scene_cache.DrawMeshes(TextureBindingState,TransformShader,PhysicallyBasedShader,false,false);
	}
	
	void DrawPointLights(lrGL3SceneCache &scene_cache)
	{
		StaticMeshPointLightShader.UseProgram();
		
		lrGL3DShader TransformShader(StaticMeshPointLightShader);
		lrGLPhysicallyBasedShader PhysicallyBasedShader(StaticMeshPointLightShader);
		lrGLPointLightShader PointLightShader(StaticMeshPointLightShader);
		TextureBindingState.SetShader(&PhysicallyBasedShader);
		
		scene_cache.UploadMatrices(TransformShader);
		scene_cache.DrawPointLights(TextureBindingState,TransformShader,PointLightShader,PhysicallyBasedShader);
	}
	
	void DrawReflections(lrGL3SceneCache &scene_cache)
	{
		StaticMeshEnvironmentShader.UseProgram();
		
		lrGL3DShader TransformShader(StaticMeshEnvironmentShader);
		lrGLPhysicallyBasedShader PhysicallyBasedShader(StaticMeshEnvironmentShader);
		TextureBindingState.SetShader(&PhysicallyBasedShader);
		
		scene_cache.UploadMatrices(TransformShader);
		scene_cache.DrawMeshes(TextureBindingState,TransformShader,PhysicallyBasedShader,true,true);
	}
	
public:
	
	lrGLShader &GetStaticMeshPrePassProgram()
	{
		return StaticMeshPrePassShader;
	}
	
	lrGLShader &GetStaticMeshPointLightProgram()
	{
		return StaticMeshPointLightShader;
	}
	
	lrGLShader &GetStaticMeshEnvMapProgram()
	{
		return StaticMeshEnvironmentShader;
	}
	
	lrGLStaticMeshShader GetStaticMeshPointLightShader()
	{
		return lrGLStaticMeshShader(StaticMeshPointLightShader);
	}
	
	lrGLStaticMeshShader GetStaticMeshEnvironmentShader()
	{
		return lrGLStaticMeshShader(StaticMeshEnvironmentShader);
	}
	
	void DrawScene(lrGL3SceneCache &scene_cache)
	{
		TextureBindingState.Reset();
		
		/*
		 * Setting up depth testing
		 */
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_TRUE); // Ez kell, hogy a clear hatással legyen a depth bufferre.
		
		/*
		 * Setting up backface culling
		 */
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		glDisable(GL_BLEND);
		
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		//glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		
		glFinish();
		auto Before = std::chrono::steady_clock::now();
		PrePass(scene_cache);
		glFinish();
		auto After = std::chrono::steady_clock::now();
		
		std::chrono::nanoseconds Difference = After - Before;
		std::cout << "Pre pass time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
		
		//glDepthFunc(GL_EQUAL);
		
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		
		glFinish();
		Before = std::chrono::steady_clock::now();
		DrawReflections(scene_cache);
		glFinish();
		After = std::chrono::steady_clock::now();
		
		Difference = After - Before;
		std::cout << "EnvMap time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
		
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE,GL_ONE);
		
		glFinish();
		Before = std::chrono::steady_clock::now();
		DrawPointLights(scene_cache);
		glFinish();
		After = std::chrono::steady_clock::now();
		
		Difference = After - Before;
		std::cout << "Point light time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
		
	}
	
	lrGL3RenderState3D(lrGLShader &static_mesh_pre_pass_shader,lrGLShader &static_mesh_point_light_shader,lrGLShader &static_mesh_environment_shader)
		:StaticMeshPrePassShader(static_mesh_pre_pass_shader),StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader)
	{}
	
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
	
	virtual void DrawScene() override;
	
public:
	
	lrGL3DCachedLayer(lrGL3RenderState3D &render_state_3D,lrGLResourceLoader &resource_loader)
		:RenderState3D(render_state_3D),ResourceLoader(resource_loader)
	{}
	
	virtual ~lrGL3DCachedLayer() override
	{}
	/*
	 * End of class
	 */
};

#include <lRenderer/lrRenderer/lGLRenderer/lrGLViewport.h>

class lrGL3DCachedLayerFactory : public liLayerFactory
{
private:
	lrGL3RenderState3D &RenderState3D;
	
	lrGLResourceLoader &ResourceLoader;
	
public:
	
	virtual lrLayer *CreateLayer() override
	{
		return new lrGL3DCachedLayer(RenderState3D,ResourceLoader);
	}
	
	lrGL3DCachedLayerFactory(lrGL3RenderState3D &render_state_3D,lrGLResourceLoader &resource_loader)
		:RenderState3D(render_state_3D),ResourceLoader(resource_loader)
	{}
	virtual ~lrGL3DCachedLayerFactory()
	{}
};


#endif // LR_GL3D_CACHED_LAYER_H
