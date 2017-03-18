
#include "md5Utility.h"
#include "md5File.h"

void md5File::Print(std::ostream &out)
{
    out << "numJoints " << BindPoseSkeletonMetadata.JointMetadata.size() << "\n";
    out << "numMeshes " << Meshes.size() << "\n\n";
    out << "joints {\n";

    for(unsigned int i = 0;i < BindPoseSkeletonMetadata.JointMetadata.size();i++)
    {
        std::string Key;
        for(auto I = BindPoseSkeletonMetadata.JointNames.begin();I != BindPoseSkeletonMetadata.JointNames.end();I++)
        {
            if(I->second == i)
            {
                Key = I->first;
                break;
            }
        }
        out << "\"" << Key << "\"" << " " << BindPoseSkeletonMetadata.JointMetadata[i].ParentIndex
            << " ( " << BindPoseSkeleton[i].Position[0] << " " << BindPoseSkeleton[i].Position[1] << " " << BindPoseSkeleton[i].Position[2] << " ) "
            << " ( " << BindPoseSkeleton[i].Orientation.X << " " << BindPoseSkeleton[i].Orientation.Y << " " << BindPoseSkeleton[i].Orientation.Z << " )\n";
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

bool md5LoadFile(std::istream &in,md5File &file)
{
	unsigned int LoadedMeshes = 0;

    while(!in.eof())
    {
        std::string Tmp;
        in >> Tmp;

        if(Tmp.size() > 0)
        {
            if(Tmp == "MD5Version")
            {
                int MD5Version;
                in >> MD5Version;
                if(MD5Version != 10)
                {
                	std::cerr << "Error: Md5 version mismatch" << std::endl;
                    return false;
                }
            }
            else if(Tmp == "commandline")
            {
                getline(in,Tmp);
                Tmp.clear();
            }
            else if(Tmp == "numJoints")
            {
                int numJoints;
                in >> numJoints;
                file.BindPoseSkeletonMetadata.JointMetadata.resize(numJoints);
                file.BindPoseSkeleton.Resize(numJoints);
            }
            else if(Tmp == "numMeshes")
            {
                int numMeshes;
                in >> numMeshes;
                file.Meshes.resize(numMeshes);
            }
            else if(Tmp == "mesh")
            {
                in >> Tmp;
                while(Tmp != "}")
                {
                    if(Tmp == "shader")
                    {
                        std::string MtlName;
                        md5Utility::LoadQuotedString(in,MtlName);
                        /*
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
                        */
                        file.Meshes[LoadedMeshes].Material = MtlName;
                    }
                    else if(Tmp == "numverts")
                    {
                        int numverts;
                        in >> numverts;
                        file.Meshes[LoadedMeshes].Vertices.resize(numverts);
                        for(int i = 0;i < numverts;i++)
                        {
                            in >> Tmp;
                            int Index;
                            in >> Index;

                            in >> Tmp;
                            in >> file.Meshes[LoadedMeshes].Vertices[Index].TexCoord[0];
                            in >> file.Meshes[LoadedMeshes].Vertices[Index].TexCoord[1];
                            in >> Tmp;
                            in >> file.Meshes[LoadedMeshes].Vertices[Index].StartWeight;
                            in >> file.Meshes[LoadedMeshes].Vertices[Index].WeightCount;
                        }
                    }
                    else if(Tmp == "numtris")
                    {
                        int numtris;
                        in >> numtris;
                        file.Meshes[LoadedMeshes].Triangles.resize(numtris);
                        for(int i = 0;i < numtris;i++)
                        {
                            in >> Tmp;
                            int Index;
                            in >> Index;

                            in >> file.Meshes[LoadedMeshes].Triangles[Index].V1;
                            in >> file.Meshes[LoadedMeshes].Triangles[Index].V2;
                            in >> file.Meshes[LoadedMeshes].Triangles[Index].V3;
                        }
                    }
                    else if(Tmp == "numweights")
                    {
                        int numweights;
                        in >> numweights;
                        file.Meshes[LoadedMeshes].Weights.resize(numweights);
                        for(int i = 0;i < numweights;i++)
                        {
                            in >> Tmp;
                            int Index;
                            in >> Index;

                            in >> file.Meshes[LoadedMeshes].Weights[Index].JointIndex;
                            in >> file.Meshes[LoadedMeshes].Weights[Index].WeightBias;
                            in >> Tmp;
                            in >> file.Meshes[LoadedMeshes].Weights[Index].wPosition[0];
                            in >> file.Meshes[LoadedMeshes].Weights[Index].wPosition[1];
                            in >> file.Meshes[LoadedMeshes].Weights[Index].wPosition[2];
                             /*
                            std::cout <<	Meshes[LoadedMeshes].Weights[Index].wPosition[0] << ";" <<
											Meshes[LoadedMeshes].Weights[Index].wPosition[1] << ";" <<
											Meshes[LoadedMeshes].Weights[Index].wPosition[2] << std::endl;
							 */

                            in >> Tmp;
                        }
                    }

                    in >> Tmp;
                }
                LoadedMeshes++;
            }
            else if(Tmp == "joints")
            {
                in >> Tmp;
                for(unsigned int i=0;i < file.BindPoseSkeleton.Size();i++)
                {
                    float ox,oy,oz;
                    std::string Key;
                    md5Utility::LoadQuotedString(in,Key);

                    in >> file.BindPoseSkeletonMetadata.JointMetadata[i].ParentIndex;
                    file.BindPoseSkeletonMetadata.JointMetadata[i].Index = i;
                    in >> Tmp;

                    in >> file.BindPoseSkeleton[i].Position[0];
                    in >> file.BindPoseSkeleton[i].Position[1];
                    in >> file.BindPoseSkeleton[i].Position[2];
                    //
                     /*
					std::cout <<	Joints[i].Position[0] << ";" <<
									Joints[i].Position[1] << ";" <<
									Joints[i].Position[2] << std::endl;
					 */
                    //
                    in >> Tmp;
                    in >> Tmp;

                    in >> ox;
                    in >> oy;
                    in >> oz;
                    //
                    in >> Tmp;
                    md5Utility::ComputeQuaternion(ox,oy,oz,file.BindPoseSkeleton[i].Orientation);
                    //
                    file.BindPoseSkeletonMetadata.JointNames[Key] = i;
                }
            }
        }
    }
    //
    return true;
}
