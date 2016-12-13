#ifndef LR_GL3_RENDERER_H
#define LR_GL3_RENDERER_H

#include "../lGLIncludes.h"
#include "../lrGLViewport.h"
#include "../lrGLFramebuffer.h"

#include "../../lr3DSceneReader.h"

//#define L_DEBUG_PRINT_SCENE_CACHE


#include "../../../lRenderer/lrLayer.h"

#include "lrGL3SceneCache.h"

#include "lrGL3StaticMeshShader.h"
#include "../lGLResources/lrGLResourceLoader.h"

class lrGL3DCachedLayer : public lr3DLayer
{
private:
	lrGL3StaticMeshShader &StaticMeshShader;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual void DrawScene() override
	{
		if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
		{return;}
		//
		lrGLModelCacher ModelCacher(ResourceLoader);
		lr3DSceneCacheStats SceneCacheStats(ModelCacher);
        lr3DSceneReader SceneReader(SceneCacheStats);
        Scene->Draw(SceneReader);
        //
        lrGL3SceneCache SceneCache(*Frustum,*Camera,SceneCacheStats,StaticMeshShader,ResourceLoader);
        //
        lr3DSceneReader SceneCacher(SceneCache.GetSceneCacher());
        //
        Scene->Draw(SceneCacher);
        //
		#ifdef L_DEBUG_PRINT_SCENE_CACHE
        	SceneCache.Print();
		#endif
        //
        SceneCache.DrawScene();
	}
	//
public:
	//
	lrGL3DCachedLayer(lrGL3StaticMeshShader &static_mesh_shader,lrGLResourceLoader &resource_loader)
		:StaticMeshShader(static_mesh_shader),ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGL3DCachedLayer() override
	{}
	/*
	 * End of class
	 */
};

#include "../lGLResources/lrGLResourceLoader.h"

class lrGL3Viewport : public lrGLViewport
{
private:
	lrGL3StaticMeshShader &StaticMeshShader;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual lrLayer *CreateGL2DLayer() override
	{
		return nullptr;
	}
	//
	virtual lrLayer *CreateGL3DLayer() override
	{
		return new lrGL3DCachedLayer(StaticMeshShader,ResourceLoader);
	}
	//
public:
	//
	lrGL3Viewport(int x,int y,int width,int height,lrGL3StaticMeshShader &static_mesh_shader,lrGLResourceLoader &resource_loader)
		:lrGLViewport(x,y,width,height),StaticMeshShader(static_mesh_shader),ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGL3Viewport() override
	{}
	/*
	 * End of class
	 */
};

#include "../lGLResources/lrGLResourceLoader.h"

class lrGL3Framebuffer : public lrGLFramebuffer
{
private:
	lrGL3StaticMeshShader &StaticMeshShader;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual lrGLViewport *CreateGLViewport(int x,int y,int width,int height) override
	{
		return new lrGL3Viewport(x,y,width,height,StaticMeshShader,ResourceLoader);
	}
	//
public:
	//
	lrGL3Framebuffer(int width,int height,lrGL3StaticMeshShader &static_mesh_shader,lrGLResourceLoader &resource_loader)
		:lrGLFramebuffer(width,height),StaticMeshShader(static_mesh_shader),ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGL3Framebuffer() override
	{}
	/*
	 * End of class
	 */
};

class lrGL3Renderer : public liRenderer
{
private:
	//
	lrGL3StaticMeshShader StaticMeshShader;
	lrGLResourceLoader ResourceLoader;
	//
	lrGL3Framebuffer MainFramebuffer;
	//
public:
	//
	virtual liFramebuffer &GetMainFramebuffer() override
	{
		return MainFramebuffer;
	}
	//
	virtual void Render() override
	{
		MainFramebuffer.Draw();
	}
	//
	lrGL3Renderer(unsigned int width,unsigned int height,liResourceManager &resource_manager)
		:StaticMeshShader(VertexShaderSource,FragmentShaderSource),ResourceLoader(resource_manager),MainFramebuffer(width,height,StaticMeshShader,ResourceLoader)
	{
		glEnable(GL_BLEND);
		glEnable(GL_SCISSOR_TEST);
	}
	//
	virtual ~lrGL3Renderer() override
	{
    	//
	}
	/*
	 * End of class
	 */
};

#endif // LR_GL3_RENDERER_H
