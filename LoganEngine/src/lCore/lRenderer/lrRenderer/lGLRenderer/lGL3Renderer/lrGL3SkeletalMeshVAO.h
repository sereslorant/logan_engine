#ifndef LR_GL3_SKELETAL_MESH_VAO_H
#define LR_GL3_SKELETAL_MESH_VAO_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLResources/lrGLResourceLoader.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLVertexArray.h>

class lrGL3SkeletalMeshVAO
{
private:
	bool MeshInitialized = false;
	lrGLVertexArray VertexArray;
	lrGLSkeletalMaterialGroupView MaterialGroup;
	
public:
	
	void Construct(liGLStaticMeshShader &static_mesh_shader,liGLSkeletalMeshShader &skeletal_mesh_shader,lrGLSkeletalMaterialGroupView &material_group)
	{
		if(!MeshInitialized)
		{
			VertexArray.Create();
			VertexArray.Bind();
			
			MaterialGroup = material_group;
			
			MaterialGroup.BindAttributes(static_mesh_shader);
			MaterialGroup.BindBoneData(skeletal_mesh_shader);
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
	
	lrGL3SkeletalMeshVAO()
	{}
	
	~lrGL3SkeletalMeshVAO()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL3_SKELETAL_MESH_VAO_H
