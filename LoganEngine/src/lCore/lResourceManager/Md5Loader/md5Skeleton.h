#ifndef MD5_SKELETON_H
#define MD5_SKELETON_H

#include <string>
#include <vector>
#include <map>

#include "../../lMath/lMath.h"

struct md5JointData
{
	lmVector3D Position;
	lmQuaternion Orientation;
};

/*
 * Extrinsic state
 */
struct md5SkeletonExtrinsic
{
	std::vector<md5JointData> JointData;

	void Resize(unsigned int num_joints)
	{
		JointData.resize(num_joints);
	}

	unsigned int Size() const
	{
		return JointData.size();
	}

	const md5JointData &operator[](unsigned int i) const
	{
		return JointData[i];
	}

	md5JointData &operator[](unsigned int i)
	{
		return JointData[i];
	}
};

/*
 * Intrinsic state
 */
struct md5SkeletonHierarchy
{
	struct md5JointMetadata
	{
		int ParentIndex;
		unsigned int Index;
		//lmVector3D      Position;
		//lmQuaternion    Orientation;
	};
	std::map<std::string,unsigned int> JointNames;
	std::vector<md5JointMetadata> JointMetadata;
	//
};

#endif // MD5_SKELETON_H
