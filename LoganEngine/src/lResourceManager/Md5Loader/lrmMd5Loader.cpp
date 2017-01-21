
#include "lrmMd5Loader.h"
#include "md5Utility.h"

using namespace std;

std::string lrmMd5Loader::ImgPath;
/*
void lrmMd5Loader::ComputeQuaternion(float x,float y,float z,lmQuaternion &q)
{
    q.X = x;
    q.Y = y;
    q.Z = z;

    float t = 1.0 - (x*x + y*y + z*z);
    if(t < 0.0)
    {
        q.W = 0.0;
    }
    else
    {
        q.W = -sqrt(t);
    }
}*/
/*
void lrmMd5Loader::LoadQuotedString(istream &in,string &str)
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
}*/
/*
//EZT A FÜGGVÉNYT MÉG TESZTELNI KELL!!!
bool lrmMd5Loader::Cmp(const md5File::md5Mesh::md5Weight &A,const md5File::md5Mesh::md5Weight&B)
{
    if(A.WeightBias > B.WeightBias)
    {
        return true;
    }
    return false;
}

void lrmMd5Loader::Skeleton_YUp(liSkeleton &Skeleton)
{
    for(unsigned int i=0;i < Skeleton.Size();i++)
    {
    	lmVector3D YUp_Pos;
    	Vector3D_YUp(Skeleton[i]->GetPosition(),YUp_Pos);

    	lmQuaternion YUp_Orientation;
    	Quat_YUp(Skeleton[i]->GetOrientation(),YUp_Orientation);

        //Skeleton[i]->SetPosition(Vector3D_YUp(Skeleton[i]->GetPosition()));
        //Skeleton[i]->SetOrientation(Quat_YUp(Skeleton[i]->GetOrientation()));
        Skeleton[i]->SetPosition(YUp_Pos);
        Skeleton[i]->SetOrientation(YUp_Orientation);
    }
}
*/
void lrmMd5Loader::Vector3D_YUp(const lmVector3D &v,lmVector3D &Target)
{
    //Target[0] =  v[0];
    //Target[1] =  v[2];
    //Target[2] = -v[1];

    Target = {v[0],v[2],-v[1]};
}

void lrmMd5Loader::Quat_YUp(const lmQuaternion &p,lmQuaternion &Target)
{
    lmQuaternion q({1.0f,0.0f,0.0f},-PI/2.0f);
    lmQuaternion qinv = q.GetInverse();

    Target = q * p * qinv;
}

unsigned int lrmMd5Loader::GetMeshesSize(const string &Key)
{
    return Models[Key]->Meshes.size();
}
const string &lrmMd5Loader::GetMeshTexture(const string &Key,unsigned int i)
{
    return Models[Key]->Meshes[i].Material;
}

