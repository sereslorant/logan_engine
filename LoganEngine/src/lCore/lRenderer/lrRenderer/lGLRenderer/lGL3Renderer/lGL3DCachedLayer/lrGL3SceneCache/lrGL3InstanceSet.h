#ifndef LR_GL_MESH_INSTANCES_H
#define LR_GL_MESH_INSTANCES_H

#include "../../../lGLResources/lrGLResourceLoader.h"
#include "../../../../lrUtils.h"

#include "../../../liGLShaderInterfaces.h"
#include "lrGLData/lrGLMaterialData.h"

class liGL3InstanceSet
{
public:
	//
	virtual unsigned int NumInstances() = 0;
	virtual void SetInstance(const liMaterial &material,const li3DMesh &mesh,unsigned int instance_id) = 0;
	virtual void InitializeMvpMatrices(const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix) = 0;
	//
	//virtual void DrawInstances(liGLShader &shader,liGLPbMatShader &mat_shader) = 0;
	//
	liGL3InstanceSet(){}
	//
	virtual ~liGL3InstanceSet(){}
	/*
	 * End of class
	 */
};

class lrGL3InstanceSetBase
{
protected:
	bool MeshInitialized = false;
	lrGLMaterialGroupView MaterialGroup;
	GLuint VertexArrayObject = 0;
	//
	//virtual void BindInstanceData()
	//{
		//
	//}
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
			//
			MaterialGroup.BindAttributes(static_mesh_shader);
			MaterialGroup.BindIndexBuffer();
			//
			//BindInstanceData();
			//
			glBindVertexArray(0);
			//
			MeshInitialized = true;
		}
	}
	//
	lrGL3InstanceSetBase()
	{}
	//
	virtual ~lrGL3InstanceSetBase()
	{
		glDeleteVertexArrays(1,&VertexArrayObject);
	}
	/*
	 * End of class
	 */
};

class lrGL3InstanceSet : public lrGL3InstanceSetBase, public liGL3InstanceSet
{
private:
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	std::vector<lrGLMaterialData> Materials;
	std::vector<lmMatrix4x4> ModelMatrices;
	std::vector<lmMatrix3x3> NormalMatrices;
	//std::vector<lmMatrix4x4> MvMatrices;
	std::vector<lmMatrix4x4> MvpMatrices;
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
	/*void DrawRange(liGLShader &shader,liGLPbMatShader &mat_shader,unsigned int begin,unsigned int end)
	{
		glBindVertexArray(VertexArrayObject);
		for(unsigned int i=begin;i < std::min(NumInstances(),end);i++)
		{
			ApplyMatrix(shader,i);
			ApplyMaterial(mat_shader,i);
			//
			MaterialGroup.Draw();
		}
		glBindVertexArray(0);
	}*/
	//
public:
	//
	/*
	class lrGL3InstanceSubset : public liGL3InstanceSet
	{
	private:
		lrGL3InstanceSet &InstanceSet;
		unsigned int Begin;
		unsigned int End;
		//
	public:
		//
		virtual void SetInstance(const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix,const liMaterial &material,const li3DMesh &mesh,unsigned int instance_id) override
		{
			InstanceSet.SetInstance(projection_matrix,view_matrix,material,mesh,Begin + instance_id);
		}
		//
		virtual void DrawInstances(liGLShader &shader,liGLPbMatShader &mat_shader) override
		{
			InstanceSet.DrawRange(shader,mat_shader,Begin,End);
		}
		//
		lrGL3InstanceSubset(lrGL3InstanceSet &instance_set,unsigned int begin,unsigned int end)
			:InstanceSet(instance_set),Begin(begin),End(end)
		{}
		//
		virtual ~lrGL3InstanceSubset() override
		{}
		/*
		 * End of class
		 * /
	};
	//
	lrGL3InstanceSubset GetSubset(unsigned int begin,unsigned int end)
	{
		return lrGL3InstanceSubset(*this,begin,end);
	}
	*/
	//
	virtual unsigned int NumInstances() override
	{
		return (unsigned int)ModelMatrices.size();
	}
	//
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		ModelMatrices.resize(size);
		NormalMatrices.resize(size);
		//MvMatrices.resize(size);
		MvpMatrices.resize(size);
	}
	//
	virtual void InitializeMvpMatrices(const lmMatrix4x4 &projection_matrix,const lmMatrix4x4 &view_matrix) override
	{
		for(int i=0;i < MvpMatrices.size();i++)
		{
			//MvMatrices[i] = view_matrix * ModelMatrices[i];
			//MvpMatrices[i] = projection_matrix MvMatrices[i];
			MvpMatrices[i] = projection_matrix * view_matrix * ModelMatrices[i];
		}
	}
	//
	virtual void SetInstance(const liMaterial &material,const li3DMesh &mesh,unsigned int instance_id) override
	{
		if(instance_id < NumInstances())
		{
			Materials[instance_id].SetMaterial(material);
			ModelMatrices[instance_id] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			lrUtils::GetModelMatrix(mesh,ModelMatrices[instance_id]);
			lmMatrix3x3 InvModelMatrix;
			lmInverse(ModelMatrices[instance_id].GetSubMatrix(3,3),InvModelMatrix);
			InvModelMatrix.Transpose(NormalMatrices[instance_id]);
		}
	}
	//
	//virtual
	void DrawInstances(liGLShader &shader,liGLPbMatShader &mat_shader)// override
	{
		//DrawRange(shader,mat_shader,0,NumInstances());
		glBindVertexArray(VertexArrayObject);
		for(unsigned int i=0;i < NumInstances();i++)
		{
			ApplyMatrix(shader,i);
			ApplyMaterial(mat_shader,i);
			//
			MaterialGroup.Draw();
		}
		glBindVertexArray(0);
	}
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
	lrGL3InstanceSet()
	{}
	//
	virtual ~lrGL3InstanceSet() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_MESH_INSTANCES_H
