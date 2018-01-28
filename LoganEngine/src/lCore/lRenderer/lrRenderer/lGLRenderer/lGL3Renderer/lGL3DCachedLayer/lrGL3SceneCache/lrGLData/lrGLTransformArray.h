#ifndef LR_GL_TRANSFORM_ARRAY_H
#define LR_GL_TRANSFORM_ARRAY_H

#include <lMath/lmMatrix3x3.hpp>
#include <lMath/lmMatrix4x4.hpp>

#include <vector>

struct lrGLTransform
{
	lmMatrix4x4 &ModelMatrix;
	lmMatrix3x3 &NormalMatrix;
	lmMatrix4x4 &MvpMatrix;
	
	lrGLTransform(const lrGLTransform &t)
		:ModelMatrix(t.ModelMatrix),NormalMatrix(t.NormalMatrix),MvpMatrix(t.MvpMatrix)
	{}
	
	lrGLTransform(lmMatrix4x4 &model_matrix,lmMatrix3x3 &normal_matrix,lmMatrix4x4 &mvp_matrix)
		:ModelMatrix(model_matrix),NormalMatrix(normal_matrix),MvpMatrix(mvp_matrix)
	{}
};

struct lrGLTransformArray
{
	std::vector<lmMatrix4x4> ModelMatrices;
	std::vector<lmMatrix3x3> NormalMatrices;
	std::vector<lmMatrix4x4> MvpMatrices;
	
	unsigned int GetNumInstances()
	{
		return ModelMatrices.size();
	}
	
	void Resize(unsigned int size)
	{
		ModelMatrices.resize(size);
		NormalMatrices.resize(size);
		MvpMatrices.resize(size);
	}
	
	void SetInstance(unsigned int instance_id,const lmMatrix4x4 &model_matrix)
	{
		ModelMatrices[instance_id] = model_matrix;
		
		lmMatrix3x3 InvModelMatrix;
		lmInverse(ModelMatrices[instance_id].GetSubMatrix(3,3),InvModelMatrix);
		InvModelMatrix.Transpose(NormalMatrices[instance_id]);
	}
	
	void InitializeMvpMatrices(const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix)
	{
		for(int i=0;i < MvpMatrices.size();i++)
		{
			MvpMatrices[i] = projection_matrix * view_matrix * ModelMatrices[i];
		}
	}
	
	lrGLTransform GetInstance(unsigned int id)
	{
		return lrGLTransform(ModelMatrices[id],NormalMatrices[id],MvpMatrices[id]);
	}
	
	lrGLTransformArray()
	{}
	
	lrGLTransformArray(unsigned int size)
		:ModelMatrices(size),NormalMatrices(size),MvpMatrices(size)
	{}
};

#endif // LR_GL_TRANSFORM_ARRAY_H