//EZT A FÜGGVÉNYT ÁT KELL ÍRNI
bool lrmMd5Loader::LoadAnim(const string &filename)
{
	std::ifstream fin;
    fin.open((Path + filename).c_str());
    //
    if(!fin.is_open())
    {
    	cerr << "Failed to load Md5 anim: " << Path + filename;
        return false;
    }
    //
    md5Anim *Anim = new md5Anim;
    //
    if(md5LoadAnim(fin,*Anim))
    {
        auto I = Anims.find(filename);

        if(I  == Anims.end())
        {
            Anims[filename] = Anim;

            //Anim->Print(cout);
            return true;
        }
        else
        {
            delete Anim;
        }
    }
    else
    {
        cerr << "Epic fail!\n";
    }
    return false;
}
//EZT IS
bool lrmMd5Loader::LoadMesh(const string &filename)
{
	std::ifstream fin;
    fin.open((Path + filename).c_str());
    //
    if(!fin.is_open())
    {
    	cerr << "Failed to load Md5 mesh: " << Path + filename;
        return false;
    }
    //
    md5File *File = new md5File;
    //
    if(!md5LoadFile(fin,*File))
    {
        cerr << "Failed to load Md5 mesh: " << Path + filename;
        delete File;
        return false;
    }
    else
    {
        Models[filename] = File;
        for(unsigned int i=0;i < File->Meshes.size();i++)
        {
            //ExtractBpMesh(&File->Meshes[i].BpMesh,filename,i);
        }

        //File->Print(cout);
        return true;
    }
}
/*
void lrmMd5Loader::ExtractBpMesh(lrmStaticMesh **Target,const string &Key,unsigned int MeshIndex)
{
    *Target = new lrmStaticMesh;

    (*Target)->Vertices.resize(Models[Key]->Meshes[MeshIndex].Vertices.size());
    (*Target)->TexCoords.resize(Models[Key]->Meshes[MeshIndex].Vertices.size());

    if(!(Models[Key]->BindPoseSkeletonInitialized))
	{
		//std::vector<lrSkeleton::lrBone> Bones(Models[Key]->Joints.size());
		Models[Key]->BindPoseSkeleton.Resize(Models[Key]->Joints.size());
		Models[Key]->BindPoseSkeleton.SetBoneNames(Models[Key]->JointNames);

		for(int i=0;i < Models[Key]->BindPoseSkeleton.Size();i++)
		{
			(Models[Key]->BindPoseSkeleton)[i]->SetPosition(Models[Key]->Joints[i].Position);
			(Models[Key]->BindPoseSkeleton)[i]->SetOrientation(Models[Key]->Joints[i].Orientation);
			(Models[Key]->BindPoseSkeleton)[i]->SetParentIndex(-1);
		}

		//Models[Key]->BindPoseSkeleton = new lrSkeleton(Bones,Models[Key]->JointNames);

		if(Y_Up)
		{
			Skeleton_YUp(Models[Key]->BindPoseSkeleton);
		}

		Models[Key]->BindPoseSkeletonInitialized = true;
	}

    for(unsigned int i = 0; i < Models[Key]->Meshes[MeshIndex].Vertices.size();i++)
    {
        lmVector3D Vertex;
        for(int j=0;j < Models[Key]->Meshes[MeshIndex].Vertices[i].WeightCount;j++)
        {
            md5File::md5Mesh::md5Weight &Weight = Models[Key]->Meshes[MeshIndex].Weights[Models[Key]->Meshes[MeshIndex].Vertices[i].StartWeight + j];
            md5File::md5Joint  &Joint = Models[Key]->Joints[Weight.JointIndex];

            lmVector3D      wPos;
            //lmVector3D      JointPos;
            //lmQuaternion    JointOrientation;

            if(Y_Up)
            {
                Vector3D_YUp(Weight.wPosition,wPos);
                //Vector3D_YUp(Joint.Position,JointPos);
                //Quat_YUp(Joint.Orientation,JointOrientation);
            }
            else
            {
                wPos = Weight.wPosition;
                //JointPos = Joint.Position;
                //JointOrientation = Joint.Orientation;
            }

            lmMatrix4x4 BoneTransform = Models[Key]->BindPoseSkeleton.GetTransformation(Weight.JointIndex);

            //lmVector4D Vertex4D = (lmTranslate4x4(JointPos) * lmRotate4x4(JointOrientation) * lmVector4D(wPos.X,wPos.Y,wPos.Z,1.0))*Weight.WeightBias;
            lmVector4D Vertex4D = (BoneTransform * lmVector4D({wPos[0],wPos[1],wPos[2],1.0}))*Weight.WeightBias;

            Vertex[0] += Vertex4D[0];
            Vertex[1] += Vertex4D[1];
            Vertex[2] += Vertex4D[2];
        }

        (*Target)->Vertices[i] = Vertex;
        (*Target)->TexCoords[i] = Models[Key]->Meshes[MeshIndex].Vertices[i].TexCoord;
    }

    (*Target)->Normals.resize(Models[Key]->Meshes[MeshIndex].Vertices.size());
    (*Target)->Tangents.resize(Models[Key]->Meshes[MeshIndex].Vertices.size());
    (*Target)->Bitangents.resize(Models[Key]->Meshes[MeshIndex].Vertices.size());

    lrmStaticMesh::lrmMtlGroup *newMtlGroup = new lrmStaticMesh::lrmMtlGroup;
    (*Target)->MatGroups.push_back(newMtlGroup);
    newMtlGroup->IndexBuffer.resize(Models[Key]->Meshes[MeshIndex].Triangles.size()*3);

    unsigned int k = 0;
    for(auto i=Models[Key]->Meshes[MeshIndex].Triangles.begin();i != Models[Key]->Meshes[MeshIndex].Triangles.end();i++)
    {
        lmVector3D VecV1V3 = (*Target)->Vertices[i->V3] - (*Target)->Vertices[i->V1];
        lmVector3D VecV1V2 = (*Target)->Vertices[i->V2] - (*Target)->Vertices[i->V1];

        i->Normal += lmCross(VecV1V3,VecV1V2);
        i->Normal.Normalize();

        /*
        lmVector3D v1 = (*Target)->Vertices[i->V1];
        lmVector3D v2 = (*Target)->Vertices[i->V2];
        lmVector3D v3 = (*Target)->Vertices[i->V3];

        i->Normal.X = (v1.Y - v2.Y) * (v1.Z + v2.Z) + (v2.Y - v3.Y) * (v2.Z + v3.Z) + (v3.Y - v1.Y) * (v3.Z + v1.Z);
        i->Normal.Y = (v1.Z - v2.Z) * (v1.X + v2.X) + (v2.Z - v3.Z) * (v2.X + v3.X) + (v3.Z - v1.Z) * (v3.X + v1.X);
        i->Normal.Z = (v1.X - v2.X) * (v1.Y + v2.Y) + (v2.X - v3.X) * (v2.Y + v3.Y) + (v3.X - v1.X) * (v3.Y + v1.Y);

        i->Normal.Normalize();
        * /

        (*Target)->Normals[i->V1] += i->Normal;
        (*Target)->Normals[i->V2] += i->Normal;
        (*Target)->Normals[i->V3] += i->Normal;

        newMtlGroup->IndexBuffer[k] = i->V1;
        k++;
        newMtlGroup->IndexBuffer[k] = i->V2;
        k++;
        newMtlGroup->IndexBuffer[k] = i->V3;
        k++;
    }

    for(unsigned int i=0;i < (*Target)->Normals.size();i++)
    {
        (*Target)->Normals[i].Normalize();

        lmVector3D Tangent = lmCross(lmCross((*Target)->Normals[i],(*Target)->Vertices[i]*(-1)),(*Target)->Normals[i]);
        Tangent.Normalize();

        (*Target)->Tangents[i] = Tangent;

        lmVector3D Bitangent = lmCross(Tangent,(*Target)->Normals[i]);
        Bitangent.Normalize();

        (*Target)->Bitangents[i] = Bitangent;
    }

    std::cout << (**Target) << std::endl;
}
*/
/*
void lrmMd5Loader::Extract(lrmSkeletalMesh **Target,const string &Key,unsigned int MeshIndex)
{
    struct Vertex
    {
        lmVector2D TexCoord;
        list<md5File::md5Mesh::md5Weight> Weights;
    };

    vector<Vertex> Vertices;
    Vertices.resize(Models[Key]->Meshes[MeshIndex].Vertices.size());

    for(unsigned int i=0;i < Vertices.size();i++)
    {
        Vertices[i].TexCoord = Models[Key]->Meshes[MeshIndex].Vertices[i].TexCoord;

        for(int j=0;j < Models[Key]->Meshes[MeshIndex].Vertices[i].WeightCount;j++)
        {
            Vertices[i].Weights.push_back(Models[Key]->Meshes[MeshIndex].Weights[Models[Key]->Meshes[MeshIndex].Vertices[i].StartWeight+j]);
        }

        Vertices[i].Weights.sort(Cmp);
        if(Vertices[i].Weights.size() > lrmSkeletalMesh::MAX_WEIGHT_COUNT)
        {
            Vertices[i].Weights.resize(lrmSkeletalMesh::MAX_WEIGHT_COUNT);
            float WeightSum = 0.0;

            for(auto j=Vertices[i].Weights.begin();j != Vertices[i].Weights.end();j++)
            {
                WeightSum += j->WeightBias;
            }

            for(auto j=Vertices[i].Weights.begin();j != Vertices[i].Weights.end();j++)
            {
                j->WeightBias /= WeightSum;
            }
        }
    }

    *Target = new lrmSkeletalMesh;

    (*Target)->Vertices.resize(Vertices.size());
    (*Target)->Normals.resize(Vertices.size());
    (*Target)->Tangents.resize(Vertices.size());
    (*Target)->Bitangents.resize(Vertices.size());
    (*Target)->TexCoords.resize(Vertices.size());
    (*Target)->BoneId.resize(Vertices.size());
    (*Target)->Weights.resize(Vertices.size());

    for(unsigned int i=0;i < Vertices.size();i++)
    {
        int k=0;
        for(auto j=Vertices[i].Weights.begin();j != Vertices[i].Weights.end();j++)
        {
            md5File::md5Joint &Joint = Models[Key]->Joints[j->JointIndex];

            lmQuaternion q1 = Joint.Orientation;

            lmVector3D wPos;

            if(Y_Up)
            {
                Quat_YUp(q1,q1);
                Vector3D_YUp(j->wPosition,wPos);
            }
            else
            {
                wPos = j->wPosition;
            }

            q1 = q1.GetInverse();

			lmMatrix3x3 Rotation(lmMatrix3x3::IDENTITY);
			lmRotate3x3(Rotation,q1);

			//lmVector3D &Position = Models[Key]->Meshes[MeshIndex].BpMesh->Vertices[i];
            (*Target)->Vertices[i]/*[k]* /  = Models[Key]->Meshes[MeshIndex].BpMesh->Vertices[i];

            lmVector3D &Normal = Models[Key]->Meshes[MeshIndex].BpMesh->Normals[i];
            (*Target)->Normals[i]/*[k]* /   = /*Rotation * * /Normal;

            lmVector3D &Tangent = Models[Key]->Meshes[MeshIndex].BpMesh->Tangents[i];
            (*Target)->Tangents[i]/*[k]* /  = /*Rotation * * /Tangent;

            lmVector3D &Bitangent = Models[Key]->Meshes[MeshIndex].BpMesh->Bitangents[i];
            (*Target)->Bitangents[i]/*[k]* / = /*Rotation * * /Bitangent;

            (*Target)->BoneId[i][k]  = j->JointIndex;
            (*Target)->Weights[i][k] = j->WeightBias;

            k++;
        }

        (*Target)->TexCoords[i] = Vertices[i].TexCoord;
    }


    (*Target)->Material = Models[Key]->Meshes[MeshIndex].Material;
    (*Target)->IndexBuffer.resize(Models[Key]->Meshes[MeshIndex].Triangles.size()*3);
    int k = 0;
    for(unsigned int i=0;i < Models[Key]->Meshes[MeshIndex].Triangles.size();i++)
    {
        (*Target)->IndexBuffer[k] = Models[Key]->Meshes[MeshIndex].Triangles[i].V1;
        k++;

        (*Target)->IndexBuffer[k] = Models[Key]->Meshes[MeshIndex].Triangles[i].V2;
        k++;

        (*Target)->IndexBuffer[k] = Models[Key]->Meshes[MeshIndex].Triangles[i].V3;
        k++;
    }

    //(*Target)->Print(cout);
}
*/
/*
void lrmMd5Loader::ExtractAnim(liKeyframeAnimation **Target,liCharAnimFactory *char_anim_factory,const string &Str)
{
    lmQuaternion q({1.0,0.0,0.0},-PI/2.0);
    lmQuaternion qinv = q.GetInverse();

    vector<md5File::md5Joint>			Bones;
    vector<unsigned int>				Flags;
    vector<unsigned int>				StartIndex;
    //map<string,unsigned int>	BoneNames;
    vector<pair<string,unsigned int> >	BoneNames;

    Bones.resize(Anims[Str]->Hierarchy.size());
    StartIndex.resize(Anims[Str]->Hierarchy.size());
    Flags.resize(Anims[Str]->Hierarchy.size());

    for(unsigned int i = 0;i < Anims[Str]->Hierarchy.size();i++)
    {
        //Bones[i].Position    = Anims[Str]->Baseframe[i].Position;
        //Bones[i].Orientation = Anims[Str]->Baseframe[i].Orientation;
        //Bones[i].ParentIndex = Anims[Str]->Hierarchy[i].ParentIndex;
        StartIndex[i]        = Anims[Str]->Hierarchy[i].StartIndex;
        Flags[i]             = Anims[Str]->Hierarchy[i].Flags;
        //BoneNames[Anims[Str]->Hierarchy[i].Name] = i;
        BoneNames.push_back({Anims[Str]->Hierarchy[i].Name,i});
    }

    //*Target = new lrKeyframeAnimation;

    //(*Target)->SetFrameRate(Anims[Str]->FrameRate);
    //(*Target)->ResizeFrames(Anims[Str]->NumFrames);

    *Target = char_anim_factory->CreateKeyframeAnimation(Anims[Str]->FrameRate,Anims[Str]->NumFrames,Bones.size());

    for(unsigned int i=0;i < Anims[Str]->Frames.size();i++)
    {
    	for(unsigned int i = 0;i < Anims[Str]->Hierarchy.size();i++)
		{
			Bones[i].Position    = Anims[Str]->Baseframe[i].Position;
			Bones[i].Orientation = Anims[Str]->Baseframe[i].Orientation;
			Bones[i].ParentIndex = Anims[Str]->Hierarchy[i].ParentIndex;
			//StartIndex[i]        = Anims[Str]->Hierarchy[i].StartIndex;
			//Flags[i]             = Anims[Str]->Hierarchy[i].Flags;
			//BoneNames[Anims[Str]->Hierarchy[i].Name] = i;
		}

		(**Target)[i]->SetBoneNames(BoneNames);

        for(unsigned int k=0;k < Bones.size();k++)
        {
            int j = 0;

            bool QuatRecalc = false;
            if(Flags[k] & 1)
            {
                Bones[k].Position[0] = Anims[Str]->Frames[i][StartIndex[k] + j];
                j++;
            }
            if(Flags[k] & 2)
            {
                Bones[k].Position[1] = Anims[Str]->Frames[i][StartIndex[k] + j];
                j++;
            }
            if(Flags[k] & 4)
            {
                Bones[k].Position[2] = Anims[Str]->Frames[i][StartIndex[k] + j];
                j++;
            }
            if(Flags[k] & 8)
            {
                Bones[k].Orientation.X = Anims[Str]->Frames[i][StartIndex[k] + j];
                QuatRecalc = true;
                j++;
            }
            if(Flags[k] & 16)
            {
                Bones[k].Orientation.Y = Anims[Str]->Frames[i][StartIndex[k] + j];
                QuatRecalc = true;
                j++;
            }
            if(Flags[k] & 32)
            {
                Bones[k].Orientation.Z = Anims[Str]->Frames[i][StartIndex[k] + j];
                QuatRecalc = true;
                j++;
            }

            if(QuatRecalc)
            {
                ComputeQuaternion(Bones[k].Orientation.X,Bones[k].Orientation.Y,Bones[k].Orientation.Z,Bones[k].Orientation);
            }

            (*(**Target)[i])[k]->SetPosition(Bones[k].Position);
            (*(**Target)[i])[k]->SetOrientation(Bones[k].Orientation);
            (*(**Target)[i])[k]->SetParentIndex(Bones[k].ParentIndex);
        }

        //(**Target)[i] = lrSkeleton(Bones,BoneNames);

        if(Y_Up)
		{
			Skeleton_YUp(*(**Target)[i]);
		}
    }
}
*/
/*
void lrmMd5Loader::ExtractMaterial(lrMultiMaterial **Material,const std::string &Key)
{
    *Material = Models[Key]->Materials;
}
*/
lrmMd5Loader::lrmMd5Loader(const std::string &path,bool freeMaterials,bool y_Up):Path(path),FreeMaterials(freeMaterials),Y_Up(y_Up)
{
    /*if(!File[Key].LoadMd5Mesh(Path + filename))
    {
        cerr << "Failed to load Md5 mesh: " << filename;
    }
    else
    {
        for(unsigned int i=0;i < File.Meshes.size();i++)
        {
            ExtractBpMesh(&File.Meshes[i].BpMesh,i);
        }
    }*/
}

