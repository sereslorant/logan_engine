#ifndef LR_GL3_RENDERER_H
#define LR_GL3_RENDERER_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLIncludes.h>

#include <lRenderer/lrRenderer/lGLRenderer/lrGLFramebuffer.h>
#include <lRenderer/lrRenderer/lGLRenderer/lrGLViewport.h>

#include "lGL2DLayer/lrGL2DLayer.h"
#include "lGL3DCachedLayer/lrGL3DCachedLayer.h"
#include "lGL3DDumbLayer/lrGL3DDumbLayer.h"

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShaderLibrary.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>

class lrGL3Renderer : public liRenderer
{
private:
	lrGLShaderLibrary ShaderLibrary;
	lrGLResourceLoader ResourceLoader;
	
	lrGLShader Shader2D;
	
	lrGLShader StaticMeshPrePassShader;
	lrGLShader StaticMeshPointLightShader;
	lrGLShader StaticMeshEnvironmentShader;
	
	lrGL3RenderState3D RenderState3D;
	
	lrGL2DLayerFactory Layer2DFactory;
	
	lrGL3DCachedLayerFactory Layer3DFactory;
	//lrGL3DDumbLayerFactory Layer3DFactory;
	
	lrGLFramebuffer MainFramebuffer;
	
public:
	
	virtual void SetResourceManager(liResourceManager *resource_manager) override
	{
		ResourceLoader.SetResourceManager(resource_manager);
	}
	
	virtual liFramebuffer2D &GetMainFramebuffer() override
	{
		return MainFramebuffer;
	}
	
	virtual void Render() override
	{
		MainFramebuffer.Draw();
	}
	
	lrGL3Renderer(unsigned int width,unsigned int height)
		:RenderState3D(StaticMeshPrePassShader,StaticMeshPointLightShader,StaticMeshEnvironmentShader),
		 Layer2DFactory(Shader2D),
		 Layer3DFactory(RenderState3D,ResourceLoader),
		 MainFramebuffer(width,height,Layer2DFactory,Layer3DFactory)
	{
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_SCISSOR_TEST);
		
		GLuint Shader2DShaders[2];
		Shader2DShaders[0] = ShaderLibrary.GetShader("VertexShader2D");
		Shader2DShaders[1] = ShaderLibrary.GetShader("FragmentShader2D");
		Shader2D.Initialize(Shader2DShaders,2);
		
		GLuint ShaderPrepassShaders[2];
		ShaderPrepassShaders[0] = ShaderLibrary.GetShader("StaticMeshPrepassVertexShader");
		ShaderPrepassShaders[1] = ShaderLibrary.GetShader("PrepassFragmentShader");
		StaticMeshPrePassShader.Initialize(ShaderPrepassShaders,2);
		
		GLuint PointLightShaders[4];
		PointLightShaders[0] = ShaderLibrary.GetShader("StaticMeshVertexShader");
		PointLightShaders[1] = ShaderLibrary.GetShader("PbEquationsShader");
		PointLightShaders[2] = ShaderLibrary.GetShader("FwdRendererShader");
		PointLightShaders[3] = ShaderLibrary.GetShader("PbFragmentShaderMain");
		StaticMeshPointLightShader.Initialize(PointLightShaders,4);
		
		GLuint EnvMapShaders[4];
		EnvMapShaders[0] = ShaderLibrary.GetShader("StaticMeshVertexShader");
		EnvMapShaders[1] = ShaderLibrary.GetShader("PbEquationsShader");
		EnvMapShaders[2] = ShaderLibrary.GetShader("FwdRendererShader");
		EnvMapShaders[3] = ShaderLibrary.GetShader("PbEnvMapShaderMain");
		StaticMeshEnvironmentShader.Initialize(EnvMapShaders,4);
	}
	
	virtual ~lrGL3Renderer() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL3_RENDERER_H
