#ifndef LI_PHYS_2_INTEGRATOR_H
#define LI_PHYS_2_INTEGRATOR_H

#include "../../lInterfaces/lPhys/liIntegrator.h"

#include "../liPhys2SimulationWorld.h"

/** \brief Az integrátor class absztrakt ősosztálya. Az integrátor felelős azért, hogy
 * numerikus integrálással a szimuláció elemeinek állapotát előre mozdítsa az időben.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liPhys2Integrator : public liIntegrator
{
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	liPhys2Integrator(const liPhys2Integrator &i) = delete;
	liPhys2Integrator &operator=(const liPhys2Integrator &i) = delete;

	virtual void SetSimulationWorld(liPhys2SimulationWorld *simulation_world) = 0;
	virtual liInteractionModel *GetInteractionModel() = 0;

    /** \brief Default konstruktor, nem csinál semmit.
     */
    liPhys2Integrator(){}

    /** \brief Virtuális destruktor, nem csinál semmit.
     */
    virtual ~liPhys2Integrator() override{}
};

#endif // LI_PHYS_2_INTEGRATOR_H