lrmMd5Loader::~lrmMd5Loader()
{
    for(auto i = Anims.begin();i!= Anims.end();i++)
    {
        delete i->second;
    }

    for(auto i = Models.begin();i!= Models.end();i++)
    {
        delete i->second;
    }
}


void md5ExtractBindPoseMesh(md5File &file,bool y_up,bool smooth_normals,unsigned int mesh_index,lrmStaticMesh &bind_pose_mesh)
{
    bind_pose_mesh.Vertices.resize(file.Meshes[mesh_index].Vertices.size());
    bind_pose_mesh.TexCoords.resize(file.Meshes[mesh_index].Vertices.size());
    /*
    if(!(Models[Key]->BindPoseSkeletonInitialized))
	{
		//std::vector<lrSkeleton::lrBone> Bones(Models[Key]->Joints.size());
		Models[Key]->BindPoseSkeleton.Resize(Models[Key]->Joints.size());
		Models[Key]->BindPoseSkeleton.SetBoneNames(Models[Key]->JointNames);
		//
		for(int i=0;i < Models[Key]->BindPoseSkeleton.Size();i++)
		{
			(Models[Key]->BindPoseSkeleton)[i]->SetPosition(Models[Key]->Joints[i].Position);
			(Models[Key]->BindPoseSkeleton)[i]->SetOrientation(Models[Key]->Joints[i].Orientation);
			(Models[Key]->BindPoseSkeleton)[i]->SetParentIndex(-1);
		}
		//
		//Models[Key]->BindPoseSkeleton = new lrSkeleton(Bones,Models[Key]->JointNames);
		//
		if(Y_Up)
		{
			Skeleton_YUp(Models[Key]->BindPoseSkeleton);
		}
		//
		Models[Key]->BindPoseSkeletonInitialized = true;
	}
	*/
    for(unsigned int i = 0; i < file.Meshes[mesh_index].Vertices.size();i++)
    {
        lmVector3D Vertex;
        for(int j=0;j < file.Meshes[mesh_index].Vertices[i].WeightCount;j++)
        {
        	unsigned int WeightIndex = file.Meshes[mesh_index].Vertices[i].StartWeight + j;
            md5File::md5Mesh::md5Weight &Weight = file.Meshes[mesh_index].Weights[WeightIndex];
            //md5Skeleton::md5Joint  &Joint = file.BindPoseSkeleton.Joints[Weight.JointIndex];
            md5JointData  &Joint = file.BindPoseSkeleton[Weight.JointIndex];
            //
            lmVector3D      wPos;
            lmVector3D      JointPos;
            lmQuaternion    JointOrientation;
            //
            if(y_up)
            {
                md5Utility::Vector3D_YUp(Weight.wPosition,wPos);
                md5Utility::Vector3D_YUp(Joint.Position,JointPos);
                md5Utility::Quat_YUp(Joint.Orientation,JointOrientation);
            }
            else
            {
                wPos = Weight.wPosition;
                JointPos = Joint.Position;
                JointOrientation = Joint.Orientation;
            }
            //
            lmVector3D Tmp = lmQuaternionRotate(wPos,JointOrientation) + JointPos;
            Vertex += Weight.WeightBias * Tmp;
            //
            //lmMatrix4x4 BoneTransform = Models[Key]->BindPoseSkeleton.GetTransformation(Weight.JointIndex);

            //lmVector4D Vertex4D = (lmTranslate4x4(JointPos) * lmRotate4x4(JointOrientation) * lmVector4D(wPos.X,wPos.Y,wPos.Z,1.0))*Weight.WeightBias;
            //lmVector4D Vertex4D = (BoneTransform * lmVector4D({wPos[0],wPos[1],wPos[2],1.0}))*Weight.WeightBias;

            //Vertex[0] += Vertex4D[0];
            //Vertex[1] += Vertex4D[1];
            //Vertex[2] += Vertex4D[2];
        }
        //
        bind_pose_mesh.Vertices[i] = Vertex;
        bind_pose_mesh.TexCoords[i] = file.Meshes[mesh_index].Vertices[i].TexCoord;
    }
    //
    bind_pose_mesh.Normals.resize(file.Meshes[mesh_index].Vertices.size());
    //
    lrmStaticMesh::lrmMtlGroup *newMtlGroup = new lrmStaticMesh::lrmMtlGroup;
    //bind_pose_mesh.MtlGroups.push_back(newMtlGroup);
    bind_pose_mesh.AddMaterialGroup(newMtlGroup);
    //
    newMtlGroup->Material = file.Meshes[mesh_index].Material;
    newMtlGroup->IndexBuffer.resize(file.Meshes[mesh_index].Triangles.size()*3);
    //
    unsigned int k = 0;
    for(auto &Triangle : file.Meshes[mesh_index].Triangles)
    {
        lmVector3D VecV1V3 = bind_pose_mesh.Vertices[Triangle.V3] - bind_pose_mesh.Vertices[Triangle.V1];
        lmVector3D VecV1V2 = bind_pose_mesh.Vertices[Triangle.V2] - bind_pose_mesh.Vertices[Triangle.V1];
        //
        Triangle.Normal += lmCross(VecV1V3,VecV1V2);
        Triangle.Normal.Normalize();
        //
        bind_pose_mesh.Normals[Triangle.V1] += Triangle.Normal;
        bind_pose_mesh.Normals[Triangle.V2] += Triangle.Normal;
        bind_pose_mesh.Normals[Triangle.V3] += Triangle.Normal;
        //
        newMtlGroup->IndexBuffer[k] = Triangle.V1;
        k++;
        newMtlGroup->IndexBuffer[k] = Triangle.V2;
        k++;
        newMtlGroup->IndexBuffer[k] = Triangle.V3;
        k++;
    }
    //
    bind_pose_mesh.Tangents.resize(file.Meshes[mesh_index].Vertices.size());
    bind_pose_mesh.Bitangents.resize(file.Meshes[mesh_index].Vertices.size());
    /*
     * Normalizing normal vectors and generating tangent space vectors
     */
    for(unsigned int i=0;i < bind_pose_mesh.Normals.size();i++)
    {
    	/*
    	 * TODO Ez itt kódduplikáció, ki kell vágni egy külön
    	 * függvénybe, mert a Wavefront loaderben is ugyanez van.
    	 */
        bind_pose_mesh.Normals[i].Normalize();
        //
        lmVector3D FirstCross = lmCross(bind_pose_mesh.Normals[i],bind_pose_mesh.Vertices[i]*(-1));
        lmVector3D Tangent = lmCross(FirstCross,bind_pose_mesh.Normals[i]);
        Tangent.Normalize();
		//
        bind_pose_mesh.Tangents[i] = Tangent;
		//
        lmVector3D Bitangent = lmCross(Tangent,bind_pose_mesh.Normals[i]);
        Bitangent.Normalize();
		//
        bind_pose_mesh.Bitangents[i] = Bitangent;
    }
}

