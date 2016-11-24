#ifndef LRM_PH_HEIGHTMAP_MESH_H
#define LRM_PH_HEIGHTMAP_MESH_H

#include <string>
#include <iostream>

#include "lrmPhMesh.h"

#include "BMP.h"
#include "lrmHeightmapLoader.h"

class lrmPhHeightmapMesh : public lrmPhMesh
{
public:

	lrmPhHeightmapMesh(const std::string &filename,float TileX = 4.0,float TileY = 4.0,float TileZ = 4.0)
	{
		BMP *Bmp = new BMP(filename);

    	if(Bmp->GetError() != BMP::ERR_OK)
        {
            std::cerr << "Couldn't open file " << filename << std::endl;
            delete Bmp;
            return;
        }

		LoadHeightmapVertices(Bmp,this->Vertices);

		for(lmVector3D &Vertex : this->Vertices)
		{
			Vertex[0] *= TileX;
			Vertex[1] *= TileY;
			Vertex[2] *= TileZ;
		}

		for(unsigned int i=0;i < Bmp->GetHeight()-1;i++)
		{
			for(unsigned int j=0;j < Bmp->GetWidth()-1;j++)
			{
				const lmVector3D &V1 = Vertices[i	  * Bmp->GetWidth() + j];
				const lmVector3D &V2 = Vertices[i	  * Bmp->GetWidth() + j+1];
				const lmVector3D &V3 = Vertices[(i+1) * Bmp->GetWidth() + j];
				const lmVector3D &V4 = Vertices[(i+1) * Bmp->GetWidth() + j+1];

				lmVector3D Normal1 = lmCross(V2-V1,V4-V1);
				Normal1.Normalize();

				lmVector3D Normal2 = lmCross(V4-V1,V3-V1);
				Normal2.Normalize();

				//LoadHeightmapTriangles_PH(Bmp,i,j,Normal1,Normal2,(*Target)->Normals,(*Target)->Triangles);
				Normals.push_back(Normal1);
				lrmPhTriangle T1;

				T1.Normal = Normals.size()-1;
				T1.V[0] =  i	* Bmp->GetWidth() + j;
				T1.V[1] =  i	* Bmp->GetWidth() + j+1;
				T1.V[2] = (i+1)	* Bmp->GetWidth() + j+1;

				Triangles.push_back(T1);

				Normals.push_back(Normal2);
				lrmPhTriangle T2;

				T2.Normal = Normals.size()-1;
				T2.V[0] =  i	* Bmp->GetWidth() + j;
				T2.V[1] = (i+1)	* Bmp->GetWidth() + j;
				T2.V[2] = (i+1)	* Bmp->GetWidth() + j+1;

				Triangles.push_back(T2);
			}
		}

		Successful = true;
        delete Bmp;
	}

	virtual ~lrmPhHeightmapMesh() override
	{
		//Üres függvénytörzs
	}
};

#endif // LRM_PH_HEIGHTMAP_MESH_H
