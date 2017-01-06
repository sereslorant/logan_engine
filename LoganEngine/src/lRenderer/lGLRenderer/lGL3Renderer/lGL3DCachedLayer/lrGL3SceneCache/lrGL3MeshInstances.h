#ifndef LR_GL_MESH_INSTANCES_H
#define LR_GL_MESH_INSTANCES_H

#include "../../../lGLResources/lrGLResourceLoader.h"
#include "../../../../lrUtils.h"

#include "../../../liGLShaderInterfaces.h"
#include "lrGLData/lrGLMaterialData.h"

class lrGL3MeshInstances
{
private:
	//
	bool MeshInitialized = false;
	lrGLMaterialGroupView MaterialGroup;
	GLuint VertexArrayObject = 0;
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	unsigned int Next = 0;
	std::vector<lrGLMaterialData> Materials;
	std::vector<lmMatrix4x4> ModelMatrices;
	//std::vector<lmMatrix4x4> MvMatrices;
	std::vector<lmMatrix4x4> MvpMatrices;
	std::vector<lmMatrix3x3> NormalMatrices;
	//
	void ApplyMatrix(liGLShader &shader,unsigned int instance_id)
	{
		glUniformMatrix4fv(shader.GetModelMatrixLocation(),1,GL_FALSE,ModelMatrices[instance_id][0]);
		glUniformMatrix3fv(shader.GetNormalMatrixLocation(),1,GL_FALSE,NormalMatrices[instance_id][0]);
		glUniformMatrix4fv(shader.GetMvpMatrixLocation(),1,GL_FALSE,MvpMatrices[instance_id][0]);
	}
	//
	void ApplyMaterial(liGLPbMatShader &shader,unsigned int instance_id)
	{
		lrGLMaterialData &Material = Materials[instance_id];
		//
		glUniform3fv(shader.GetMatAlbedoLocation(),1,Material.Material[0]);
		glUniform4fv(shader.GetMaterial1Location(),1,Material.Material[1]);
	}
	//
public:
	//
	void Construct(liGLStaticMeshShader &static_mesh_shader,lrGLMaterialGroupView &material_group)
	{
		if(!MeshInitialized)
		{
			glGenVertexArrays(1,&VertexArrayObject);
			glBindVertexArray(VertexArrayObject);
			//
			MaterialGroup = material_group;
			MaterialGroup.BindAttributes(static_mesh_shader);
			//
			glBindVertexArray(0);
			//
			MeshInitialized = true;
		}
	}
	//
	void DrawInstances(liGLShader &shader,liGLPbMatShader &mat_shader)
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			ApplyMatrix(shader,i);
			ApplyMaterial(mat_shader,i);
			//
			glBindVertexArray(VertexArrayObject);
			MaterialGroup.Draw();
			glBindVertexArray(0);
		}
	}
	//
	/*
	 * Minden más!
	 */
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	//
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
	//
	#endif
	//
	unsigned int NumInstances()
	{
		return (unsigned int)ModelMatrices.size();
	}
	//
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		ModelMatrices.resize(size);
		//MvMatrices.resize(size);
		MvpMatrices.resize(size);
		NormalMatrices.resize(size);
		Next = 0;
	}
	//
	void SetNextInstance(lmMatrix4x4 &projection_matrix,lmMatrix4x4 &view_matrix,const liMaterial &material,const li3DMesh &mesh)
	{
		if(Next < NumInstances())
		{
			SetInstance(projection_matrix,view_matrix,material,mesh,Next);
			Next++;
		}
	}
	//
	void SetInstance(lmMatrix4x4 &projection_matrix,lmMatrix4x4 &view_matrix,const liMaterial &material,const li3DMesh &mesh,unsigned int instance_id)
	{
		if(instance_id < NumInstances())
		{
			Materials[instance_id].SetMaterial(material);
			ModelMatrices[instance_id] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			lrUtils::GetModelMatrix(mesh,ModelMatrices[instance_id]);
			//
			//MvMatrices[instance_id] = view_matrix * ModelMatrices[instance_id];
			//MvpMatrices[instance_id] = projection_matrix MvMatrices[instance_id];
			MvpMatrices[instance_id] = projection_matrix * view_matrix * ModelMatrices[instance_id];
			ModelMatrices[instance_id].GetSubMatrix(3,3).Invert().Transpose(NormalMatrices[instance_id]);
		}
	}
	//
	lrGL3MeshInstances()
	{}
	//
	~lrGL3MeshInstances()
	{
		glDeleteVertexArrays(1,&VertexArrayObject);
	}
};

#endif // LR_GL_MESH_INSTANCES_H