bool md5CmpWeight(const md5File::md5Mesh::md5Weight &A,const md5File::md5Mesh::md5Weight&B)
{
    if(A.WeightBias > B.WeightBias)
    {
        return true;
    }
    return false;
}

void md5ExtractSkeletalMesh(md5File &file,bool y_up,bool smooth_normals,unsigned int mesh_index,lrmSkeletalMesh &skeletal_mesh)
{
	md5ExtractBindPoseMesh(file,y_up,smooth_normals,mesh_index,skeletal_mesh.BindPoseMesh);
	//
	unsigned int NumVertices = file.Meshes[mesh_index].Vertices.size();
    //
    skeletal_mesh.BoneId.resize(NumVertices);
    skeletal_mesh.Weights.resize(NumVertices);
    //
    for(unsigned int i=0;i < NumVertices;i++)
    {
        list<md5File::md5Mesh::md5Weight> Weights;
        //
        for(int j=0;j < file.Meshes[mesh_index].Vertices[i].WeightCount;j++)
        {
            Weights.push_back(file.Meshes[mesh_index].Weights[file.Meshes[mesh_index].Vertices[i].StartWeight+j]);
        }
        //
        Weights.sort(md5CmpWeight);
        if(Weights.size() > lrmSkeletalMesh::MAX_WEIGHT_COUNT)
        {
            Weights.resize(lrmSkeletalMesh::MAX_WEIGHT_COUNT);
            float WeightSum = 0.0;
            //
            for(md5File::md5Mesh::md5Weight &j : Weights)
            {
                WeightSum += j.WeightBias;
            }
            //
            for(md5File::md5Mesh::md5Weight &j : Weights)
            {
                j.WeightBias /= WeightSum;
            }
        }
        //
        int k=0;
		for(md5File::md5Mesh::md5Weight &j : Weights)
		{
			skeletal_mesh.BoneId[i][k]  = j.JointIndex;
			skeletal_mesh.Weights[i][k] = j.WeightBias;
			//
			k++;
		}
    }
}

