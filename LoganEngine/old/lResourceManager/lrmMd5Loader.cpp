
#include "lrmMd5Loader.h"

using namespace std;

std::string lrmMd5Loader::ImgPath;

bool lrmMd5Loader::md5File::LoadMd5Mesh(const string &Path,const string &filename)
{
    ifstream fin;
    fin.open((Path + filename).c_str());

    if(!fin.is_open())
    {
        return false;
    }

    unsigned int LoadedMeshes = 0;

    while(!fin.eof())
    {
        string Tmp;
        fin >> Tmp;

        if(Tmp.size() > 0)
        {
            if(Tmp == "MD5Version")
            {
                int MD5Version;
                fin >> MD5Version;
                if(MD5Version != 10)
                {
                    return false;
                }
            }
            else if(Tmp == "commandline")
            {
                getline(fin,Tmp);
                Tmp.clear();
            }
            else if(Tmp == "numJoints")
            {
                int numJoints;
                fin >> numJoints;
                Joints.resize(numJoints);
            }
            else if(Tmp == "numMeshes")
            {
                int numMeshes;
                fin >> numMeshes;
                Meshes.resize(numMeshes);
            }
            else if(Tmp == "mesh")
            {
                fin >> Tmp;
                while(Tmp != "}")
                {
                    if(Tmp == "shader")
                    {
                        string MtlName;
                        LoadQuotedString(fin,MtlName);

                        if(!Materials->CheckMaterial(MtlName))
                        {
                            string DiffuseMap   = MtlName + ".bmp";
                            string NormalMap    = MtlName + "_local.bmp";
                            string SpecularMap  = MtlName + "_s.bmp";
                            //string EmissiveMap  = MtlName + ".bmp";

                            float Diffuse[3] = {1.0,1.0,1.0};
                            float Specular[3] = {1.0,1.0,1.0};
                            float Shininess = 100.0;

                            lrMaterial *newMaterial = new lrMaterial(Diffuse,Specular,Shininess);

                            ifstream fin;

                            fin.open((ImgPath + DiffuseMap).c_str());
                            if(fin.is_open())
                            {
                                newMaterial->SetDiffuseMap(DiffuseMap);
                            }
                            fin.close();

                            fin.open((ImgPath + NormalMap).c_str());
                            if(fin.is_open())
                            {
                                newMaterial->SetNormalMap(NormalMap);
                            }
                            fin.close();

                            fin.open((ImgPath + SpecularMap).c_str());
                            if(fin.is_open())
                            {
                                newMaterial->SetSpecularMap(SpecularMap);
                            }
                            fin.close();

                            Materials->AddMaterial(MtlName,newMaterial);
                        }

                        Meshes[LoadedMeshes].Material = MtlName;
                    }
                    else if(Tmp == "numverts")
                    {
                        int numverts;
                        fin >> numverts;
                        Meshes[LoadedMeshes].Vertices.resize(numverts);
                        for(int i = 0;i < numverts;i++)
                        {
                            fin >> Tmp;
                            int Index;
                            fin >> Index;

                            fin >> Tmp;
                            fin >> Meshes[LoadedMeshes].Vertices[Index].TexCoord[0];
                            fin >> Meshes[LoadedMeshes].Vertices[Index].TexCoord[1];
                            fin >> Tmp;
                            fin >> Meshes[LoadedMeshes].Vertices[Index].StartWeight;
                            fin >> Meshes[LoadedMeshes].Vertices[Index].WeightCount;
                        }
                    }
                    else if(Tmp == "numtris")
                    {
                        int numtris;
                        fin >> numtris;
                        Meshes[LoadedMeshes].Triangles.resize(numtris);
                        for(int i = 0;i < numtris;i++)
                        {
                            fin >> Tmp;
                            int Index;
                            fin >> Index;

                            fin >> Meshes[LoadedMeshes].Triangles[Index].V1;
                            fin >> Meshes[LoadedMeshes].Triangles[Index].V2;
                            fin >> Meshes[LoadedMeshes].Triangles[Index].V3;
                        }
                    }
                    else if(Tmp == "numweights")
                    {
                        int numweights;
                        fin >> numweights;
                        Meshes[LoadedMeshes].Weights.resize(numweights);
                        for(int i = 0;i < numweights;i++)
                        {
                            fin >> Tmp;
                            int Index;
                            fin >> Index;

                            fin >> Meshes[LoadedMeshes].Weights[Index].JointIndex;
                            fin >> Meshes[LoadedMeshes].Weights[Index].WeightBias;
                            fin >> Tmp;
                            fin >> Meshes[LoadedMeshes].Weights[Index].wPosition[0];
                            fin >> Meshes[LoadedMeshes].Weights[Index].wPosition[1];
                            fin >> Meshes[LoadedMeshes].Weights[Index].wPosition[2];
                             /*
                            std::cout <<	Meshes[LoadedMeshes].Weights[Index].wPosition[0] << ";" <<
											Meshes[LoadedMeshes].Weights[Index].wPosition[1] << ";" <<
											Meshes[LoadedMeshes].Weights[Index].wPosition[2] << std::endl;
							 */

                            fin >> Tmp;
                        }
                    }

                    fin >> Tmp;
                }
                LoadedMeshes++;
            }
            else if(Tmp == "joints")
            {
                fin >> Tmp;
                for(unsigned int i=0;i < Joints.size();i++)
                {
                    float ox,oy,oz;
                    string Key;
                    LoadQuotedString(fin,Key);

                    fin >> Joints[i].ParentIndex;
                    fin >> Tmp;

                    fin >> Joints[i].Position[0];
                    fin >> Joints[i].Position[1];
                    fin >> Joints[i].Position[2];

                     /*
					std::cout <<	Joints[i].Position[0] << ";" <<
									Joints[i].Position[1] << ";" <<
									Joints[i].Position[2] << std::endl;
					 */

                    fin >> Tmp;
                    fin >> Tmp;

                    fin >> ox;
                    fin >> oy;
                    fin >> oz;

                    fin >> Tmp;
                    ComputeQuaternion(ox,oy,oz,Joints[i].Orientation);

                    JointNames[Key] = i;
                }
            }
        }
    }

    return true;
}

