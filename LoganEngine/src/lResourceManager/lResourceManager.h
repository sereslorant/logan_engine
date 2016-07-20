#ifndef L_RESOURCE_MANAGER_H
#define L_RESOURCE_MANAGER_H

#include "../lMath/lMath.h"

template<class lResource_T>
class liResource
{
public:
	virtual bool IsReady() = 0;
	virtual bool ErrorHappened() = 0;

	virtual lResource_T &GetResource() = 0;

	liResource(){}
	virtual ~liResource(){}
};

template<class lResource_T>
class lrmResource : public liResource<lResource_T>
{
private:
	bool Ready = false;
	bool Error = false;
	lResource_T *Resource;

public:

	virtual bool IsReady() override
	{
		return Ready;
	}

	virtual bool ErrorHappened() override
	{
		return Error;
	}

	virtual lResource_T &GetResource() override
	{
		return *Resource;
	}

	void SetReady(bool ready)
	{
		Ready = ready;
	}

	void SetError(bool error)
	{
		Error = error;
	}

	void SetResource(lResource_T *resource)
	{
		Resource = resource;
	}

	lrmResource(lResource_T *null_object)
		:Resource(null_object)
	{
		//
	}

	virtual ~lrmResource() override
	{

	}
};


#include <string>

struct lrmStaticMesh
{
    std::vector<lmVector3D> Vertices;	/**< A vertexeket tartalmazó tömb */
    std::vector<lmVector3D> Normals;	/**< A normálvektorokat tartalmazó tömb */
    //std::vector<lmVector3D> Tangents;   /**< A tangent vektorokat tartalmazó tömb */
    //std::vector<lmVector3D> Bitangents; /**< A bitangent vektorokat tartalmazó tömb */
    std::vector<lmVector2D> TexCoords;  /**< A textúra koordinátákat tartalmazó tömb */

    struct lrmMtlGroup
    {
        std::string Material;                   /**< A material kulcsa */
        std::vector<unsigned int> IndexBuffer;  /**< Az index buffer */
    };

    std::vector<lrmMtlGroup *> MtlGroups; /**< A material groupokat tartalmazó tömb. */

    lrmStaticMesh()
    {}

    ~lrmStaticMesh()
    {
    	for(lrmMtlGroup *MtlGroup : MtlGroups)
    	{
    		delete MtlGroup;
    	}
    }
};

#include "lrmWfLoader.h"

class lrmResourceManager
{
private:
	std::map<std::string,lrmStaticMesh *> StaticMeshes;

public:
	//
	lrmStaticMesh *GetStaticMesh(const std::string &mesh_name)
	{
		auto I = StaticMeshes.find(mesh_name);
		//
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *StaticMesh = new lrmStaticMesh;
			StaticMesh->Vertices.push_back(lmVector3D({0.0,0.4,-3.0}));
			StaticMesh->Vertices.push_back(lmVector3D({-0.2,-0.4,-3.0}));
			StaticMesh->Vertices.push_back(lmVector3D({0.2,-0.4,-3.0}));
			//
			lrmStaticMesh::lrmMtlGroup *MtlGroup = new lrmStaticMesh::lrmMtlGroup;
			MtlGroup->IndexBuffer.push_back(0);
			MtlGroup->IndexBuffer.push_back(1);
			MtlGroup->IndexBuffer.push_back(2);
			//
			StaticMesh->MtlGroups.push_back(MtlGroup);
			//
			StaticMeshes[mesh_name] = StaticMesh;
			//
			return StaticMesh;
		}
		else
		{
			return I->second;
		}
	}
	//
	lrmResourceManager()
		{}
	//
	virtual ~lrmResourceManager() //override
		{}
};

#endif // L_RESOURCE_MANAGER_H
