
#include "lrSkeletalMesh.h"

using namespace std;

string &lrSkeletalMesh::GetMaterial()
{
    return Material;
}

const string &lrSkeletalMesh::GetMaterial() const
{
    return Material;
}

lrSkeletalMesh::lrSkeletalMesh(const string &material):Material(material)
{

}

lrSkeletalMesh::~lrSkeletalMesh()
{

}

