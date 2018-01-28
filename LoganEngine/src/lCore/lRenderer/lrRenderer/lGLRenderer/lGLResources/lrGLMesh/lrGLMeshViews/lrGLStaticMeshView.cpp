
#include "lrGLStaticMeshView.h"

unsigned int lrGLStaticMeshView::NumMtlGroups()
{
	if(StaticMeshResource != nullptr)
	{
		return StaticMeshResource->MaterialGroups.size();
	}
	else
	{
		return 0;
	}
}

lrGLStaticMaterialGroupView lrGLStaticMeshView::GetMaterialGroup(unsigned int id)
{
	if(StaticMeshResource != nullptr && id < StaticMeshResource->MaterialGroups.size())
	{
		return lrGLStaticMaterialGroupView(&StaticMeshResource->VertexData,&StaticMeshResource->MaterialGroups[id]);
	}
	else
	{
		return lrGLStaticMaterialGroupView();
	}
}
