#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>

#include "lrmWavefrontLoader.h"

#include "../lMath/lMath.h"

using namespace std;

void lrmWavefrontLoader::CalculateTBVectors(lrmStaticMesh &Target)
{
    for(unsigned int i=0;i < Target.Normals.size();i++)
    {
        lmVector3D Tangent = lmCross(lmCross(Target.Normals[i],Target.Vertices[i]*(-1)),Target.Normals[i]);
        Tangent.Normalize();

        Target.Tangents.push_back(Tangent);

        lmVector3D Bitangent = lmCross(Tangent,Target.Normals[i]);
        Bitangent.Normalize();

        Target.Bitangents.push_back(Bitangent);
    }
}

int lrmWavefrontLoader::LoadWavefrontMtl(const string &filename)
{
    if(MultiMaterial == nullptr)
    {return 1;}

    fstream in;
    in.open(filename.c_str());

    if(!in.is_open())
    {return 2;}
    else
    {
        /*TODO: Adj hozzá Dissolve (átlátszóság) támogatást!*/
        /*TODO: Adj hozzá Specular Map támogatást!*/
        string MaterialName;

        float Shininess = 0.0;

        bool DiffuseMapFound = false;
        string DiffuseMap;
        bool NormalMapFound = false;
        string NormalMap;
        bool EmissiveMapFound = false;
        string EmissiveMap;

        float Diffuse[3]  = {0.0,0.0,0.0};
        float Specular[3] = {0.0,0.0,0.0};

        bool FirstMat = true;
        while(!in.eof())
        {
            string Tmp;
            getline(in,Tmp);

            if((Tmp[0] == 'n')&&(Tmp[1] == 'e')&&(Tmp[2] == 'w')&&(Tmp[3] == 'm'))
            {
                char cTmp[50];

                if(FirstMat == true)
                {
                    sscanf(Tmp.c_str(),"newmtl %s",cTmp);
                    MaterialName = cTmp;
                    FirstMat = false;
                }
                else
                {
                    #ifdef L_TESTING
                        cout << MaterialName << endl;

                        cout << Diffuse[0] << ";"<<Diffuse[1] << ";" << Diffuse[2] << endl;
                        cout << Specular[0] << ";"<<Specular[1] << ";" << Specular[2] << endl;

                        cout << Shininess << endl << DiffuseMap << endl << NormalMap << endl;
                    #endif

                    lrMaterial *newMaterial = new lrMaterial(Diffuse,Specular,Shininess);
                    if(DiffuseMapFound)
                    {
                        newMaterial->SetDiffuseMap(DiffuseMap);
                    }
                    if(NormalMapFound)
                    {
                        newMaterial->SetNormalMap(NormalMap);
                    }

                    if(MultiMaterial->AddMaterial(MaterialName,newMaterial))
                    {
                        cerr << "Error!\nFile corrupted: "<< filename << endl;
                        delete newMaterial;
                    }

                    sscanf(Tmp.c_str(),"newmtl %s",cTmp);
                    MaterialName = cTmp;

                    Shininess = 0.0;

                    DiffuseMapFound = false;
                    DiffuseMap.clear();
                    NormalMapFound = false;
                    NormalMap.clear();

                    Diffuse[0]  = 0.0;Diffuse[1]  = 0.0;Diffuse[2]  = 0.0;
                    Specular[0] = 0.0;Specular[1] = 0.0;Specular[2] = 0.0;
                }
            }
            else if((Tmp[0] == 'K')&&(Tmp[1] == 'd'))
            {
                sscanf(Tmp.c_str(),"Kd %f %f %f",&Diffuse[0],&Diffuse[1],&Diffuse[2]);
            }
            else if((Tmp[0] == 'K')&&(Tmp[1] == 's'))
            {
                sscanf(Tmp.c_str(),"Ks %f %f %f",&Specular[0],&Specular[1],&Specular[2]);
            }
            else if((Tmp[0] == 'N')&&(Tmp[1] == 's'))
            {
                sscanf(Tmp.c_str(),"Ns %f",&Shininess);
            }
            else if((Tmp[0] == 'm')&&(Tmp[1] == 'a')&&(Tmp[2] == 'p')&&(Tmp[3] == '_')&&(Tmp[4] == 'K')&&(Tmp[5] == 'd'))
            {
                char cTmp[50];
                sscanf(Tmp.c_str(),"map_Kd %s",cTmp);
                DiffuseMap = cTmp;
                DiffuseMapFound = true;
            }
            else if((Tmp[0] == 'm')&&(Tmp[1] == 'a')&&(Tmp[2] == 'p')&&(Tmp[3] == '_')&&(Tmp[4] == 'K')&&(Tmp[5] == 's'))
            {
                char cTmp[50];
                sscanf(Tmp.c_str(),"map_Ks %s",cTmp);
                cout << "Specular map is not yet supported" << endl;/*Majd ide kell a Specular Map támogatás*/
            }
            else if((Tmp[0] == 'm')&&(Tmp[1] == 'a')&&(Tmp[2] == 'p')&&(Tmp[3] == '_')&&(Tmp[4] == 'b'))
            {
                char cTmp[50];
                sscanf(Tmp.c_str(),"map_bump %s",cTmp);
                NormalMap = cTmp;
                NormalMapFound = true;
            }
            else if((Tmp[0] == 'b')&&(Tmp[1] == 'u')&&(Tmp[2] == 'm')&&(Tmp[3] == 'p'))
            {
                char cTmp[50];
                sscanf(Tmp.c_str(),"bump %s",cTmp);
                NormalMap = cTmp;
                NormalMapFound = true;
            }
            else if((Tmp[0] == 'L')&&(Tmp[1] == 'O')&&(Tmp[2] == 'G')&&(Tmp[3] == 'A'))
            {
                char cTmp[50];
                sscanf(Tmp.c_str(),"LOGAN_EMISSIVE %s",cTmp);
                EmissiveMap = cTmp;
                EmissiveMapFound = true;
            }

        }

        #ifdef L_TESTING
            cout << MaterialName << endl;

            cout << Diffuse[0] << ";"<<Diffuse[1] << ";" << Diffuse[2] << endl;
            cout << Specular[0] << ";"<<Specular[1] << ";" << Specular[2] << endl;

            cout << Shininess << endl << DiffuseMap << endl << NormalMap << endl;
        #endif

        lrMaterial *newMaterial = new lrMaterial(Diffuse,Specular,Shininess);
        if(DiffuseMapFound)
        {
            newMaterial->SetDiffuseMap(DiffuseMap);
        }
        if(NormalMapFound)
        {
            newMaterial->SetNormalMap(NormalMap);
        }
        if(EmissiveMapFound)
		{
			newMaterial->SetEmissiveMap(EmissiveMap);
		}

        if(MultiMaterial->AddMaterial(MaterialName,newMaterial))
        {
            cerr << "Error!\nFile corrupted: "<< filename << endl;
            delete newMaterial;
        }
    }

    return 0;
}

