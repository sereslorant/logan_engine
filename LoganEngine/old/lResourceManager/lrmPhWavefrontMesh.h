#ifndef LRM_PH_WAVEFRONT_MESH_H
#define LRM_PH_WAVEFRONT_MESH_H

#include <string>
#include <iostream>
#include <fstream>

#include "lrmPhMesh.h"

class lrmPhWavefrontMesh : public lrmPhMesh
{
public:

	lrmPhWavefrontMesh(const std::string &filename,bool FlipNormals)
	{
		std::fstream in;
		in.open((filename).c_str());

		if(!in.is_open())
		{
			std::cerr << "Couldn't open file " << filename << std::endl;
			Successful = false;
			return;
		}

		while(!in.eof())
		{
			std::string Tmp;
			getline(in,Tmp);

			if(Tmp[0] == 'v' && Tmp[1] == 'n')
			{
				float n1,n2,n3;
				sscanf(Tmp.c_str(),"vn %f %f %f",&n1,&n2,&n3);

				if(FlipNormals)
				{
					Normals.push_back({-n1,-n2,-n3});
				}
				else
				{
					Normals.push_back({n1,n2,n3});
				}
			}
			else if(Tmp[0] == 'v')
			{
				float v1,v2,v3;
				sscanf(Tmp.c_str(),"v %f %f %f",&v1,&v2,&v3);

				Vertices.push_back({v1,v2,v3});
			}
			else if(Tmp[0] == 'f')
			{
				unsigned int V[3];
				unsigned int Normal;
				sscanf(Tmp.c_str(),"f %d//%d %d//%d %d//%d",&V[0],&Normal,&V[1],&Normal,&V[2],&Normal);

				V[0] -= 1;V[1] -= 1;V[2] -= 1;
				Normal -= 1;

				Triangles.push_back({&V[0],Normal});
				/*
				if((*Target)->Vertices[Triangle.V1].T1 == -1)		{(*Target)->Vertices[Triangle.V1].T1 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V1].T2 == -1)	{(*Target)->Vertices[Triangle.V1].T2 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V1].T3 == -1)	{(*Target)->Vertices[Triangle.V1].T3 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V1].T4 == -1)	{(*Target)->Vertices[Triangle.V1].T4 = (*Target)->Triangles.size() -1;}

				if((*Target)->Vertices[Triangle.V2].T1 == -1)		{(*Target)->Vertices[Triangle.V2].T1 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V2].T2 == -1)	{(*Target)->Vertices[Triangle.V2].T2 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V2].T3 == -1)	{(*Target)->Vertices[Triangle.V2].T3 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V2].T4 == -1)	{(*Target)->Vertices[Triangle.V2].T4 = (*Target)->Triangles.size() -1;}

				if((*Target)->Vertices[Triangle.V3].T1 == -1)		{(*Target)->Vertices[Triangle.V3].T1 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V3].T2 == -1)	{(*Target)->Vertices[Triangle.V3].T2 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V3].T3 == -1)	{(*Target)->Vertices[Triangle.V3].T3 = (*Target)->Triangles.size() -1;}
				else if((*Target)->Vertices[Triangle.V3].T4 == -1)	{(*Target)->Vertices[Triangle.V3].T4 = (*Target)->Triangles.size() -1;}
				*/
			}
		}

		Successful = true;
		in.close();
	}

	virtual ~lrmPhWavefrontMesh() override
	{
		//Üres függvénytörzs
	}
};

#endif // LRM_PH_WAVEFRONT_MESH_H
