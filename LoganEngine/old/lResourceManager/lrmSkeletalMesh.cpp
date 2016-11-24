
#include "lrmSkeletalMesh.h"

using namespace std;

lrmSkeletalMesh::lrmWeight::lrmWeight()
{
    for(unsigned int i=0;i < MAX_WEIGHT_COUNT;i++)
    {
        Weight[i] = 0.0;
    }
}

float &lrmSkeletalMesh::lrmWeight::operator[](unsigned int i)
{
    return Weight[i];
}

const float &lrmSkeletalMesh::lrmWeight::operator[](unsigned int i) const
{
    return Weight[i];
}

lrmSkeletalMesh::lrmBoneId::lrmBoneId()
{
    for(unsigned int i=0;i < MAX_WEIGHT_COUNT;i++)
    {
        Id[i] = -1;
    }
}

int &lrmSkeletalMesh::lrmBoneId::operator[](unsigned int i)
{
    return Id[i];
}

const int &lrmSkeletalMesh::lrmBoneId::operator[](unsigned int i) const
{
    return Id[i];
}

void lrmSkeletalMesh::Print(ostream &out)
{
    out << "MaxWeightCount " << MAX_WEIGHT_COUNT << "\n\n";

    out << "BoneId\nSize:\n" << BoneId.size() << "\n";
    for(unsigned int i=0;i < BoneId.size();i++)
    {
        for(unsigned int j=0;j < MAX_WEIGHT_COUNT;j++)
        {
            out <<j<<":"<< BoneId[i][j] << " ";
        }
    }
    out << "\n\n";

    out << "Vrt\nSize:\n" << Vertices.size() << "\n";
    for(unsigned int i=0;i < Vertices.size();i++)
    {
        /*for(unsigned int j=0;j < MAX_WEIGHT_COUNT;j++)
        {
           if(BoneId[i][j] != -1)
           {*/
               out << Vertices[i]/*[j]*/[0] << " " << Vertices[i]/*[j]*/[1] << " " << Vertices[i]/*[j]*/[2] << " ";
        /*   }
        }*/
    }
    out << "\n\n";

    out << "Tex\nSize:\n" << TexCoords.size() << "\n";
    for(unsigned int i=0;i < TexCoords.size();i++)
    {
        out << TexCoords[i][0] << " " << TexCoords[i][1] << " ";
    }
    out << "\n\n";


    out << "Nrm\nSize:\n" << Normals.size() << "\n";
    for(unsigned int i=0;i < Normals.size();i++)
    {
        /*for(unsigned int j=0;j < MAX_WEIGHT_COUNT;j++)
        {
            if(BoneId[i][j] != -1)
            {*/
                out << Normals[i]/*[j]*/[0] << " " << Normals[i]/*[j]*/[1] << " " << Normals[i]/*[j]*/[2] << " ";
        /*    }
        }*/
    }
    out << "\n\n";

    out << "Tan\nSize:\n" << Tangents.size() << "\n";
    for(unsigned int i=0;i < Tangents.size();i++)
    {
        /*for(unsigned int j=0;j < MAX_WEIGHT_COUNT;j++)
        {
            if(BoneId[i][j] != -1)
            {*/
                out << Tangents[i]/*[j]*/[0] << " " << Tangents[i]/*[j]*/[1] << " " << Tangents[i]/*[j]*/[2] << " ";
        /*    }
        }*/
    }
    out << "\n\n";

    out << "Bnr\nSize:\n" << Bitangents.size() << "\n";
    for(unsigned int i=0;i < Bitangents.size();i++)
    {
        /*for(unsigned int j=0;j < MAX_WEIGHT_COUNT;j++)
        {
            if(BoneId[i][j] != -1)
            {*/
                out << Bitangents[i]/*[j]*/[0] << " " << Bitangents[i]/*[j]*/[1] << " " << Bitangents[i]/*[j]*/[2] << " ";
        /*    }
        }*/
    }
    out << "\n\n";

    out << "Weights\nSize:\n" << Weights.size() << "\n";
    for(unsigned int i=0;i < Weights.size();i++)
    {
        for(unsigned int j=0;j < MAX_WEIGHT_COUNT;j++)
        {
            if(BoneId[i][j] != -1)
            {
               out << Weights[i][j] << " ";
            }
        }
    }
    out << "\n\n";

    out << "Ib\nSize:\n" << IndexBuffer.size() << "\n";
    for(auto i=IndexBuffer.begin();i != IndexBuffer.end();i++)
    {
        out << *i << " ";
    }
    out << "\n\n";
}

lrmSkeletalMesh::lrmSkeletalMesh()
{

}

lrmSkeletalMesh::~lrmSkeletalMesh()
{

}