void lrmMd5Loader::md5File::Print(ostream &out)
{
    out << "numJoints " << Joints.size() << "\n";
    out << "numMeshes " << Meshes.size() << "\n\n";
    out << "joints {\n";

    for(unsigned int i = 0;i < Joints.size();i++)
    {
        string Key;
        for(auto I = JointNames.begin();I != JointNames.end();I++)
        {
            if(I->second == i)
            {
                Key = I->first;
                break;
            }
        }
        out << "\"" << Key << "\"" << " " << Joints[i].ParentIndex
            << " ( " << Joints[i].Position[0] << " " << Joints[i].Position[1] << " " << Joints[i].Position[2] << " ) "
            << " ( " << Joints[i].Orientation.X << " " << Joints[i].Orientation.Y << " " << Joints[i].Orientation.Z << " )\n";
    }

    out << "}\n\n";

    for(auto I = Meshes.begin();I != Meshes.end();I++)
    {
        out << "mesh {\n";
        out << "shader \"" << I->Material << "\"\n\n";
        out << "numverts " << I->Vertices.size() << "\n";
        for(unsigned int i = 0;i < I->Vertices.size();i++)
        {
            out << "vert " << i << " ( " << I->Vertices[i].TexCoord[0] << " " << I->Vertices[i].TexCoord[1] << " ) "
                << I->Vertices[i].StartWeight << " " << I->Vertices[i].WeightCount << "\n";
        }
        out << "\n";
        out << "numtris " << I->Triangles.size() << "\n";
        for(unsigned int i = 0;i < I->Triangles.size();i++)
        {
            out << "tri " << i << " " << I->Triangles[i].V1 << " " << I->Triangles[i].V2 << " "<< I->Triangles[i].V3 << "\n";
        }
        out << "\n";
        out << "numweights " << I->Weights.size() << "\n";
        for(unsigned int i = 0;i < I->Weights.size();i++)
        {
            out << "weight " << i << " " << I->Weights[i].JointIndex << " " << I->Weights[i].WeightBias
                << " ( " << I->Weights[i].wPosition[0] << " " << I->Weights[i].wPosition[1] << " " << I->Weights[i].wPosition[2] << " )\n";
        }
        out << "\n";
        out << "}\n\n";
    }
}

bool lrmMd5Loader::md5Anim::LoadMd5Anim(const string &Path,const string &filename)
{
    fstream fin;
    fin.open((Path + filename).c_str());

    if(!fin.is_open())
    {
        return false;
    }

    while(!fin.eof())
    {
        string Tmp;
        fin >> Tmp;

        if(Tmp.size() > 0)
        {
            if(Tmp == "MD5Version")
            {
                int MD5Version;
                fin >> MD5Version;
                if(MD5Version != 10)
                {
                    return false;
                }
            }
            else if(Tmp == "commandline")
            {
                getline(fin,Tmp);
                Tmp.clear();
            }
            if(Tmp == "numJoints")
            {
                fin >> NumJoints;
                Baseframe.resize(NumJoints);
            }
            else if(Tmp == "numFrames")
            {
                fin >> NumFrames;
                Frames.resize(NumFrames);
                Bounds.resize(NumFrames);
            }
            else if(Tmp == "frameRate")
            {
                fin >> FrameRate;
            }
            else if(Tmp == "numAnimatedComponents")
            {
                fin >> NumAnimatedComponents;
                for(unsigned int i=0;i < Frames.size();i++)
                {
                    Frames[i].resize(NumAnimatedComponents);
                }
            }
            else if(Tmp == "hierarchy")
            {
                fin >> Tmp;
                Hierarchy.resize(NumJoints);
                for(int i=0;i < NumJoints;i++)
                {
                    string Name;
                    LoadQuotedString(fin,Name);
                    //md5Hierarchy newHierarchy;

                    Hierarchy[i].Name = Name;
                    fin >> Hierarchy[i].ParentIndex;
                    fin >> Hierarchy[i].Flags;
                    fin >> Hierarchy[i].StartIndex;

                    //Hierarchy[i] = newHierarchy;
                }
            }
            else if(Tmp == "bounds")
            {
                fin >> Tmp;
                for(unsigned int i=0;i < Bounds.size();i++)
                {
                    fin >> Tmp;
                    fin >> Bounds[i].Min[0];
                    fin >> Bounds[i].Min[1];
                    fin >> Bounds[i].Min[2];
                    fin >> Tmp;
                    fin >> Tmp;
                    fin >> Bounds[i].Max[0];
                    fin >> Bounds[i].Max[1];
                    fin >> Bounds[i].Max[2];
                    fin >> Tmp;
                }
            }
            else if(Tmp == "baseframe")
            {
                fin >> Tmp;
                for(unsigned int i=0;i < Baseframe.size();i++)
                {
                    float x,y,z;

                    fin >> Tmp;

                    fin >> Baseframe[i].Position[0];
                    fin >> Baseframe[i].Position[1];
                    fin >> Baseframe[i].Position[2];

                    fin >> Tmp;
                    fin >> Tmp;
                    fin >> x;
                    fin >> y;
                    fin >> z;
                    fin >> Tmp;

                    ComputeQuaternion(x,y,z,Baseframe[i].Orientation);
                }
            }
            else if(Tmp == "frame")
            {
                int CurrentFrame;
                fin >> CurrentFrame;
                fin >> Tmp;

                for(unsigned int i=0;i < Frames[CurrentFrame].size();i++)
                {
                    fin >> Frames[CurrentFrame][i];
                }
            }
        }
    }

    return true;
}

