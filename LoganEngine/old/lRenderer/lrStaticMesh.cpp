
#include <iostream>

#include "lrStaticMesh.h"

using namespace std;


std::string &lrStaticMesh::lrMaterialGroup::GetMaterial()
{
    return Material;
}

const std::string &lrStaticMesh::lrMaterialGroup::GetMaterial() const
{
    return Material;
}

lrStaticMesh::lrMaterialGroup::lrMaterialGroup(const std::string &material)
    :Material(material)
{

}

lrStaticMesh::lrMaterialGroup::~lrMaterialGroup()
{

}

unsigned int lrStaticMesh::GetSize() const
{
    return MaterialGroups.size();
}

bool lrStaticMesh::Add(unsigned int i,liMaterialGroup *MatGroup)
{
    if(i >= MaterialGroups.size())
    {
        return false;
    }

    MaterialGroups[i] = MatGroup;
    return true;
}

liStaticMesh::liMaterialGroup *lrStaticMesh::operator[] (unsigned int i)
{
    if(i >= MaterialGroups.size())
    {
        return nullptr;
    }

    return MaterialGroups[i];
}

const liStaticMesh::liMaterialGroup *lrStaticMesh::operator[] (unsigned int i) const
{
    if(i >= MaterialGroups.size())
    {
        return nullptr;
    }

    return MaterialGroups[i];
}

lrStaticMesh::lrStaticMesh(unsigned int Size):MaterialGroups(Size)
{

}

lrStaticMesh::~lrStaticMesh()
{
    for(liStaticMesh::liMaterialGroup * I :  MaterialGroups)
    {
        delete I;
    }
}

