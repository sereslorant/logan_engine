#ifndef LR_GL_STATIC_MATERIAL_GROUP_VIEW_H
#define LR_GL_STATIC_MATERIAL_GROUP_VIEW_H

#include "../lrGLMeshData/lrGLVertexData.h"
#include "../lrGLMeshData/lrGLPrimitiveData.h"

#include "../../../liGLShaderInterfaces.h"

class lrGLStaticMaterialGroupView
{
private:
	lrGLVertexData		*VertexData = nullptr;
	lrGLPrimitiveData	*MaterialGroup = nullptr;
	
public:
	
	const std::string &GetMaterial()
	{
		return MaterialGroup->MaterialName;
	}
	
	void BindAttributes(liGLStaticMeshShader &static_mesh_shader);
	
	void BindIndexBuffer();
	
	void Draw();
	/*
	void DrawInstanced(unsigned int num_instances)
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,MaterialGroup->IndexBuffer);
			glDrawElementsInstanced(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr,num_instances);
		}
	}
	*/
	lrGLStaticMaterialGroupView()
	{}
	
	lrGLStaticMaterialGroupView(lrGLVertexData *vertex_data,lrGLPrimitiveData *material_group)
		:VertexData(vertex_data),MaterialGroup(material_group)
	{}
	
	~lrGLStaticMaterialGroupView()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_STATIC_MATERIAL_GROUP_VIEW_H