void md5ExtractAnimFrame(md5Anim &anim,bool y_up,unsigned int frame_index,md5SkeletonExtrinsic &anim_frame)
{
	anim_frame.Resize(anim.Hierarchy.size());
	//
	for(unsigned int i = 0;i < anim_frame.Size();i++)
	{
		anim_frame.JointData[i].Position    = anim.Baseframe[i].Position;
		anim_frame.JointData[i].Orientation = anim.Baseframe[i].Orientation;
	}
	//
	for(unsigned int k=0;k < anim_frame.Size();k++)
	{
		unsigned int StartIndex = anim.Hierarchy[k].StartIndex;
		int j = 0;
		//
		bool QuatRecalc = false;
		if(anim.Hierarchy[k].Flags & 1)
		{
			anim_frame.JointData[k].Position[0] = anim.Frames[frame_index][StartIndex + j];
			j++;
		}
		if(anim.Hierarchy[k].Flags & 2)
		{
			anim_frame.JointData[k].Position[1] = anim.Frames[frame_index][StartIndex + j];
			j++;
		}
		if(anim.Hierarchy[k].Flags & 4)
		{
			anim_frame.JointData[k].Position[2] = anim.Frames[frame_index][StartIndex + j];
			j++;
		}
		if(anim.Hierarchy[k].Flags & 8)
		{
			anim_frame.JointData[k].Orientation.X = anim.Frames[frame_index][StartIndex + j];
			QuatRecalc = true;
			j++;
		}
		if(anim.Hierarchy[k].Flags & 16)
		{
			anim_frame.JointData[k].Orientation.Y = anim.Frames[frame_index][StartIndex + j];
			QuatRecalc = true;
			j++;
		}
		if(anim.Hierarchy[k].Flags & 32)
		{
			anim_frame.JointData[k].Orientation.Z = anim.Frames[frame_index][StartIndex + j];
			QuatRecalc = true;
			j++;
		}
		//
		if(QuatRecalc)
		{
			md5Utility::ComputeQuaternion(anim_frame.JointData[k].Orientation.X,anim_frame.JointData[k].Orientation.Y,anim_frame.JointData[k].Orientation.Z,anim_frame.JointData[k].Orientation);
		}
	}
	//
	if(y_up)
	{
		md5Utility::Skeleton_YUp(anim_frame,anim_frame);
	}
}
