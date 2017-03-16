#ifndef LRM_MD5_LOADER
#define LRM_MD5_LOADER

#include "md5File.h"
#include "md5Anim.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <map>

#include "../../lMath/lMath.h"

#include "../lLegacyResourceManager/lrmSkeletalMesh.h"

class lrmMd5SkeletonAdapter
{
private:
	//
	struct lBoneTransform
    {
        bool MatrixReady; /**< True, ha a mátrixot már kiszámoltuk, false egyébként. */
        lmMatrix4x4 Matrix; /**< A transzformáció mátrixa */

        /** \brief Default konstruktor, ami a MatrixReady-t false-ra állítja.
         *
         */
        lBoneTransform():MatrixReady(false)
        {}
    };
	//
	md5SkeletonHierarchy &Skeleton;
	bool Recursive;
	//
	std::vector<lBoneTransform> TransformCache;
	std::vector<lBoneTransform> InvTransformCache;
	//
public:
	//
	unsigned int Size()
	{
		return Skeleton.JointMetadata.size();
	}
	//
	const lmMatrix4x4 &GetTransformation(const std::string &bone_name,md5SkeletonExtrinsic &extrinsic_state)
	{
		auto j = Skeleton.JointNames.find(bone_name);
		unsigned int Index = j->second;
		return GetTransformation(Index,extrinsic_state);
	}
	//
	const lmMatrix4x4 &GetInverseTransformation(const std::string &bone_name,md5SkeletonExtrinsic &extrinsic_state)
	{
		auto j = Skeleton.JointNames.find(bone_name);
		unsigned int Index = j->second;
		return GetInverseTransformation(Index,extrinsic_state);
	}
	//
	const lmMatrix4x4 &GetTransformation(unsigned int Index,md5SkeletonExtrinsic &extrinsic_state)
	{
		if(Index > TransformCache.size())
		{
			TransformCache.resize(Skeleton.JointMetadata.size());
		}

		if(TransformCache[Index].MatrixReady == false)
		{
			lmMatrix4x4 Transformation = lmTranslate4x4(extrinsic_state[Index].Position)*lmRotate4x4(extrinsic_state[Index].Orientation);
			//
			if(Skeleton.JointMetadata[Index].ParentIndex == -1)
			{
				TransformCache[Index].Matrix = Transformation;
			}
			else
			{
				if(Recursive)
				{
					TransformCache[Index].Matrix = GetTransformation(Skeleton.JointMetadata[Index].ParentIndex,extrinsic_state) * Transformation;
				}
				else
				{
					TransformCache[Index].Matrix = Transformation;
				}
			}
			//
			TransformCache[Index].MatrixReady = true;
		}
		//
		return TransformCache[Index].Matrix;
	}
	//
	const lmMatrix4x4 &GetInverseTransformation(unsigned int Index,md5SkeletonExtrinsic &extrinsic_state)
    {
		if(Index > InvTransformCache.size())
		{
			InvTransformCache.resize(Skeleton.JointMetadata.size());
		}

    	if(InvTransformCache[Index].MatrixReady == false)
		{
			lmMatrix4x4 Transformation = lmRotate4x4(extrinsic_state[Index].Orientation.GetInverse()) * lmTranslate4x4(-1.0*extrinsic_state[Index].Position);
			//
			if(Skeleton.JointMetadata[Index].ParentIndex == -1)
			{
				InvTransformCache[Index].Matrix = Transformation;
			}
			else
			{
				if(Recursive)
				{
					InvTransformCache[Index].Matrix =  Transformation * GetInverseTransformation(Skeleton.JointMetadata[Index].ParentIndex,extrinsic_state);
				}
				else
				{
					InvTransformCache[Index].Matrix = Transformation;
				}
			}
			//
			InvTransformCache[Index].MatrixReady = true;
		}
    	//
		return InvTransformCache[Index].Matrix;
    }
	//
	lrmMd5SkeletonAdapter(md5SkeletonHierarchy &skeleton,bool recursive,bool prealloc_transform_cache = false,bool prealloc_inv_transform_cache = false)
		:Skeleton(skeleton),Recursive(recursive)
	{
		if(prealloc_transform_cache)
		{
			TransformCache.resize(Skeleton.JointMetadata.size());
		}
		if(prealloc_inv_transform_cache)
		{
			InvTransformCache.resize(Skeleton.JointMetadata.size());
		}
	}
};

struct lrmMd5Skeleton : public lrmMd5SkeletonAdapter
{
	md5SkeletonHierarchy Skeleton;
	lrmMd5Skeleton():lrmMd5SkeletonAdapter(Skeleton,true,false,true)
	{}
};

/** \brief MD5 fájlbetöltő
 *
 * \todo Dokumentálni kéne
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrmMd5Loader
{
private:
    //TMP!!!
    static std::string ImgPath;


    static void ComputeQuaternion(float x,float y,float z,lmQuaternion &q);
    static void LoadQuotedString(std::istream &in,std::string &str);
    static bool Cmp(const md5File::md5Mesh::md5Weight &A,const md5File::md5Mesh::md5Weight&B);

    //static void Skeleton_YUp(liSkeleton &Skeleton);

    static void Vector3D_YUp(const lmVector3D &v,lmVector3D &Target);
    static void Quat_YUp(const lmQuaternion &p,lmQuaternion &Target);
	/*
    static const lmVector3D &Vector3D_YUp(const lmVector3D &v)
    {
    	return lmVector3D(v.X,v.Z,-v.Y);
	}

    static const lmQuaternion &Quat_YUp(const lmQuaternion &p)
    {
    	lmQuaternion q(lmVector3D(1.0,0.0,0.0),-PI/2.0);

		return q * p * q.GetInverse();
    }
	*/
    //void ExtractBpMesh(lrmStaticMesh **Target,const std::string &Key,unsigned int MeshIndex);

    std::string Path;
    std::map<std::string,md5File *> Models;
    std::map<std::string,md5Anim *> Anims;
    bool FreeMaterials;
    bool Y_Up;

public:
	/*liSkeleton &GetBindPoseSkeleton(const std::string &filename)
	{
		return Models[filename]->BindPoseSkeleton;
	}*/

    unsigned int GetMeshesSize(const std::string &Key);
    const std::string &GetMeshTexture(const std::string &Key,unsigned int i);

    //TMP
    void SetImgPath(const std::string &imgPath)
    {
        ImgPath = imgPath;
    }

    bool LoadAnim(const std::string &filename);
    bool LoadMesh(const std::string &filename);

    //void Extract(lrmSkeletalMesh **Target,const std::string &Key,unsigned int MeshIndex);
    //void ExtractAnim(liKeyframeAnimation **Target,liCharAnimFactory *char_anim_factory,const std::string &Str);
    //void ExtractMaterial(lrMultiMaterial **Material,const std::string &Key);

    lrmMd5Loader(const std::string &path,bool freeMaterials = true,bool y_Up = true);
    ~lrmMd5Loader();
};

void md5ExtractBindPoseMesh(md5File &file,bool y_up,unsigned int mesh_index,lrmStaticMultiMesh &bind_pose_mesh);

void md5ExtractSkeletalMesh(md5File &file,bool y_up,unsigned int mesh_index,lrmSkeletalMesh &skeletal_mesh);

void md5ExtractAnimFrame(md5Anim &anim,bool y_up,unsigned int frame_index,md5SkeletonExtrinsic &anim_frame);

#endif