int lrmWavefrontLoader::LoadWavefrontObj(const std::string &materialPath,const std::string &staticMeshPath,const std::string &filename)
{
    fstream in;
    in.open((staticMeshPath + filename).c_str());

    if(!in.is_open())
    {
        cerr << "Couldn't open file " << filename << endl;
        return 1;
    }

    unsigned int CurrentMtlGroup = -1;

    while(!in.eof())
    {
        string Tmp;
        getline(in,Tmp);

        if(Tmp[0] == 'm' && Tmp[1] == 't' && Tmp[2] == 'l' && Tmp[3] == 'l' && Tmp[4] == 'i' && Tmp[5] == 'b')
        {
            char MtlFile[50];
            sscanf(Tmp.c_str(),"mtllib %s",MtlFile);

            MaterialFileName = MtlFile;
            MultiMaterial = new lrMultiMaterial;

            LoadWavefrontMtl(materialPath + MaterialFileName);
        }
        else if(Tmp[0] == 'v' && Tmp[1] == 't')
        {
            float u,v;
            sscanf(Tmp.c_str(),"vt %f %f",&u,&v);
            File.TexCoords.push_back({u,v});
        }
        else if(Tmp[0] == 'v' && Tmp[1] == 'n')
        {
            float n1,n2,n3;
            sscanf(Tmp.c_str(),"vn %f %f %f",&n1,&n2,&n3);
            File.Normals.push_back({n1,n2,n3});
        }
        else if(Tmp[0] == 'v')
        {
            float v1,v2,v3;
            sscanf(Tmp.c_str(),"v %f %f %f",&v1,&v2,&v3);
            File.Vertices.push_back({v1,v2,v3});
        }
        else if(Tmp[0] == 'u' && Tmp[1] == 's' && Tmp[2] == 'e' && Tmp[3] == 'm' && Tmp[4] == 't' && Tmp[5] == 'l')
        {
            char cTmp[50];
            sscanf(Tmp.c_str(),"usemtl %s",cTmp);

            File.MatGroups.push_back(new lrmWfFile::lrmWfMatGroup);
            CurrentMtlGroup++;
            lrmWfFile::lrmWfMatGroup *Current = File.MatGroups[CurrentMtlGroup];
            Current->Material = cTmp;
        }
        else if(Tmp[0] == 'f')
        {
            lrmWfFile::lrmWfTriangle Triangle;
            int Normal;
            sscanf(Tmp.c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&Triangle.V1,&Triangle.Tx1,&Normal,&Triangle.V2,&Triangle.Tx2,&Normal,&Triangle.V3,&Triangle.Tx3,&Normal);

            Triangle.V1 -= 1;Triangle.V2 -= 1;Triangle.V3 -= 1;
            Triangle.Tx1 -= 1;Triangle.Tx2 -= 1;Triangle.Tx3 -= 1;
            Triangle.Normal = Normal-1;

            lrmWfFile::lrmWfMatGroup *Current = File.MatGroups[CurrentMtlGroup];
            if(Current != nullptr)
            {
                Current->Triangles.push_back(Triangle);
            }
        }
    }

    return 0;
}



