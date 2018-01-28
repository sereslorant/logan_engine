#ifndef LR_GL3_STATIC_MESH_VAO_H
#define LR_GL3_STATIC_MESH_VAO_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLVertexArray.h>

class lrGL3StaticMeshVAO
{
private:
	bool MeshInitialized = false;
	lrGLVertexArray VertexArray;
	lrGLStaticMaterialGroupView MaterialGroup;
	
public:
	
	void Construct(liGLStaticMeshShader &static_mesh_shader,lrGLStaticMaterialGroupView &material_group)
	{
		if(!MeshInitialized)
		{
			VertexArray.Create();
			VertexArray.Bind();
			
			MaterialGroup = material_group;
			
			MaterialGroup.BindAttributes(static_mesh_shader);
			MaterialGroup.BindIndexBuffer();
			
			glBindVertexArray(0);
			
			MeshInitialized = true;
		}
	}
	
	void Bind()
	{
		VertexArray.Bind();
	}
	
	void Draw()
	{
		MaterialGroup.Draw();
	}
	
	lrGL3StaticMeshVAO()
	{}
	
	~lrGL3StaticMeshVAO()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL3_STATIC_MESH_VAO_H
