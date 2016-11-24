
#include "lpSphere.h"

//Konstruktor
lpSphere::lpSphere(bool ghost,lmScalar mass,liRigidBody::liState *state,lmScalar radius)
    :lpRigidBody(ghost,mass,state,LP_SPHERE),Radius(radius)
{
    InertiaTensor[0][0] = 2.0/5.0 * Mass * Radius * Radius; InertiaTensor[1][0] = 0.0;                              InertiaTensor[2][0] = 0.0;
    InertiaTensor[0][1] = 0.0;                              InertiaTensor[1][1] = 2.0/5.0 * Mass * Radius * Radius; InertiaTensor[2][1] = 0.0;
    InertiaTensor[0][2] = 0.0;                              InertiaTensor[1][2] = 0.0;                              InertiaTensor[2][2] = 2.0/5.0 * Mass * Radius * Radius;

    if(Mass != 0.0)
    {
        InertiaTensor.Invert(InvInertiaTensor);
    }
}

//Getter függvények
lmScalar lpSphere::GetRadius() const
{
    return Radius;
}
