#ifndef MD5_ANIM_H
#define MD5_ANIM_H

#include "md5Skeleton.h"

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include <lCore/lMath/lMath.h>

struct md5Anim
{
	int NumFrames = 0;
	int NumJoints = 0;
	int FrameRate = 0;
	int NumAnimatedComponents = 0;
	//
	struct md5Hierarchy
	{
		std::string Name;
		int ParentIndex;
		int Flags;
		int StartIndex;
	};
	//std::map<std::string,md5Hierarchy> Hierarchy;
	std::vector<md5Hierarchy> Hierarchy;
	//
	md5SkeletonHierarchy SkeletonHierarchy;
	//
	struct md5Bounds
	{
		lmVector3D Min;
		lmVector3D Max;
	};
	std::vector<md5Bounds> Bounds;
	//
	std::vector<md5JointData> Baseframe;
	//
	std::vector<std::vector<float> > Frames;
	//
	void Print(std::ostream &out);
	//
	md5Anim(){}
	//
	~md5Anim(){}
	//
	/*
	 * End of class
	 */
};

bool md5LoadAnim(std::istream &in,md5Anim &anim);

#endif // MD5_ANIM_H
