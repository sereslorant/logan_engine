#ifndef LR_GL3_INSTANCE_CACHER_H
#define LR_GL3_INSTANCE_CACHER_H

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGL3InstanceSet.h>

class lrGL3InstanceCacher
{
private:
	const lmMatrix4x4 *ProjectionMatrix = nullptr;
	const lmMatrix4x4 *ViewMatrix = nullptr;
	
	lrGL3StaticInstanceSet *InstanceSet= nullptr;
	unsigned int Next;
	
public:
	
	bool Initialized()
	{
		return InstanceSet != nullptr;
	}
	
	bool Finished()
	{
		if(InstanceSet != nullptr)
		{
			return Next == InstanceSet->NumInstances();
		}
		
		return false;
	}
	
	void Initialize(const lmMatrix4x4 *projection_matrix,const lmMatrix4x4 *view_matrix,lrGL3StaticInstanceSet *instance_set)
	{
		ProjectionMatrix = projection_matrix;
		ViewMatrix = view_matrix;
		
		InstanceSet = instance_set;
		Next = 0;
	}
	
	void SetNextInstance(const liMaterial &material,const li3DMesh &mesh)
	{
		if(Initialized())
		{
			lmMatrix4x4 ModelMatrix(lmMatrix4x4::IDENTITY);
			lrUtils::GetModelMatrix(mesh,ModelMatrix);
			
			InstanceSet->SetInstance({material},ModelMatrix,Next);
			Next++;
			
			if(Finished())
			{
				if(ProjectionMatrix != nullptr && ViewMatrix != nullptr)
				{
					InstanceSet->InitializeMvpMatrices(*ProjectionMatrix,*ViewMatrix);
				}
			}
		}
	}
	
	lrGL3InstanceCacher()
	{}
	
	~lrGL3InstanceCacher()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL3_INSTANCE_CACHER_H
