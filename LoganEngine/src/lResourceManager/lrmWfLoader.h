#ifndef LRM_WF_LOADER_H
#define LRM_WF_LOADER_H

/*
 * Matekkönyvtár include
 */
#include "../lMath/lMath.h"

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

#include <istream>
#include <cstdio>

/** \class lrmWfLoader
 * \brief Egy Wavefront .obj fájlt importáló osztály.
 *
 * \todo Dokumentálni kéne + tisztességesen meg kéne csinálni + sscanf_s használata
 *
 * \author Seres Lóránt
 *
 */
class lrmWfLoader
{
private:
	lrmWfObj &Obj;
	std::istream &In;
	bool Error = false;
	//
public:
	//
	bool GetError(){return Error;}
	//
	lrmWfLoader(lrmWfObj &obj,std::istream &in,bool smooth_normals)
		:Obj(obj),In(in)
	{
		unsigned int CurrentMtlGroup = -1;
		while(!In.eof())
		{
			std::string Tmp;
			std::getline(In,Tmp);
			//
			if(Tmp[0] == 'm' && Tmp[1] == 't' && Tmp[2] == 'l' && Tmp[3] == 'l' && Tmp[4] == 'i' && Tmp[5] == 'b')
			{
				char MtlFile[50];
				std::sscanf(Tmp.c_str(),"mtllib %s",MtlFile);
				//
				//std::cout << "mtllib " << MtlFile << std::endl;
				/*
				MaterialFileName = MtlFile;
				MultiMaterial = new lrMultiMaterial;
				//
				LoadWavefrontMtl(materialPath + MaterialFileName);
				*/
			}
			else if(Tmp[0] == 'v' && Tmp[1] == 't')
			{
				float u,v;
				std::sscanf(Tmp.c_str(),"vt %f %f",&u,&v);
				//Obj.TexCoords.push_back({u,v});
				Obj.TexCoords.push_back(lmVector2D({u,v})); //A fogyatékos Eclipse és a false pozitívjai. -.-
			}
			else if(Tmp[0] == 'v' && Tmp[1] == 'n')
			{
				float n1,n2,n3;
				sscanf(Tmp.c_str(),"vn %f %f %f",&n1,&n2,&n3);
				//Obj.Normals.push_back({n1,n2,n3});
				Obj.Normals.push_back(lmVector3D({n1,n2,n3})); //A fogyatékos Eclipse és a false pozitívjai. -.-
			}
			else if(Tmp[0] == 'v')
			{
				float v1,v2,v3;
				sscanf(Tmp.c_str(),"v %f %f %f",&v1,&v2,&v3);
				//Obj.Vertices.push_back({v1,v2,v3});
				Obj.Vertices.push_back(lmVector3D({v1,v2,v3})); //A fogyatékos Eclipse és a false pozitívjai. -.-
			}
			else if(Tmp[0] == 'u' && Tmp[1] == 's' && Tmp[2] == 'e' && Tmp[3] == 'm' && Tmp[4] == 't' && Tmp[5] == 'l')
			{
				char TmpArray[50];
				sscanf(Tmp.c_str(),"usemtl %s",TmpArray);
				//
				Obj.MatGroups.push_back(new lrmWfObj::lrmWfMatGroup);
				CurrentMtlGroup++;
				lrmWfObj::lrmWfMatGroup *Current = Obj.MatGroups[CurrentMtlGroup];
				Current->Material = TmpArray;
			}
			else if(Tmp[0] == 'f')
			{
				lrmWfObj::lrmWfTriangle Triangle;
				int Normal;
				sscanf(Tmp.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&Triangle.V1,&Triangle.Tx1,&Normal,&Triangle.V2,&Triangle.Tx2,&Normal,&Triangle.V3,&Triangle.Tx3,&Normal);
				//
				Triangle.V1 -= 1;Triangle.V2 -= 1;Triangle.V3 -= 1;
				Triangle.Tx1 -= 1;Triangle.Tx2 -= 1;Triangle.Tx3 -= 1;
				Triangle.Normal = Normal-1;
				//
				if(smooth_normals)
				{
					if(Obj.SmoothNormals.size() != Obj.Vertices.size())
						{Obj.SmoothNormals.resize(Obj.Vertices.size());}
					//
					Obj.SmoothNormals[Triangle.V1] += Obj.Normals[Triangle.Normal];
					Obj.SmoothNormals[Triangle.V2] += Obj.Normals[Triangle.Normal];
					Obj.SmoothNormals[Triangle.V3] += Obj.Normals[Triangle.Normal];
				}
				//
				lrmWfObj::lrmWfMatGroup *Current = Obj.MatGroups[CurrentMtlGroup];
				if(Current != nullptr)
				{
					Current->Triangles.push_back(Triangle);
				}
			}
		}
		//
		if(smooth_normals)
		{
			for(lmVector3D &Normal : Obj.SmoothNormals)
			{
				Normal.Normalize();
			}
		}
	}
	//
	~lrmWfLoader()
	{}
};


#endif // LRM_WF_LOADER_H