void lrmWavefrontLoader::SmoothNormals()
{
    for(unsigned int i=0;i < File.Vertices.size();i++)
    {
        lmVector3D NewNormal;

        for(auto j=File.MatGroups.begin();j != File.MatGroups.end();j++)
        {
            for(auto k=(*j)->Triangles.begin();k != (*j)->Triangles.end();k++)
            {
                if(k->V1 == (int)i)
                {NewNormal += File.Normals[k->Normal];}
                else if(k->V2 == (int)i)
                {NewNormal += File.Normals[k->Normal];}
                else if(k->V3 == (int)i)
                {NewNormal += File.Normals[k->Normal];}
            }
        }

        NewNormal.Normalize();

        File.smoothNormals.push_back(NewNormal);
    }
}

const std::string &lrmWavefrontLoader::GetMaterialFileName() const
{
    return MaterialFileName;
};

lrMultiMaterial *lrmWavefrontLoader::GetMultiMaterial()
{
    return MultiMaterial;
};

void lrmWavefrontLoader::Extract(lrmStaticMesh **Target)
{
    lrmStaticMesh *newModel = new lrmStaticMesh;

    std::vector<int> iVertices;
    std::vector<int> iNormals;
    std::vector<int> iTexCoords;

    for(auto i = File.MatGroups.begin();i != File.MatGroups.end();i++)
    {
        for(auto k = (*i)->Triangles.begin();k != (*i)->Triangles.end();k++)
        {
            bool Found;

            Found = false;
            for(unsigned int j = 0;j < iVertices.size();j++)
            {
                if((iVertices[j] == k->V1) && (iTexCoords[j] == k->Tx1))
                {
                    Found = true;
                    break;
                }
            }

            if(!Found)
            {
                iVertices.push_back(k->V1);
                iNormals.push_back(k->V1);
                iTexCoords.push_back(k->Tx1);
            }

            Found = false;
            for(unsigned int j = 0;j < iVertices.size();j++)
            {
                if((iVertices[j] == k->V2) && (iTexCoords[j] == k->Tx2))
                {
                    Found = true;
                    break;
                }
            }

            if(!Found)
            {
                iVertices.push_back(k->V2);
                iNormals.push_back(k->V2);
                iTexCoords.push_back(k->Tx2);
            }

            Found = false;
            for(unsigned int j = 0;j < iVertices.size();j++)
            {
                if((iVertices[j] == k->V3) && (iTexCoords[j] == k->Tx3))
                {
                    Found = true;
                    break;
                }
            }

            if(!Found)
            {
                iVertices.push_back(k->V3);
                iNormals.push_back(k->V3);
                iTexCoords.push_back(k->Tx3);
            }
        }
    }

    for(unsigned int j = 0;j < iVertices.size();j++)
    {
        newModel->Vertices.push_back(File.Vertices[iVertices[j]]);
        newModel->Normals.push_back(File.smoothNormals[iNormals[j]]);
        newModel->TexCoords.push_back(File.TexCoords[iTexCoords[j]]);
    }

    for(auto i = File.MatGroups.begin();i != File.MatGroups.end();i++)
    {
        lrmStaticMesh::lrmMtlGroup *newMtlGroup = new lrmStaticMesh::lrmMtlGroup;
        newMtlGroup->Material = (*i)->Material;

        for(auto k = (*i)->Triangles.begin();k != (*i)->Triangles.end();k++)
        {
            for(unsigned int j=0;j < newModel->Vertices.size();j++)
            {
                if((iVertices[j] == k->V1) && (iTexCoords[j] == k->Tx1))
                {
                    newMtlGroup->IndexBuffer.push_back(j);
                    break;
                }
            }

            for(unsigned int j=0;j < newModel->Vertices.size();j++)
            {
                if((iVertices[j] == k->V2) && (iTexCoords[j] == k->Tx2))
                {
                    newMtlGroup->IndexBuffer.push_back(j);
                    break;
                }
            }

            for(unsigned int j=0;j < newModel->Vertices.size();j++)
            {
                if((iVertices[j] == k->V3) && (iTexCoords[j] == k->Tx3))
                {
                    newMtlGroup->IndexBuffer.push_back(j);
                    break;
                }
            }
        }

        newModel->MatGroups.push_back(newMtlGroup);
    }

    CalculateTBVectors(*newModel);

    *Target = newModel;
}

lrmWavefrontLoader::lrmWavefrontLoader(const std::string &materialPath,const std::string &staticMeshPath,const std::string &filename):MultiMaterial(nullptr)
{
    if(LoadWavefrontObj(materialPath,staticMeshPath,filename) != 0)
    {
        std::cerr << "Error! Couldn't load wavefront obj " << filename << std::endl;
    }
    else
    {
        SmoothNormals();
    }
}

lrmWavefrontLoader::~lrmWavefrontLoader()
{

}
