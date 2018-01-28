#ifndef LR_GL_SKELETAL_MATERIAL_GROUP_VIEW_H
#define LR_GL_SKELETAL_MATERIAL_GROUP_VIEW_H

#include "../lrGLMeshData/lrGLVertexData.h"
#include "../lrGLMeshData/lrGLBoneData.h"
#include "../lrGLMeshData/lrGLPrimitiveData.h"

#include "../../../liGLShaderInterfaces.h"

class lrGLSkeletalMaterialGroupView
{
private:
	lrGLVertexData		*VertexData		= nullptr;
	lrGLBoneData		*BoneData		= nullptr;
	lrGLPrimitiveData	*MaterialGroup	= nullptr;
	
public:
	
	const std::string &GetMaterial()
	{
		return MaterialGroup->MaterialName;
	}
	
	void BindAttributes(liGLStaticMeshShader &static_mesh_shader);
	
	void BindBoneData(liGLSkeletalMeshShader &skeletal_mesh_shader);
	
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
	lrGLSkeletalMaterialGroupView()
	{
		//Empty
	}
	
	lrGLSkeletalMaterialGroupView(lrGLVertexData *vertex_data,lrGLBoneData *bone_data,lrGLPrimitiveData *material_group)
		:VertexData(vertex_data),BoneData(bone_data),MaterialGroup(material_group)
	{
		//Empty
	}
	
	~lrGLSkeletalMaterialGroupView()
	{
		//Empty
	}
	/*
	 * End of class
	 */
};

#endif // LR_GL_SKELETAL_MATERIAL_GROUP_VIEW_H
