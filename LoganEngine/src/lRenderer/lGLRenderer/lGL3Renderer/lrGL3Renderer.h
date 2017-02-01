#ifndef LR_GL3_RENDERER_H
#define LR_GL3_RENDERER_H

#include "../lGLIncludes.h"
#include "../lrGLViewport.h"
#include "../lrGLFramebuffer.h"

#include "lGL2DLayer/lrGL2DLayer.h"
#include "lGL3DCachedLayer/lrGL3DCachedLayer.h"

#include "../lGLResources/lrGLResourceLoader.h"

class lrGL3Viewport : public lrGLViewport
{
private:
	lrGL2DShader &Shader2D;
	//
	lrGL3RenderState3D &RenderState3D;
	//
	lrGLResourceLoader &ResourceLoader;
	//
	virtual lrLayer *CreateGL2DLayer() override
	{
		return new lrGL2DLayer(Shader2D);
	}
	//
	virtual lrLayer *CreateGL3DLayer() override
	{
		return new lrGL3DCachedLayer(RenderState3D,ResourceLoader);
	}
	//
public:
	//
	lrGL3Viewport(int x,int y,int width,int height,lrGL2DShader &shader2d,lrGL3RenderState3D &render_state_3D,lrGLResourceLoader &resource_loader)
		:lrGLViewport(x,y,width,height),Shader2D(shader2d),RenderState3D(render_state_3D),ResourceLoader(resource_loader)
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
	lrGL2DShader &Shader2D;
	//
	lrGL3RenderState3D &RenderState3D;
	//
	lrGLResourceLoader &ResourceLoader;
	//
	virtual lrGLViewport *CreateGLViewport(int x,int y,int width,int height) override
	{
		return new lrGL3Viewport(x,y,width,height,Shader2D,RenderState3D,ResourceLoader);
	}
	//
public:
	//
	lrGL3Framebuffer(int width,int height,lrGL2DShader &shader2d,lrGL3RenderState3D &render_state_3D,lrGLResourceLoader &resource_loader)
		:lrGLFramebuffer(width,height),Shader2D(shader2d),RenderState3D(render_state_3D),ResourceLoader(resource_loader)
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
	lrGL3ShaderLibrary ShaderLibrary;
	//
	lrGL2DShader Shader2D;
	//
	lrGL3StaticMeshShader StaticMeshPointLightShader;
	lrGL3StaticMeshShader StaticMeshEnvironmentShader;
	//
	lrGL3RenderState3D RenderState3D;
	lrGLResourceLoader ResourceLoader;
	//
	lrGL3Framebuffer MainFramebuffer;
	//
public:
	//
	virtual void SetResourceManager(liResourceManager *resource_manager) override
	{
		ResourceLoader.SetResourceManager(resource_manager);
	}
	//
	virtual liFramebuffer2D &GetMainFramebuffer() override
	{
		return MainFramebuffer;
	}
	//
	virtual void Render() override
	{
		MainFramebuffer.Draw();
	}
	//
	lrGL3Renderer(unsigned int width,unsigned int height)
		://StaticMeshPointLightShader(VertexShaderSource,PbEquationsSource,FwdFragmentShaderSrc,FragmentShaderSource),
		 //StaticMeshEnvironmentShader(VertexShaderSource,PbEquationsSource,FwdFragmentShaderSrc,EnvMapShaderSource),
		 RenderState3D(StaticMeshPointLightShader,StaticMeshEnvironmentShader),
		 MainFramebuffer(width,height,Shader2D,RenderState3D,ResourceLoader)
	{
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_SCISSOR_TEST);
		//
		GLuint Shader2DShaders[2];
		Shader2DShaders[0] = ShaderLibrary.GetShader("VertexShader2D");
		Shader2DShaders[1] = ShaderLibrary.GetShader("FragmentShader2D");
		Shader2D.Initialize(Shader2DShaders,2);
		//
		GLuint PointLightShaders[4];
		PointLightShaders[0] = ShaderLibrary.GetShader("StaticMeshVertexShader");
		PointLightShaders[1] = ShaderLibrary.GetShader("PbEquationsShader");
		PointLightShaders[2] = ShaderLibrary.GetShader("FwdRendererShader");
		PointLightShaders[3] = ShaderLibrary.GetShader("PbFragmentShaderMain");
		StaticMeshPointLightShader.Initialize(PointLightShaders,4);
		//
		GLuint EnvMapShaders[4];
		EnvMapShaders[0] = ShaderLibrary.GetShader("StaticMeshVertexShader");
		EnvMapShaders[1] = ShaderLibrary.GetShader("PbEquationsShader");
		EnvMapShaders[2] = ShaderLibrary.GetShader("FwdRendererShader");
		EnvMapShaders[3] = ShaderLibrary.GetShader("PbEnvMapShaderMain");
		StaticMeshEnvironmentShader.Initialize(EnvMapShaders,4);
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
