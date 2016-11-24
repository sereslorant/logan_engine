#ifndef LRM_MD5_LOADER
#define LRM_MD5_LOADER

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <map>

#include "../lMath/lMath.h"
#include "../lMath/lmQuaternion.h"
#include "../lMath/lmVector3D.hpp"
#include "../lMath/lmMatrix3x3.h"
#include "../lMath/lmMatrix4x4.h"

#include "lrmStaticMesh.h"
#include "lrmSkeletalMesh.h"

#include "../lInterfaces/lResourceManager/liCharAnimFactory.h"



#include "../lRenderer/lrCharacterAnim/lrSkeleton.h"
#include "../lRenderer/lrMultiMaterial.h"


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

    struct md5File
    {
        struct md5Joint
        {
            int ParentIndex;
            lmVector3D      Position;
            lmQuaternion    Orientation;
        };
        std::vector<md5Joint> Joints;
        std::map<std::string,unsigned int> JointNames;

        lrMultiMaterial *Materials;

        bool FreeMaterials;

        struct md5Mesh
        {
            std::string Material;
            lrmStaticMesh *BpMesh;

            struct md5Vertex
            {
                lmVector2D TexCoord;
                int StartWeight;
                int WeightCount;
            };
            std::vector<md5Vertex> Vertices;

            struct md5Triangle
            {
                int V1;
                int V2;
                int V3;
                lmVector3D Normal;//NE felejtsd el kiszámítani
            };
            std::vector<md5Triangle> Triangles;

            struct md5Weight
            {
                int JointIndex;
                float WeightBias;
                lmVector3D wPosition;
            };
            std::vector<md5Weight> Weights;

            md5Mesh()
            {BpMesh = nullptr;}
            ~md5Mesh()
            {delete BpMesh;}
        };
        std::vector<md5Mesh> Meshes;

		lrSkeleton		BindPoseSkeleton;
		bool			BindPoseSkeletonInitialized = false;
        lrmStaticMesh	*BindPoseMesh = nullptr;

        bool LoadMd5Mesh(const std::string &Path,const std::string &filename);
        void Print(std::ostream &out);

        md5File(bool freeMaterials):FreeMaterials(freeMaterials)
        {
            Materials = new lrMultiMaterial;
        }
        ~md5File()
        {
            if(FreeMaterials)
            {
                delete Materials;
            }

			delete BindPoseMesh;
        }
    };

    struct md5Anim
    {
        int NumFrames;
        int NumJoints;
        int FrameRate;
        int NumAnimatedComponents;

        struct md5Hierarchy
        {
            std::string Name;
            int ParentIndex;
            int Flags;
            int StartIndex;
        };
        //std::map<std::string,md5Hierarchy> Hierarchy;
        std::vector<md5Hierarchy> Hierarchy;

        struct md5Bounds
        {
            lmVector3D Min;
            lmVector3D Max;
        };
        std::vector<md5Bounds> Bounds;

        struct md5Baseframe
        {
            lmVector3D Position;
            lmQuaternion Orientation;
        };
        std::vector<md5Baseframe> Baseframe;

        std::vector<std::vector<float> > Frames;

        bool LoadMd5Anim(const std::string &Path,const std::string &filename);
        void Print(std::ostream &out);
    };

    static void ComputeQuaternion(float x,float y,float z,lmQuaternion &q);
    static void LoadQuotedString(std::istream &in,std::string &str);
    static bool Cmp(const md5File::md5Mesh::md5Weight &A,const md5File::md5Mesh::md5Weight&B);

    static void Skeleton_YUp(liSkeleton &Skeleton);

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
    void ExtractBpMesh(lrmStaticMesh **Target,const std::string &Key,unsigned int MeshIndex);

    std::string Path;
    std::map<std::string,md5File *> Models;
    std::map<std::string,md5Anim *> Anims;
    bool FreeMaterials;
    bool Y_Up;

public:
	liSkeleton &GetBindPoseSkeleton(const std::string &filename)
	{
		return Models[filename]->BindPoseSkeleton;
	}

    unsigned int GetMeshesSize(const std::string &Key);
    const std::string &GetMeshTexture(const std::string &Key,unsigned int i);

    //TMP
    void SetImgPath(const std::string &imgPath)
    {
        ImgPath = imgPath;
    }

    bool LoadAnim(const std::string &filename);
    bool LoadMesh(const std::string &filename);

    void Extract(lrmSkeletalMesh **Target,const std::string &Key,unsigned int MeshIndex);
    void ExtractAnim(liKeyframeAnimation **Target,liCharAnimFactory *char_anim_factory,const std::string &Str);
    void ExtractMaterial(lrMultiMaterial **Material,const std::string &Key);

    lrmMd5Loader(const std::string &path,bool freeMaterials = true,bool y_Up = true);
    ~lrmMd5Loader();
};

#endif
