#ifndef L_RESOURCE_MANAGER_H
#define L_RESOURCE_MANAGER_H

#include "liResourceManager.h"

template<class lResource_T>
class lrmResource : public liResource<lResource_T>
{
private:
	bool Ready = false;
	bool Error = false;
	lResource_T *Resource;
	//
public:
	//
	virtual bool IsReady() override
	{
		return Ready;
	}
	//
	virtual bool ErrorHappened() override
	{
		return Error;
	}
	//
	virtual lResource_T &GetResource() override
	{
		return *Resource;
	}
	//
	void SetReady(bool ready)
	{
		Ready = ready;
	}
	//
	void SetError(bool error)
	{
		Error = error;
	}
	//
	void SetResource(lResource_T *resource)
	{
		Resource = resource;
	}
	//
	lrmResource(lResource_T *null_object)
		:Resource(null_object)
	{
		//
	}
	//
	virtual ~lrmResource() override
	{
		//
	}
	/*
	 * End of class
	 */
};

#include "ObjLoader/lrmWfLoader.h"

#include <string>
#include <map>

class lrmResourceManager : public liResourceManager
{
private:
	std::map<std::string,lrmStaticMesh *> StaticMeshes;

public:
	//
	static void GenerateCube(lrmStaticMesh &static_mesh,bool inside_out)
	{
		constexpr unsigned int NUM_VERTICES = 16;
		//
		static_mesh.Vertices.resize(NUM_VERTICES);
		//Front
		static_mesh.Vertices[0] = { 0.5f, 0.5f, 0.5f};
		static_mesh.Vertices[1] = {-0.5f, 0.5f, 0.5f};
		static_mesh.Vertices[2] = {-0.5f,-0.5f, 0.5f};
		static_mesh.Vertices[3] = { 0.5f,-0.5f, 0.5f};
		//Back
		static_mesh.Vertices[4] = { 0.5f, 0.5f,-0.5f};
		static_mesh.Vertices[5] = {-0.5f, 0.5f,-0.5f};
		static_mesh.Vertices[6] = {-0.5f,-0.5f,-0.5f};
		static_mesh.Vertices[7] = { 0.5f,-0.5f,-0.5f};
		//Left
		static_mesh.Vertices[8] = { 0.5f,-0.5f, 0.5f};
		static_mesh.Vertices[9] = { 0.5f, 0.5f, 0.5f};
		static_mesh.Vertices[10] = { 0.5f, 0.5f,-0.5f};
		static_mesh.Vertices[11] = { 0.5f,-0.5f,-0.5f};
		//Right
		static_mesh.Vertices[12] = {-0.5f, 0.5f, 0.5f};
		static_mesh.Vertices[13] = {-0.5f, 0.5f,-0.5f};
		static_mesh.Vertices[14] = {-0.5f,-0.5f,-0.5f};
		static_mesh.Vertices[15] = {-0.5f,-0.5f, 0.5f};
		//
		static_mesh.Normals.resize(NUM_VERTICES);
		for(unsigned int i=0;i < NUM_VERTICES;i++)
		{
			static_mesh.Normals[i] = static_mesh.Vertices[i];
			static_mesh.Normals[i].Normalize();
			if(inside_out)
			{
				static_mesh.Normals[i] *= -1.0;
			}
		}
		static_mesh.Tangents.resize(NUM_VERTICES);
		static_mesh.Bitangents.resize(NUM_VERTICES);
		//
		static_mesh.TexCoords.resize(NUM_VERTICES);
		//Front
		static_mesh.TexCoords[0] = {1.0f,1.0f};
		static_mesh.TexCoords[1] = {0.0f,1.0f};
		static_mesh.TexCoords[2] = {0.0f,0.0f};
		static_mesh.TexCoords[3] = {1.0f,0.0f};
		//Back
		static_mesh.TexCoords[4] = {1.0f,0.0f};
		static_mesh.TexCoords[5] = {0.0f,0.0f};
		static_mesh.TexCoords[6] = {0.0f,1.0f};
		static_mesh.TexCoords[7] = {1.0f,1.0f};
		//Left
		static_mesh.TexCoords[8] = {1.0f,0.0f};
		static_mesh.TexCoords[9] = {0.0f,0.0f};
		static_mesh.TexCoords[10] = {0.0f,1.0f};
		static_mesh.TexCoords[11] = {1.0f,1.0f};
		//Right
		static_mesh.TexCoords[12] = {1.0f,0.0f};
		static_mesh.TexCoords[13] = {0.0f,0.0f};
		static_mesh.TexCoords[14] = {0.0f,1.0f};
		static_mesh.TexCoords[15] = {1.0f,1.0f};
		//
		lrmStaticMesh::lrmMtlGroup *MtlGroup = new lrmStaticMesh::lrmMtlGroup;
		static_mesh.AddMaterialGroup(MtlGroup);
		//
		constexpr unsigned int NUM_TRIANGLES = 12;
		constexpr unsigned int INDEX_BUFFER_SIZE = NUM_TRIANGLES * 3;
		unsigned int IndexBuffer[INDEX_BUFFER_SIZE] = {	2,0,1,2,3,0,		//Front;
														7,5,4,7,6,5,		//Back
														8,10,9,8,11,10,		//Left
														12,13,14,12,14,15,	//Right
														1,4,5,1,0,4,		//Top
														2,6,7,2,7,3};		//Bottom
		//
		MtlGroup->Material = "Default";
		MtlGroup->IndexBuffer.resize(INDEX_BUFFER_SIZE);
		unsigned int NextIndex = 0;
		for(unsigned int i=0;i < NUM_TRIANGLES;i++)
		{
			if(inside_out)
			{
				MtlGroup->IndexBuffer[NextIndex] = IndexBuffer[NextIndex];
				NextIndex++;
				MtlGroup->IndexBuffer[NextIndex] = IndexBuffer[NextIndex + 1];
				NextIndex++;
				MtlGroup->IndexBuffer[NextIndex] = IndexBuffer[NextIndex - 1];
				NextIndex++;
			}
			else
			{
				for(int j=0;j < 3;j++)
				{
					MtlGroup->IndexBuffer[NextIndex] = IndexBuffer[NextIndex];
					NextIndex++;
				}
			}
		}
	}
	//
	static void GenerateSphere(lrmStaticMesh &static_mesh,unsigned int resolution_xz,unsigned int resolution_xy)
	{
		unsigned int NumVertices = resolution_xz*resolution_xy + 2;
		static_mesh.Vertices.resize(NumVertices);
		static_mesh.Normals.resize(NumVertices);
		static_mesh.Tangents.resize(NumVertices);
		static_mesh.Bitangents.resize(NumVertices);
		static_mesh.TexCoords.resize(NumVertices);
		//
		unsigned int TopIndex = resolution_xz*resolution_xy + 1;
		unsigned int BottomIndex = resolution_xz*resolution_xy;
		//
		static_mesh.Vertices[TopIndex]		= lmVector3D({0.0f, 1.0f,0.0f});
		static_mesh.Normals[TopIndex]		= lmVector3D({0.0f, 1.0f,0.0f});
		static_mesh.TexCoords[TopIndex]		= lmVector2D({0.5f,0.0f});
		//
		static_mesh.Vertices[BottomIndex]	= lmVector3D({0.0f,-1.0f,0.0f});
		static_mesh.Normals[BottomIndex]	= lmVector3D({0.0f,-1.0f,0.0f});
		static_mesh.TexCoords[BottomIndex]	= lmVector2D({0.5f,1.0f});
		//
		float dXYAngle = PI/(resolution_xy + 1);
		float dXZAngle = 2.0*PI/resolution_xz;

		for(unsigned int xy=0;xy < resolution_xy;xy++)
		{
			float XYAngle = (xy+1)*dXYAngle;
			for(unsigned int j=0;j < resolution_xz;j++)
			{
				unsigned int Id = xy*resolution_xz + j;
				float XZAngle = j*dXZAngle;
				//
				float X = std::sin(XYAngle)*std::cos(XZAngle);
				float Y = std::cos(XYAngle);
				float Z = std::sin(XYAngle)*std::sin(XZAngle);
				//
				static_mesh.Vertices[Id] = lmVector3D({X,Y,Z});
				static_mesh.Normals[Id] = lmVector3D({X,Y,Z});
				//
				float TexCoordX = XZAngle/(2.0*PI);
				float TexCoordY = XYAngle/PI;
				//
				static_mesh.TexCoords[Id] = lmVector2D({TexCoordX,TexCoordY});
			}
		}
		//
		lrmStaticMesh::lrmMtlGroup *MtlGroup = new lrmStaticMesh::lrmMtlGroup;
		static_mesh.AddMaterialGroup(MtlGroup);
		//
		unsigned int NumTrianglesInFan = resolution_xz - 1;
		unsigned int NumTrianglesInMid = (resolution_xy) * (resolution_xz) * 2;
		unsigned int IndexBufferSize = (2*NumTrianglesInFan*3)  + (NumTrianglesInMid * 3);
		//
		MtlGroup->Material = "Default";
		MtlGroup->IndexBuffer.resize(IndexBufferSize);
		//
		unsigned int NextTriangle = 0;
		for(unsigned int x1=0;x1 < resolution_xz;x1++)
		{
			unsigned int x2 = (x1 + 1) % resolution_xz;
			MtlGroup->IndexBuffer[NextTriangle] = TopIndex;	NextTriangle++;
			MtlGroup->IndexBuffer[NextTriangle] = x2;		NextTriangle++;
			MtlGroup->IndexBuffer[NextTriangle] = x1;		NextTriangle++;
			//
			unsigned int x3 = (resolution_xy - 1)*resolution_xz + x1;
			unsigned int x4 = (resolution_xy - 1)*resolution_xz + x2;
			MtlGroup->IndexBuffer[NextTriangle] = BottomIndex;	NextTriangle++;
			MtlGroup->IndexBuffer[NextTriangle] = x3;			NextTriangle++;
			MtlGroup->IndexBuffer[NextTriangle] = x4;			NextTriangle++;
		}
		//
		for(unsigned int xy1=0;xy1 < (resolution_xy - 1);xy1++)
		{
			for(unsigned int xz1=0;xz1 < resolution_xz;xz1++)
			{
				unsigned int xy2 = xy1+1;
				unsigned int xz2 = (xz1+1) % resolution_xz;
				//
				MtlGroup->IndexBuffer[NextTriangle] = xy1*resolution_xz + xz1;
				NextTriangle++;
				MtlGroup->IndexBuffer[NextTriangle] = xy1*resolution_xz + xz2;
				NextTriangle++;
				MtlGroup->IndexBuffer[NextTriangle] = xy2*resolution_xz + xz1;
				NextTriangle++;

				MtlGroup->IndexBuffer[NextTriangle] = xy1*resolution_xz + xz2;
				NextTriangle++;
				MtlGroup->IndexBuffer[NextTriangle] = xy2*resolution_xz + xz2;
				NextTriangle++;
				MtlGroup->IndexBuffer[NextTriangle] = xy2*resolution_xz + xz1;
				NextTriangle++;
			}
		}
	}
	//
	virtual lrmStaticMesh *GetStaticMesh(const std::string &resource_identifier) override
	{
		auto I = StaticMeshes.find(resource_identifier);
		//
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *StaticMesh = new lrmStaticMesh;
			GenerateSphere(*StaticMesh,8,16);
			/*
			StaticMesh->Vertices.push_back(lmVector3D({0.0f,0.4f,-3.0f}));
			StaticMesh->Vertices.push_back(lmVector3D({-0.2f,-0.4f,-3.0f}));
			StaticMesh->Vertices.push_back(lmVector3D({0.2f,-0.4f,-3.0f}));
			//
			lrmStaticMesh::lrmMtlGroup *MtlGroup = new lrmStaticMesh::lrmMtlGroup;
			MtlGroup->IndexBuffer.push_back(0);
			MtlGroup->IndexBuffer.push_back(1);
			MtlGroup->IndexBuffer.push_back(2);
			//
			StaticMesh->MtlGroups.push_back(MtlGroup);
			*/
			//
			StaticMeshes[resource_identifier] = StaticMesh;
			//
			return StaticMesh;
		}
		else
		{
			return I->second;
		}
	}
	//
	virtual liBitmap2D *GetBitmap(const std::string &resource_identifier) override
	{
		return nullptr;
	}
	//
	virtual liCubemap *GetCubemap(const std::string &resource_identifier) override
	{
		return nullptr;
	}
	//
	lrmResourceManager()
		{}
	//
	virtual ~lrmResourceManager() override
	{
		for(auto I : StaticMeshes)
		{
			delete I.second;
		}
	}
};

#endif // L_RESOURCE_MANAGER_H
