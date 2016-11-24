#include <iostream>

using namespace std;

#include "lpRigidBody/lpRigidBody.h"
#include "lpRigidBody/lpSphere.h"
#include "lpRigidBody/lpPlane.h"
#include "lpRigidBody/lpMesh.h"
#include "lpRigidBody/lpBox.h"

#include "lpSimulationWorld.h"

lpSimulationWorld::lpSimulationWorld(liIntegrator *integrator)
{
	Integrator = (liPhys2Integrator *)integrator;
	Integrator->SetSimulationWorld(this);
}

lpSimulationWorld::~lpSimulationWorld()
{
    delete Integrator;

	//A világ megszűnésekor törli az összes testet.
    for(list<lpRigidBody *>::iterator i=Bodies.begin();i != Bodies.end();i++)
    {delete *i;}
}

liRigidBody *lpSimulationWorld::AddParticle(bool ghost,lmScalar mass,lpRigidBody::liState *state)
{
    //Létrehoz az adott paraméterekkel egy új pontszerű testet, és hozzáadja a világunkhoz.
    lpRigidBody *newBody = new lpRigidBody(ghost,mass,state);
    Bodies.push_back(newBody);

    Integrator->AddBody(*newBody);
    //Visszatér a testre mutató pointerrel, hogy a keretprogram bele tudjon nyúlni, ha szükséges.
    return newBody;
}

liRigidBody *lpSimulationWorld::AddSphere(bool ghost,lmScalar mass,lpRigidBody::liState *state,lmScalar radius)
{
    //Létrehoz az adott paraméterekkel egy új gömböt, és hozzáadja a világunkhoz.
    lpSphere *newBody = new lpSphere(ghost,mass,state,radius);
    Bodies.push_back(newBody);

    Integrator->AddBody(*newBody);
    //Visszatér a testre mutató pointerrel, hogy a keretprogram bele tudjon nyúlni, ha szükséges.
    return newBody;
}

liRigidBody *lpSimulationWorld::AddPlane(bool ghost,lmScalar mass,lpRigidBody::liState *state,const lmVector3D &normal)
{
	//Létrehoz az adott paraméterekkel egy új síkot, és hozzáadja a világunkhoz.
    lpPlane *newBody = new lpPlane(ghost,mass,state,normal);
    Bodies.push_back(newBody);

    Integrator->AddBody(*newBody);
    //Visszatér a testre mutató pointerrel, hogy a keretprogram bele tudjon nyúlni, ha szükséges.
    return newBody;
}

liRigidBody *lpSimulationWorld::AddMesh(bool ghost,lmScalar mass,lpRigidBody::liState *state,const liPhMesh *PhMesh,bool bounding_sphere,bool quadtree,unsigned int bv_elements_size)
{
	//Létrehoz az adott paraméterekkel egy új síkot, és hozzáadja a világunkhoz.
    lpMesh *newBody = new lpMesh(ghost,mass,state,PhMesh);

    if(bv_elements_size != 0)
	{
		if(bounding_sphere)
		{
			newBody->GenerateBoundingVolumeHierarchy(bv_elements_size,liBoundingVolume::LP_SPHERE_TREE,false);
		}
		else
		{
			newBody->GenerateBoundingVolumeHierarchy(bv_elements_size,liBoundingVolume::LP_AABB_TREE,quadtree);
		}
	}

    Bodies.push_back(newBody);

    Integrator->AddBody(*newBody);
    //Visszatér a testre mutató pointerrel, hogy a keretprogram bele tudjon nyúlni, ha szükséges.
    return newBody;
}

liRigidBody *lpSimulationWorld::AddBox(bool ghost,lmScalar mass,lpRigidBody::liState *state,lmScalar width,lmScalar height,lmScalar depth)
{
	//Létrehoz az adott paraméterekkel egy új síkot, és hozzáadja a világunkhoz.
    lpBox *newBody = new lpBox(ghost,mass,state,width,height,depth);
    Bodies.push_back(newBody);

    Integrator->AddBody(*newBody);
    //Visszatér a testre mutató pointerrel, hogy a keretprogram bele tudjon nyúlni, ha szükséges.
    return newBody;
}

void lpSimulationWorld::Simulate(lmScalar dt)
{
    Integrator->Step(dt);
}
