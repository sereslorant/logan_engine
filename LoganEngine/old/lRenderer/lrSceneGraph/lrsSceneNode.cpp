
#include "lrsSceneNode.h"

#include "../../lInterfaces/lRenderer/liRenderer.h"

lmVector3D &lrsSceneNode::GetPosition()
{
    return Position;
}
lmVector3D &lrsSceneNode::GetScale()
{
    return Scale;
}
lmQuaternion &lrsSceneNode::GetOrientation()
{
    return Orientation;
}

const lmVector3D &lrsSceneNode::GetPosition() const
{
    return Position;
}
const lmVector3D &lrsSceneNode::GetScale() const
{
    return Scale;
}
const lmQuaternion &lrsSceneNode::GetOrientation() const
{
    return Orientation;
}

void lrsSceneNode::SetPosition(const lmVector3D &position)
{
    Position = position;
}
void lrsSceneNode::SetScale(const lmVector3D &scale)
{
    Scale = scale;
}
void lrsSceneNode::SetOrientation(const lmQuaternion &orientation)
{
    Orientation = orientation;
}

void lrsSceneNode::GetModelMatrix(lmMatrix4x4 &Target)
{
    lmTranslate4x4(Target,Position);
    lmRotate4x4(Target,Orientation);
    lmScale4x4(Target,Scale);
}

lrsSceneNode::lrsSceneNode(const lmVector3D &position,const lmVector3D &scale,const lmQuaternion &orientation,liMultiMaterial *material)
    :Position(position),Scale(scale),Orientation(orientation),Material(material)
{

}

lrsSceneNode::~lrsSceneNode()
{

}
