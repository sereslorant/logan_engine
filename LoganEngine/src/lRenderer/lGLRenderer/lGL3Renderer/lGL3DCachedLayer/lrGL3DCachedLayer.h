#ifndef LR_GL3D_CACHED_LAYER_H
#define LR_GL3D_CACHED_LAYER_H


#include "../../../lrLayer.h"

#include "../lrGL3StaticMeshShader.h"
#include "../../lGLResources/lrGLResourceLoader.h"

class lrGL3DCachedLayer : public lr3DLayer
{
private:
	lrGL3StaticMeshShader &StaticMeshPointLightShader;
	lrGL3StaticMeshShader &StaticMeshEnvironmentShader;
	lrGLResourceLoader &ResourceLoader;
	//
	virtual void DrawScene() override;
	//
public:
	//
	lrGL3DCachedLayer(lrGL3StaticMeshShader &static_mesh_point_light_shader,lrGL3StaticMeshShader &static_mesh_environment_shader,lrGLResourceLoader &resource_loader)
		:StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader),ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGL3DCachedLayer() override
	{}
	/*
	 * End of class
	 */
};


#endif // LR_GL3D_CACHED_LAYER_H
