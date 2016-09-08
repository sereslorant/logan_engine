#ifndef LRM_WF_OBJ_H
#define LRM_WF_OBJ_H

/*
 * Matekkönyvtár include
 */
#include "../../lMath/lMath.h"

/*
 * STL include-ok
 */
#include <string>
#include <vector>

class lrmWfObj
{
public:
	struct lrmWfTriangle
	{
		int V1,V2,V3;
		int Tx1,Tx2,Tx3;
		int Normal;
	};
	//
	struct lrmWfMatGroup
	{
		std::string Material;
		std::vector<lrmWfTriangle> Triangles;
	};
	//
	std::vector<lrmWfMatGroup *> MatGroups;
	//
	std::vector<lmVector3D> Vertices;
	std::vector<lmVector2D> TexCoords;
	std::vector<lmVector3D> Normals;
	//
	std::vector<lmVector3D> SmoothNormals;
	//
	lrmWfObj()
	{
		//Üres
	}
	//
	~lrmWfObj()
	{
		for(auto i = MatGroups.begin();i != MatGroups.end();i++)
		{
			delete *i;
		}
	}
};

#endif // LRM_WF_OBJ_H
