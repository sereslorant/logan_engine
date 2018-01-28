
#include "lrGLSkeletalMeshView.h"

unsigned int lrGLSkeletalMeshView::NumMtlGroups()
{
	if(SkeletalMeshResource != nullptr)
	{
		return SkeletalMeshResource->MaterialGroups.size();
	}
	else
	{
		return 0;
	}
}

lrGLSkeletalMaterialGroupView lrGLSkeletalMeshView::GetMaterialGroup(unsigned int id)
{
	if(SkeletalMeshResource != nullptr && id < SkeletalMeshResource->MaterialGroups.size())
	{
		return lrGLSkeletalMaterialGroupView(&SkeletalMeshResource->VertexData,&SkeletalMeshResource->BoneData,&SkeletalMeshResource->MaterialGroups[id]);
	}
	else
	{
		return lrGLSkeletalMaterialGroupView();
	}
}
