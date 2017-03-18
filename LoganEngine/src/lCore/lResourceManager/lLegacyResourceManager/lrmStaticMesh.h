#ifndef LRM_STATIC_MESH_H
#define LRM_STATIC_MESH_H

#include <lMath/lMath.h>

#include <vector>
#include <map>

#include <string>

struct lrmStaticMultiMesh
{
    std::vector<lmVector3D> Vertices;	/**< A vertexeket tartalmazó tömb */
    std::vector<lmVector3D> Normals;	/**< A normálvektorokat tartalmazó tömb */
    std::vector<lmVector3D> Tangents;   /**< A tangent vektorokat tartalmazó tömb */
    std::vector<lmVector3D> Bitangents; /**< A bitangent vektorokat tartalmazó tömb */
    std::vector<lmVector2D> TexCoords;  /**< A textúra koordinátákat tartalmazó tömb */
    //
    struct lrmMtlGroup
    {
        std::string Material;                   /**< A material kulcsa */
        std::vector<unsigned int> IndexBuffer;  /**< Az index buffer */
    };
    //
private:
    std::map<std::string,lrmMtlGroup *> MtlGroupDict;
    std::vector<lrmMtlGroup *> MtlGroups; /**< A material groupokat tartalmazó tömb. */
    //
public:
    //
    void AddMaterialGroup(lrmMtlGroup *mtl_group)
    {
    	MtlGroups.push_back(mtl_group);
    	MtlGroupDict[mtl_group->Material] = mtl_group;
    }
    //
    unsigned int GetNumMtlGroups()
    {
    	return MtlGroups.size();
    }
    //
    lrmMtlGroup &GetMaterialGroup(unsigned int id)
    {
    	return *MtlGroups[id];
    }
    //
    lrmMtlGroup &GetMaterialGroup(const std::string &material)
    {
    	return *MtlGroupDict[material];
    }
    //
    lrmStaticMultiMesh()
    {}
    //
    ~lrmStaticMultiMesh()
    {
    	for(lrmMtlGroup *MtlGroup : MtlGroups)
    	{
    		delete MtlGroup;
    	}
    }
};

struct lrmStaticMesh
{
	std::vector<lmVector3D> Vertices;	/**< A vertexeket tartalmazó tömb */
	std::vector<lmVector3D> Normals;	/**< A normálvektorokat tartalmazó tömb */
	std::vector<lmVector3D> Tangents;   /**< A tangent vektorokat tartalmazó tömb */
	std::vector<lmVector3D> Bitangents; /**< A bitangent vektorokat tartalmazó tömb */
	std::vector<lmVector2D> TexCoords;  /**< A textúra koordinátákat tartalmazó tömb */
	//
	std::vector<unsigned int> IndexBuffer;  /**< Az index buffer */
	//
	lrmStaticMesh()
	{}
	//
	~lrmStaticMesh()
	{}
	/*
	 * End of class
	 */
};

#endif // LRM_STATIC_MESH_H
