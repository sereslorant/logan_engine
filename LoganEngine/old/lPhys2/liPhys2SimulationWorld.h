#ifndef LI_PHYS_2_SIMULATION_WORLD_H
#define LI_PHYS_2_SIMULATION_WORLD_H

#include "../lInterfaces/lPhys/liSimulationWorld.h"

class liPhys2SimulationWorld : public liSimulationWorld
{
public:
	virtual void RemoveBody(const liRigidBody *Body) = 0;

	liPhys2SimulationWorld(){}
	virtual ~liPhys2SimulationWorld() override {}
};

#endif // LI_PHYS_2_SIMULATION_WORLD_H
