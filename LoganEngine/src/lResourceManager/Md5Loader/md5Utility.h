#ifndef MD5_UTILITY_H
#define MD5_UTILITY_H

#include "md5Skeleton.h"

#include <istream>
#include <string>

#include "../../lMath/lMath.h"

class md5Utility
{
public:
	//
	static void LoadQuotedString(std::istream &in,std::string &str)
	{
		char c;
		in.get(c);
		while(c != '\"')
		{
			in.get(c);
		}
		in.get(c);
		while(c != '\"')
		{
			str.push_back(c);
			in.get(c);
		}
	}
	//
	static void ComputeQuaternion(float x,float y,float z,lmQuaternion &q)
	{
		q.X = x;
		q.Y = y;
		q.Z = z;
		//
		float t = 1.0 - (x*x + y*y + z*z);
		if(t < 0.0)
		{
			q.W = 0.0;
		}
		else
		{
			q.W = -sqrt(t);
		}
	}
	//
	static void Vector3D_YUp(const lmVector3D &v,lmVector3D &Target)
	{
		Target = {v[0],v[2],-v[1]};
	}
	//
	static void Quat_YUp(const lmQuaternion &p,lmQuaternion &Target)
	{
		lmQuaternion q({1.0,0.0,0.0},-PI/2.0);
		lmQuaternion qinv = q.GetInverse();
		//
		Target = q * p * qinv;
	}
	//
	static void Skeleton_YUp(const md5SkeletonExtrinsic &anim_frame,md5SkeletonExtrinsic &Target)
	{
		if(anim_frame.Size() != Target.Size())
		{
			Target.Resize(anim_frame.Size());
		}
		//
		for(unsigned int k=0;k < anim_frame.Size();k++)
		{
			//lmVector3D YUp_Pos;
			Vector3D_YUp(anim_frame[k].Position,Target[k].Position);
			//
			//lmQuaternion YUp_Orientation;
			Quat_YUp(anim_frame[k].Orientation,Target[k].Orientation);
			//
			//Target[k].Position = YUp_Pos;
			//Target[k].Orientation = YUp_Orientation;
		}
	}
	//
	/*
	 * End of class
	 */
};


#endif // MD5_UTILITY_H
