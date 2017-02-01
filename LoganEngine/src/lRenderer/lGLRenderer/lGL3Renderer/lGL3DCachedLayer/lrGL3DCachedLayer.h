#ifndef LR_GL3D_CACHED_LAYER_H
#define LR_GL3D_CACHED_LAYER_H

#include "../../../lrLayer.h"

#include "../../lGLResources/lrGLResourceLoader.h"


#include "lrGL3SceneCache/lrGL3SceneCache.h"

#include "../lGL3Shaders/lrGL3StaticMeshShader.h"

class lrGL3RenderState3D
{
private:
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
		/*
		 * TODO: A shadert ki kell majd cserélni.
		 */
		//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		lrGL3StaticMeshShader &PrePassShader = StaticMeshPointLightShader;
		//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
		PrePassShader.UseProgram();
		//
		scene_cache.UploadMatrices(PrePassShader);
		scene_cache.DrawMeshes(TextureBindingState,PrePassShader,false,false);
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
		//
		scene_cache.UploadMatrices(StaticMeshPointLightShader);
		scene_cache.DrawPointLights(TextureBindingState,StaticMeshPointLightShader);
	}
	//
	void DrawReflections(lrGL3SceneCache &scene_cache)
	{
		StaticMeshEnvironmentShader.UseProgram();
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
		Prepare();
		//
		PreparePrePass();
		PrePass(scene_cache);
		//
		PrepareDraw();
		DrawPointLights(scene_cache);
		DrawReflections(scene_cache);
	}
	//
	lrGL3RenderState3D(lrGL3StaticMeshShader &static_mesh_point_light_shader,lrGL3StaticMeshShader &static_mesh_environment_shader)
		:StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader)
	{}
	//
	virtual ~lrGL3RenderState3D()
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
