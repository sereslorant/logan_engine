#ifndef LR_GL_MESH_INSTANCES_H
#define LR_GL_MESH_INSTANCES_H


#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>


#include "lrGLData/lrGLMaterialData.h"
#include "lrGLData/lrGLTransformArray.h"

#include <lRenderer/lrRenderer/lrUtils.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLRenderUtils.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3StaticMeshVAO.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lrGL3SkeletalMeshVAO.h>

class lrGL3StaticInstanceSet
{
private:
	lrGL3StaticMeshVAO StaticMeshBinding;
	
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	std::vector<lrGLMaterialData> Materials;
	lrGLTransformArray Transforms;
	
public:
	
	void Construct(liGLStaticMeshShader &static_mesh_shader,lrGLStaticMaterialGroupView &material_group)
	{
		StaticMeshBinding.Construct(static_mesh_shader,material_group);
	}
	
	unsigned int NumInstances()
	{
		return Transforms.GetNumInstances();
	}
	
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		Transforms.Resize(size);
	}
	
	void InitializeMvpMatrices(const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix)
	{
		Transforms.InitializeMvpMatrices(projection_matrix,view_matrix);
	}
	
	void SetInstance(const lrGLMaterialData &material,const lmMatrix4x4 &model_matrix,unsigned int instance_id)
	{
		Materials[instance_id] = material;
		
		Transforms.SetInstance(instance_id,model_matrix);
	}
	
	void Bind()
	{
		StaticMeshBinding.Bind();
	}
	
	void DrawInstances(liGL3DShader &shader,liGLPbMatShader &mat_shader)
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			lrGLRenderUtils::UploadUniformTransform(shader,Transforms.GetInstance(i));
			lrGLRenderUtils::UploadUniformMaterial(mat_shader,Materials[i]);
			
			StaticMeshBinding.Draw();
		}
	}
	
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	
	void Print()
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			std::cout << "\t\tMaterial:\n";
			std::cout << "\t\t" << Materials[i].Material[0][0] << ";" << Materials[i].Material[0][1] << ";" << Materials[i].Material[0][2] << "\n";
			std::cout << "\t\t" << Materials[i].Material[1][0] << ";" << Materials[i].Material[1][1] << ";" << Materials[i].Material[1][2] << ";" << Materials[i].Material[1][3] << "\n";
			std::cout << "\t\tModel matrix:\n";
			for(int j=0;j < 4;j++)
			{
				std::cout << "\t\t";
				for(int k=0;k < 4;k++)
				{
					std::cout << ModelMatrices[i][j][k] << ";";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
	}
	
	#endif
	
	lrGL3StaticInstanceSet()
	{}
	
	~lrGL3StaticInstanceSet()
	{}
	/*
	 * End of class
	 */
};

using lrBoneTransform = std::vector<lmMatrix4x4>;

class lrGL3SkeletalInstanceSet
{
private:
	lrGL3SkeletalMeshVAO SkeletalMeshBinding;
	
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	std::vector<lrGLMaterialData> Materials;
	lrGLTransformArray Transforms;
	std::vector<lrBoneTransform> BoneTransforms;
	
public:
	
	void Construct(liGLStaticMeshShader &static_mesh_shader,liGLSkeletalMeshShader &skeletal_mesh_shader,lrGLSkeletalMaterialGroupView &material_group)
	{
		SkeletalMeshBinding.Construct(static_mesh_shader,skeletal_mesh_shader,material_group);
	}
	
	unsigned int NumInstances()
	{
		return Transforms.GetNumInstances();
	}
	
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		Transforms.Resize(size);
		BoneTransforms.resize(size);
	}
	
	void InitializeMvpMatrices(const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix)
	{
		Transforms.InitializeMvpMatrices(projection_matrix,view_matrix);
	}
	
	void SetInstance(const lrGLMaterialData &material,const lmMatrix4x4 &model_matrix,const lrBoneTransform &bone_transform,unsigned int instance_id)
	{
		Materials[instance_id] = material;
		Transforms.SetInstance(instance_id,model_matrix);
		BoneTransforms[instance_id] = bone_transform;
	}
	
	void Bind()
	{
		SkeletalMeshBinding.Bind();
	}
	
	void DrawInstances(liGL3DShader &shader,liGLSkeletalMeshShader &skeletal_mesh_shader,liGLPbMatShader &mat_shader)
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			lrGLRenderUtils::UploadUniformTransform(shader,Transforms.GetInstance(i));
			lrGLRenderUtils::UploadUniformMaterial(mat_shader,Materials[i]);
			
			lrBoneTransform &CurrentTransform = BoneTransforms[i];
			glUniformMatrix4fv(skeletal_mesh_shader.GetBoneTransformLocation(),CurrentTransform.size(),GL_FALSE,CurrentTransform[0][0]);
			
			SkeletalMeshBinding.Draw();
		}
	}
	
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	
	void Print()
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			std::cout << "\t\tMaterial:\n";
			std::cout << "\t\t" << Materials[i].Material[0][0] << ";" << Materials[i].Material[0][1] << ";" << Materials[i].Material[0][2] << "\n";
			std::cout << "\t\t" << Materials[i].Material[1][0] << ";" << Materials[i].Material[1][1] << ";" << Materials[i].Material[1][2] << ";" << Materials[i].Material[1][3] << "\n";
			std::cout << "\t\tModel matrix:\n";
			for(int j=0;j < 4;j++)
			{
				std::cout << "\t\t";
				for(int k=0;k < 4;k++)
				{
					std::cout << ModelMatrices[i][j][k] << ";";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
	}
	
	#endif
	
	lrGL3SkeletalInstanceSet()
	{}
	
	~lrGL3SkeletalInstanceSet()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_MESH_INSTANCES_H
