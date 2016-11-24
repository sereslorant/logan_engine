
#include "lrsStaticMeshNode.h"

liStaticMesh *lrsStaticMeshNode::GetMesh()
{
    return Mesh;
}

const liStaticMesh *lrsStaticMeshNode::GetMesh() const
{
    return Mesh;
}

void lrsStaticMeshNode::Add(liRenderer *Renderer)
{
    Renderer->AddStaticMesh(*Mesh,*Material,*this,*this);
}

lrsStaticMeshNode::lrsStaticMeshNode(const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation,liStaticMesh *mesh,liMultiMaterial *material)
    :lrsSceneNode(position,scale,orientation,material),Mesh(mesh)
{

}

lrsStaticMeshNode::~lrsStaticMeshNode()
{

}
