
#include "lrGLSkeletalMaterialGroupView.h"

void lrGLSkeletalMaterialGroupView::BindAttributes(liGLStaticMeshShader &static_mesh_shader)
{
	if(MaterialGroup == nullptr)
		{return;}
	
	if(VertexData != nullptr)
	{
		GLint PositionLocation  = static_mesh_shader.GetVertexLocation();
		GLint NormalLocation    = static_mesh_shader.GetNormalLocation();
		GLint TangentLocation   = static_mesh_shader.GetTangentLocation();
		GLint BitangentLocation = static_mesh_shader.GetBitangentLocation();
		GLint TexCoordLocation  = static_mesh_shader.GetTexCoordLocation();
		
		VertexData->VertexBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(PositionLocation);
		glVertexAttribPointer(PositionLocation,3,GL_FLOAT,GL_FALSE,0,0);
		
		VertexData->NormalBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(NormalLocation);
		glVertexAttribPointer(NormalLocation,3,GL_FLOAT,GL_FALSE,0,0);
		
		VertexData->TangentBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(TangentLocation);
		glVertexAttribPointer(TangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
		
		VertexData->BitangentBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(BitangentLocation);
		glVertexAttribPointer(BitangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
		
		VertexData->TexCoordBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(TexCoordLocation);
		glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);
	}
}

void lrGLSkeletalMaterialGroupView::BindBoneData(liGLSkeletalMeshShader &skeletal_mesh_shader)
{
	if(MaterialGroup == nullptr)
		{return;}
	
	if(BoneData != nullptr)
	{
		GLint BoneIdLocation	= skeletal_mesh_shader.GetBoneIdLocation();
		GLint WeightLocation	= skeletal_mesh_shader.GetWeightLocation();
		
		//glBindBuffer(GL_ARRAY_BUFFER,BoneData->BoneIdBuffer);
		BoneData->BoneIdBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(BoneIdLocation);
		glVertexAttribIPointer(BoneIdLocation,lrmSkeletalMesh::MAX_WEIGHT_COUNT,GL_INT,0,0);
		
		//glBindBuffer(GL_ARRAY_BUFFER,BoneData->WeightBuffer);
		BoneData->WeightBuffer.BindBuffer(GL_ARRAY_BUFFER);
		glEnableVertexAttribArray(WeightLocation);
		glVertexAttribPointer(WeightLocation,lrmSkeletalMesh::MAX_WEIGHT_COUNT,GL_FLOAT,GL_FALSE,0,0);
	}
}

void lrGLSkeletalMaterialGroupView::BindIndexBuffer()
{
	if(VertexData != nullptr && BoneData != nullptr && MaterialGroup != nullptr)
	{
		MaterialGroup->IndexBuffer.BindBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void lrGLSkeletalMaterialGroupView::Draw()
{
	if(VertexData != nullptr && BoneData != nullptr && MaterialGroup != nullptr)
	{
		glDrawElements(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr);
	}
}
