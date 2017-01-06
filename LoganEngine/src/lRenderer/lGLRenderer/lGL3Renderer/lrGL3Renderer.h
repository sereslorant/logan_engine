#ifndef LR_GL3_RENDERER_H
#define LR_GL3_RENDERER_H

#include "../lGLIncludes.h"
#include "../lrGLViewport.h"
#include "../lrGLFramebuffer.h"

#include "lGL3DCachedLayer/lrGL3DCachedLayer.h"

#include "../lGLResources/lrGLResourceLoader.h"

class lrGL3Viewport : public lrGLViewport
{
private:
	lrGL3StaticMeshShader &StaticMeshPointLightShader;
	lrGL3StaticMeshShader &StaticMeshEnvironmentShader;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual lrLayer *CreateGL2DLayer() override
	{
		return nullptr;
	}
	//
	virtual lrLayer *CreateGL3DLayer() override
	{
		return new lrGL3DCachedLayer(StaticMeshPointLightShader,StaticMeshEnvironmentShader,ResourceLoader);
	}
	//
public:
	//
	lrGL3Viewport(int x,int y,int width,int height,lrGL3StaticMeshShader &static_mesh_point_light_shader,lrGL3StaticMeshShader &static_mesh_environment_shader,lrGLResourceLoader &resource_loader)
		:lrGLViewport(x,y,width,height),StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader),ResourceLoader(resource_loader)
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
	lrGL3StaticMeshShader &StaticMeshPointLightShader;
	lrGL3StaticMeshShader &StaticMeshEnvironmentShader;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual lrGLViewport *CreateGLViewport(int x,int y,int width,int height) override
	{
		return new lrGL3Viewport(x,y,width,height,StaticMeshPointLightShader,StaticMeshEnvironmentShader,ResourceLoader);
	}
	//
public:
	//
	lrGL3Framebuffer(int width,int height,lrGL3StaticMeshShader &static_mesh_point_light_shader,lrGL3StaticMeshShader &static_mesh_environment_shader,lrGLResourceLoader &resource_loader)
		:lrGLFramebuffer(width,height),StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader),ResourceLoader(resource_loader)
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
	lrGL3StaticMeshShader StaticMeshPointLightShader;
	lrGL3StaticMeshShader StaticMeshEnvironmentShader;
	lrGLResourceLoader ResourceLoader;
	//
	lrGL3Framebuffer MainFramebuffer;
	//
public:
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
	lrGL3Renderer(unsigned int width,unsigned int height,liResourceManager &resource_manager)
		://StaticMeshPointLightShader(VertexShaderSource,PbEquationsSource,FwdFragmentShaderSrc,FragmentShaderSource),
		 //StaticMeshEnvironmentShader(VertexShaderSource,PbEquationsSource,FwdFragmentShaderSrc,EnvMapShaderSource),
		 ResourceLoader(resource_manager),
		 MainFramebuffer(width,height,StaticMeshPointLightShader,StaticMeshEnvironmentShader,ResourceLoader)
	{
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_SCISSOR_TEST);
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
