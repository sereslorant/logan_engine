#ifndef L_PHYS_H
#define L_PHYS_H

#include "../../lMath/lMath.h"

#include "liCollisionDetector.h"
#include "liInteractionModel.h"
#include "liIntegrator.h"
#include "liSimulationWorld.h"
/*
typedef liCollisionDetector* (*lfptrCreateCollisionDetector)();
typedef liInteractionModel* (*lfptrCreateInteractionModel)(const lmVector3D &grav_acceleration,lmScalar air_density,lmScalar coefficient_of_friction);
typedef liIntegrator* (*lfptrCreateIntegrator)(liInteractionModel *interaction_model,liCollisionDetector *collision_detector);
*/
typedef liSimulationWorld* (*lfptrCreateSimulationWorld)(/*liIntegrator *integrator*/);

#endif // L_PHYS_H
