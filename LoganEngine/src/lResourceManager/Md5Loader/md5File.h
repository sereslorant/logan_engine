#ifndef MD5_FILE_H
#define MD5_FILE_H

#include "md5Skeleton.h"

#include <istream>
#include <ostream>
#include <string>
#include <vector>
#include <map>

#include "../../lMath/lMath.h"

struct md5File
{
	md5SkeletonHierarchy BindPoseSkeletonMetadata;
	md5SkeletonExtrinsic BindPoseSkeleton;
	//
	struct md5Mesh
	{
		std::string Material;
		//
		struct md5Vertex
		{
			lmVector2D TexCoord;
			int StartWeight;
			int WeightCount;
		};
		std::vector<md5Vertex> Vertices;
		//
		struct md5Triangle
		{
			int V1;
			int V2;
			int V3;
			lmVector3D Normal;//NE felejtsd el kiszámítani
		};
		std::vector<md5Triangle> Triangles;
		//
		struct md5Weight
		{
			int JointIndex;
			float WeightBias;
			lmVector3D wPosition;
		};
		std::vector<md5Weight> Weights;
		//
		md5Mesh()
		{
			//BpMesh = nullptr;
		}
		//
		~md5Mesh()
		{
			//delete BpMesh;
		}
	};
	std::vector<md5Mesh> Meshes;
	//
	void Print(std::ostream &out);
	//
	md5File(){}
	//
	~md5File(){}
	//
	/*
	 * End of class
	 */
};

bool md5LoadFile(std::istream &in,md5File &file);

#endif // MD5_FILE_H
