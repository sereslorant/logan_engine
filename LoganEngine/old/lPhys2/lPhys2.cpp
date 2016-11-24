
#include "lpCollisionDetector/lpDiscreteCollisionDetector.h"
#include "lpInteractionModel/lpSpringCollisionModel.h"
#include "lpInteractionModel/lpImpulseCollisionModel.h"
#include "lpIntegrator/lpRK4Integrator.h"
#include "lpSimulationWorld.h"

extern "C" {

liCollisionDetector *lpCreateDiscreteCollisionDetector()
{
	return new lpDiscreteCollisionDetector();
}

liInteractionModel *lpCreateSpringInteractionModel(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction,lmScalar stiffness,lmScalar damping_coefficient)
{
	return new lpSpringCollisionModel(grav_acceleration,air_density,coefficient_of_friction,stiffness,damping_coefficient);
}

liInteractionModel *lpCreateImpulseInteractionModel(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction)
{
	return new lpImpulseCollisionModel(grav_acceleration,air_density,coefficient_of_friction);
}

liIntegrator *lpCreateRK4Integrator(liInteractionModel *interaction_model,liCollisionDetector *collision_detector)
{
	return new lpRK4Integrator(interaction_model,collision_detector);
}

liSimulationWorld *lpCreateSimulationWorld(/*liIntegrator *integrator*/)
{
	liInteractionModel *InteractionModel   = new lpImpulseCollisionModel({0.0,0.0,0.0},0.0,1.0);

    liCollisionDetector *CollisionDetector = new lpDiscreteCollisionDetector();
    liIntegrator *Integrator = new lpRK4Integrator(InteractionModel,CollisionDetector);

	return new lpSimulationWorld(Integrator);
}

}
