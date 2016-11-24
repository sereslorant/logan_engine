
#include "lrmStaticMesh.h"

using namespace std;

lrmStaticMesh::lrmStaticMesh()
{

}

lrmStaticMesh::~lrmStaticMesh()
{
    for(auto I : MatGroups)
    {
        delete I;
    }
}

ostream &operator<<(ostream &out,const lrmStaticMesh &StaticMesh)
{
    out << "Vrt\n";
    out << "Size: "<< StaticMesh.Vertices.size() << "\n";

    for(const lmVector3D &i : StaticMesh.Vertices)
    {
        out << i[0] << " " << i[1] << " " << i[2] << " ";
    }

    out << "\n\n";

    out << "Tex\n";
    out << "Size: "<< StaticMesh.TexCoords.size() << "\n";

    for(const lmVector2D &i : StaticMesh.TexCoords)
    {
        out << i[0] << " " << i[1] << " ";
    }

    out << "\n\n";

    out << "Nrm\n";
    out << "Size: "<< StaticMesh.Normals.size() << "\n";

    for(const lmVector3D &i : StaticMesh.Normals)
    {
        out << i[0] << " " << i[1] << " " << i[2] << " ";
    }

    out << "Tan\n";
    out << "Size: "<< StaticMesh.Tangents.size() << "\n";

    for(const lmVector3D &i : StaticMesh.Tangents)
    {
        out << i[0] << " " << i[1] << " " << i[2] << " ";
    }

    out << "Bin\n";
    out << "Size: "<< StaticMesh.Bitangents.size() << "\n";

    for(const lmVector3D &i : StaticMesh.Bitangents)
    {
        out << i[0] << " " << i[1] << " " << i[2] << " ";
    }

    out << "\n\n";

    for(auto i = StaticMesh.MatGroups.begin();i != StaticMesh.MatGroups.end();i++)
    {
        out << "MtlGroup\n";
        out << "MtlName " << (*i)->Material << "\n";
        out << "Size " << (*i)->IndexBuffer.size() << "\n";

        for(auto j = (*i)->IndexBuffer.begin();j != (*i)->IndexBuffer.end();j++)
        {
            out << *j << " ";
        }

        out << "\n\n";
    }

    return out;
}

std::istream &operator<<(std::istream &in,lrmStaticMesh &StaticMesh)
{
    string Tmp;

    in >> Tmp;

    if(Tmp == "Vrt")
    {
        in >> Tmp;
        unsigned int Size;
        in >> Size;

        StaticMesh.Vertices.resize(Size);
        for(unsigned int i=0;i < StaticMesh.Vertices.size();i++)
        {
            in >> StaticMesh.Vertices[i][0];
            in >> StaticMesh.Vertices[i][1];
            in >> StaticMesh.Vertices[i][2];
        }
    }
    else if(Tmp == "Tex")
    {
        in >> Tmp;
        unsigned int Size;
        in >> Size;

        StaticMesh.TexCoords.resize(Size);
        for(unsigned int i=0;i < StaticMesh.TexCoords.size();i++)
        {
            in >> StaticMesh.TexCoords[i][0];
            in >> StaticMesh.TexCoords[i][1];
        }
    }
    else if(Tmp == "Nrm")
    {
        in >> Tmp;
        unsigned int Size;
        in >> Size;

        StaticMesh.Normals.resize(Size);
        for(unsigned int i=0;i < StaticMesh.Normals.size();i++)
        {
            in >> StaticMesh.Normals[i][0];
            in >> StaticMesh.Normals[i][1];
            in >> StaticMesh.Normals[i][2];
        }
    }
    else if(Tmp == "Tan")
    {
        in >> Tmp;
        unsigned int Size;
        in >> Size;

        StaticMesh.Tangents.resize(Size);
        for(unsigned int i=0;i < StaticMesh.Tangents.size();i++)
        {
            in >> StaticMesh.Tangents[i][0];
            in >> StaticMesh.Tangents[i][1];
            in >> StaticMesh.Tangents[i][2];
        }
    }
    else if(Tmp == "Bin")
    {
        in >> Tmp;
        unsigned int Size;
        in >> Size;

        StaticMesh.Bitangents.resize(Size);
        for(unsigned int i=0;i < StaticMesh.Bitangents.size();i++)
        {
            in >> StaticMesh.Bitangents[i][0];
            in >> StaticMesh.Bitangents[i][1];
            in >> StaticMesh.Bitangents[i][2];
        }
    }
    else if(Tmp == "MtlGroup")
    {
        lrmStaticMesh::lrmMtlGroup *newMtlGroup = new lrmStaticMesh::lrmMtlGroup;
        in >> Tmp;
        in >> newMtlGroup->Material;
        in >> Tmp;
        unsigned int Size;
        in >> Size;

        newMtlGroup->IndexBuffer.resize(Size);
        for(unsigned int i=0;i < newMtlGroup->IndexBuffer.size();i++)
        {
            in >> newMtlGroup->IndexBuffer[i];
        }
    }

    return in;
}
