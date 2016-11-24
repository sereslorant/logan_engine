#include <iostream>

using namespace std;

#include "lpRigidBody.h"
//Konstruktorok

lpRigidBody::lpRigidBody(bool ghost,lmScalar mass,lpRigidBody::liState *state,lpType type)
    :Type(type),Ghost(ghost),Mass(mass),InertiaTensor(lmMatrix3x3::NULLMAT),InvInertiaTensor(lmMatrix3x3::NULLMAT),State(state)
{

}

lpRigidBody::lpRigidBody(bool ghost,lmScalar mass,lpRigidBody::liState *state)
    :Type(LP_PARTICLE),Ghost(ghost),Mass(mass),InertiaTensor(lmMatrix3x3::NULLMAT),InvInertiaTensor(lmMatrix3x3::NULLMAT),State(state)
{

}

lpRigidBody::~lpRigidBody()
{
    delete State;
}

//Getter függvények
lpRigidBody::lpType lpRigidBody::GetType() const
{
    return Type;
}

lmScalar lpRigidBody::GetMass() const
{
    return Mass;
}

lmScalar lpRigidBody::GetInvMass() const
{
    if(Mass != 0.0)
    {return 1.0/Mass;}
    else
    {return 0.0;}
}

const lmMatrix3x3 &lpRigidBody::GetInertiaTensor() const
{
	return InertiaTensor;
}

const lmMatrix3x3 &lpRigidBody::GetInvInertiaTensor() const
{
	return InvInertiaTensor;
}

lpRigidBody::liState *lpRigidBody::GetState()
{
    return State;
};

const lpRigidBody::liState *lpRigidBody::GetState()const
{
    return State;
}

const lmVector3D &lpRigidBody::GetPosition()const
{
    return State->GetPosition();
}

const lmVector3D &lpRigidBody::GetMomentum()const
{
    return State->GetMomentum();
}

const lmVector3D &lpRigidBody::GetOrientation()const
{
    return State->GetOrientation();
}

const lmVector3D &lpRigidBody::GetAngularMomentum()const
{
    return State->GetAngularMomentum();
}
