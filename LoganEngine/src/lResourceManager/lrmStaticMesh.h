#ifndef LRM_STATIC_MESH_H
#define LRM_STATIC_MESH_H

#include "../lMath/lMath.h"

#include <vector>
#include <string>

struct lrmStaticMesh
{
    std::vector<lmVector3D> Vertices;	/**< A vertexeket tartalmazó tömb */
    std::vector<lmVector3D> Normals;	/**< A normálvektorokat tartalmazó tömb */
    std::vector<lmVector3D> Tangents;   /**< A tangent vektorokat tartalmazó tömb */
    std::vector<lmVector3D> Bitangents; /**< A bitangent vektorokat tartalmazó tömb */
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

#endif // LRM_STATIC_MESH_H
