
#include "lpPlane.h"

//Konstruktor
lpPlane::lpPlane(bool ghost,lmScalar mass,liRigidBody::liState *state,const lmVector3D &normal)
    :lpRigidBody(ghost,mass,state,LP_PLANE),Normal(normal)
{

}
//Getter függvények
const lmVector3D &lpPlane::GetNormal() const
{
    return Normal;
}
