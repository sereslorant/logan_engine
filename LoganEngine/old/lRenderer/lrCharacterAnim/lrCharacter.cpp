
#include "lrCharacter.h"

using namespace std;

bool lrCharacter::Add(unsigned int i,liSkeletalMesh *newMesh)
{
    if(i >= Meshes.size())
    {
        return false;
    }

    Meshes[i] = newMesh;
    return true;
}

unsigned int lrCharacter::Size()
{
    return Meshes.size();
}

liSkeletalMesh *lrCharacter::operator[](unsigned int i)
{
    if(i >= Meshes.size())
    {
        return nullptr;
    }

    return Meshes[i];
}

const liSkeletalMesh *lrCharacter::operator[](unsigned int i) const
{
    if(i >= Meshes.size())
    {
        return nullptr;
    }

    return Meshes[i];
}

lrCharacter::lrCharacter(const liSkeleton &bind_pose_skeleton,unsigned int Size)
	:Meshes(Size)
{
	BindPoseSkeleton.Copy(bind_pose_skeleton);
}

lrCharacter::~lrCharacter()
{

}