void lrmMd5Loader::md5Anim::Print(ostream &out)
{
    out << "numFrames "             << NumFrames << "\n";
    out << "numJoints "             << NumJoints << "\n";
    out << "frameRate "             << FrameRate << "\n";
    out << "numAnimatedComponents " << NumAnimatedComponents << "\n\n";

    out << "hierarchy {\n";
    for(auto i = Hierarchy.begin();i != Hierarchy.end();i++)
    {
        out << "\"" << i->Name << "\" " << i->ParentIndex << " " << i->Flags << " " << i->StartIndex << "\n";
    }
    out << "}\n\n";

    out << "bounds {\n";
    for(auto i = Bounds.begin();i != Bounds.end();i++)
    {
        out << " ( " << i->Min[0] << " " << i->Min[1] << " " << i->Min[2] << " ) " << "( " << i->Max[0] << " " << i->Max[1] << " " << i->Max[2] << " )\n";
    }
    out << "}\n\n";

    out << "baseframe {\n";
    for(auto i = Baseframe.begin();i != Baseframe.end();i++)
    {
        out << " ( " << i->Position[0] << " " << i->Position[1] << " " << i->Position[2] << " ) " << "( " << i->Orientation.X << " " << i->Orientation.Y << " " << i->Orientation.Z << " )\n";
    }
    out << "}\n\n";

    for(unsigned int i = 0;i < Frames.size();i++)
    {
        out << "frame " << i << " {\n";
        for(auto j = Frames[i].begin();j != Frames[i].end();j++)
        {
            out << *j << " ";j++;out << *j << " ";j++;out << *j << " ";j++;out << *j << " ";j++;out << *j << " ";j++;out << *j << "\n";
        }
        out << "}\n\n";
    }
}

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
}

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
}

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

void lrmMd5Loader::Vector3D_YUp(const lmVector3D &v,lmVector3D &Target)
{
    //Target[0] =  v[0];
    //Target[1] =  v[2];
    //Target[2] = -v[1];

    Target = {v[0],v[2],-v[1]};
}

void lrmMd5Loader::Quat_YUp(const lmQuaternion &p,lmQuaternion &Target)
{
    lmQuaternion q({1.0,0.0,0.0},-PI/2.0);
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
    md5Anim *Anim = new md5Anim;
    if(Anim->LoadMd5Anim(Path,filename))
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
    md5File *File = new md5File(FreeMaterials);

    if(!File->LoadMd5Mesh(Path,filename))
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
            ExtractBpMesh(&File->Meshes[i].BpMesh,filename,i);
        }

        //File->Print(cout);
        return true;
    }
}

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
        */

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
            (*Target)->Vertices[i]/*[k]*/  = Models[Key]->Meshes[MeshIndex].BpMesh->Vertices[i];

            lmVector3D &Normal = Models[Key]->Meshes[MeshIndex].BpMesh->Normals[i];
            (*Target)->Normals[i]/*[k]*/   = /*Rotation * */Normal;

            lmVector3D &Tangent = Models[Key]->Meshes[MeshIndex].BpMesh->Tangents[i];
            (*Target)->Tangents[i]/*[k]*/  = /*Rotation * */Tangent;

            lmVector3D &Bitangent = Models[Key]->Meshes[MeshIndex].BpMesh->Bitangents[i];
            (*Target)->Bitangents[i]/*[k]*/ = /*Rotation * */Bitangent;

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

void lrmMd5Loader::ExtractMaterial(lrMultiMaterial **Material,const std::string &Key)
{
    *Material = Models[Key]->Materials;
}

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
