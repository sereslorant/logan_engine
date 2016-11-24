
#include "lrsSkeletalMeshNode.h"

liCharacter *lrsSkeletalMeshNode::GetCharacter()
{
    return Character;
}

const liCharacter *lrsSkeletalMeshNode::GetCharacter() const
{
    return Character;
}

liSkeleton &lrsSkeletalMeshNode::GetSkeleton()
{
    return Skeleton;
}

const liSkeleton &lrsSkeletalMeshNode::GetSkeleton() const
{
    return Skeleton;
}

void lrsSkeletalMeshNode::SetSkeleton(const liSkeleton &skeleton)
{
    Skeleton.Copy(skeleton);
}

void lrsSkeletalMeshNode::Add(liRenderer *Renderer)
{
    Renderer->AddSkeletalMesh(*Character,Skeleton,*Material,*this,*this);
}

lrsSkeletalMeshNode::lrsSkeletalMeshNode(const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation, liCharacter *character,liMultiMaterial *material,liSkeleton &skeleton)
    :lrsSceneNode(position,scale,orientation,material),Character(character)
{
	Skeleton.Copy(skeleton);
}

lrsSkeletalMeshNode::~lrsSkeletalMeshNode()
{

}
