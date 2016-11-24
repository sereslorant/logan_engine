#ifndef LP_RK4_INTEGRATOR
#define LP_RK4_INTEGRATOR

#include "../../lInterfaces/lPhys/liIntegrator.h"
#include "../../lInterfaces/lPhys/liInteractionModel.h"
#include "../../lInterfaces/lPhys/liCollisionDetector.h"

#include "../lpRigidBody/lpRigidBody.h"

#include <list>

#include "liPhys2Integrator.h"

/** \brief Runge Kutta 4 módszert használó integrátor
 *
 * A függvény a Runge-Kutta módszerrel 4-szer mintát vesz a test pillanatnyi állapotát megadó
 * változók deriváltjából.
 *
 * Az RK4 módszer lényege:
 * legyen y' = f(x,y) összefüggésünk y(x0) = y0 kezdeti feltételekkel.
 *
 * Ekkor az y'-ból vett 4 minta:
 * k[0] = f(x0,y0)
 * k[1] = f(x0 + dx/2,y0 + dx/2 * k[0])
 * k[2] = f(x0 + dx/2,y0 + dx/2 * k[1])
 * k[3] = f(x0 + dx,y0 + dx * k[2])
 *
 * Ezután az integrálás során vesszük a deriváltakból vett minták súlyozott átlagát:
 * y[i+1] = y[i] + dx * 1/6 *(k[0] + 2k[1] + 2k[2] + k[3])
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpRK4Integrator : public liPhys2Integrator
{
private:

    /** \brief A test állapotából vett mintákat tároló struktúra. Az RK4 módszer során 4x veszünk mintát
     * a test állapotából és a deriváltjából. Ezeket az integrálás során meg kell jegyezni.
     *
     */
    struct lpRKState
    {
        liRigidBody &Body;                      /**< A testre mutató referencia */
        lpRigidBody::lpState State[4];          /**< A 4 db lineáris közelítése a test állapotának */
        lpRigidBody::lpDerivative Derivative[4];/**< A 4 db minta az állapotok deriváltjaiból */

        std::list<liInteractionModel::liConstraint *> Constraints;

        void UpdateConstraints(int iteration)
        {
        	for(auto Constraint : Constraints)
			{
				if(Constraint->GetBody_i() == &Body)
				{
					Constraint->SetState_i(&(State[iteration]));
					Constraint->SetDerivative_i(&(Derivative[iteration]));
				}
				else if(Constraint->GetBody_j() == &Body)
				{
					Constraint->SetState_j(&(State[iteration]));
					Constraint->SetDerivative_j(&(Derivative[iteration]));
				}
			}
        }

        /** \brief Konstruktor
         *
         * \param body lpRigidBody &: a test, amelynek a mintáit eltároljuk.
         *
         */
        lpRKState(liRigidBody &body):Body(body){}
    };

    liInteractionModel  *InteractionModel; /**< Ez az objektum dönti el, hogy a testek között milyen erők lépnek fel. */
    liCollisionDetector *CollisionDetector; /**< Ez az objektum állapítja meg, hogy két test ütközik-e. */

    liPhys2SimulationWorld	*SimulationWorld = nullptr;

    std::list<lpRKState *> Bodies; /**< Itt tároljuk a testek vett mintáit. */

    /** \brief A paraméterként kapott derivált alapján lineárisan közelítjük a test következő állapotát.
     *
     * \param Bi lpRigidBody &:                         Az i-edik test (akinek az állapotát lineárisan közelítjük)
     * \param State_t lpRigidBody::lpState &:           A test t időbeli állapota az ez által a referencia által mutatott helyre kerül.
     * \param dt lmScalar:                              A szimuláció időbeli lépésköze
     * \param Derivative_t lpRigidBody::lpDerivative &: A test t időbeli állapotának deriváltja
     * \param Derivative_i lpRigidBody::lpDerivative *: A test állapotának előző lépésben számolt deriváltja. Az első mintavételezésnél nullptr.
     * \return void
     *
     */
    void Sample(liRigidBody &Bi,lpRigidBody::lpState &State_t,lmScalar dt,lpRigidBody::lpDerivative &Derivative_t,lpRigidBody::lpDerivative *Derivative_i = nullptr);

    /** \brief A paraméterként kapott derivált alapján lineárisan közelítjük a test következő állapotát.
     *
     * \param Bi lpRigidBody &:                     Az i-edik test (akinek az állapotát numerikusan integráljuk)
     * \param k[] const lpRigidBody::lpDerivative:  A mintavételezés során vett 4 db minta.
     * \param dt lmScalar:                          A szimuláció időbeli lépésköze
     * \return void
     *
     */
    void Integrate(liRigidBody &Bi,const lpRigidBody::lpDerivative k[],lmScalar dt);
public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
    lpRK4Integrator(const lpRK4Integrator &i) = delete;
    lpRK4Integrator &operator=(const lpRK4Integrator &i) = delete;

    virtual void SetSimulationWorld(liPhys2SimulationWorld *simulation_world) override
    {
    	SimulationWorld = simulation_world;
    }

	virtual liInteractionModel *GetInteractionModel() override
	{
		return InteractionModel;
	}

	virtual void AddDistanceConstraint(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance) override
	{
		bool BiFound = false;
		lpRKState *Bi_RK = nullptr;

		bool BjFound = false;
		lpRKState *Bj_RK = nullptr;

		for(lpRKState *I : Bodies)
		{
			if(&(I->Body) == bi)
			{
				Bi_RK = I;
				BiFound = true;
			}

			if(&(I->Body) == bj)
			{
				Bj_RK = I;
				BjFound = true;
			}

			if(BiFound && BjFound)
			{
				break;
			}
		}

		if(!BiFound || !BjFound)
		{
			return;
		}

		liInteractionModel::liConstraint *DistConstraint = InteractionModel->AddDistConstraint(bi,bj,&(Bi_RK->State[0]),&(Bj_RK->State[0]),&(Bi_RK->Derivative[0]),&(Bj_RK->Derivative[0]),pi,pj,distance);

		Bi_RK->Constraints.push_back(DistConstraint);
		Bj_RK->Constraints.push_back(DistConstraint);
	}

    /** \brief
     *
     * \todo Be kell üzemelni, hogy ne csak a k[0]-ra hasson a rugóerő.
     *
     * \param bi lpRigidBody*
     * \param bj lpRigidBody*
     * \param pi const lmVector3D&
     * \param pj const lmVector3D&
     * \param distance lmScalar
     * \param stiffness lmScalar
     * \param damping_coefficient lmScalar
     * \return virtual void
     *
     */
	virtual void AddSpring(liRigidBody *bi,liRigidBody *bj,const lmVector3D &pi,const lmVector3D &pj,lmScalar distance,lmScalar stiffness,lmScalar damping_coefficient) override
	{
		bool BiFound = false;
		lpRKState *Bi_RK = nullptr;

		bool BjFound = false;
		lpRKState *Bj_RK = nullptr;

		for(lpRKState *I : Bodies)
		{
			if(&(I->Body) == bi)
			{
				Bi_RK = I;
				BiFound = true;
			}

			if(&(I->Body) == bj)
			{
				Bj_RK = I;
				BjFound = true;
			}

			if(BiFound && BjFound)
			{
				break;
			}
		}

		if(!BiFound || !BjFound)
		{
			return;
		}

		liInteractionModel::liConstraint *DistConstraint = InteractionModel->AddSpring(bi,bj,&(Bi_RK->State[0]),&(Bj_RK->State[0]),&(Bi_RK->Derivative[0]),&(Bj_RK->Derivative[0]),pi,pj,distance,stiffness,damping_coefficient);

		Bi_RK->Constraints.push_back(DistConstraint);
		Bj_RK->Constraints.push_back(DistConstraint);
	}

    /** \brief Konstruktor, paraméterként átveszi az ütközésmodellt és az ütközésellenőrzőt.
     *
     * \param interaction_model lpInteractionModel *:    Az ütközésmodell - dinamikusan allokált, az integrátor szabadítja fel.
     * \param collision_detector liCollisionDetector *:  Az ütközésellenőrző - dinamikusan allokált, az ütközésellenőrző szabadítja fel.
     *
     */
    lpRK4Integrator(liInteractionModel *interaction_model,liCollisionDetector *collision_detector)
        :InteractionModel(interaction_model),CollisionDetector(collision_detector)
    {
    	//Üres függvénytörzs
    }

    /** \brief Destruktor, feszabadítja az ütközésmodellt, az ütközésellenőrzőt és a testek mintáit tároló atatstruktúrákat.
     */
    virtual ~lpRK4Integrator() override
    {
        delete InteractionModel;
        delete CollisionDetector;

        for(auto &I : Bodies)
        {
            delete I;
        }
    }

    /** \brief Új test hozzáadása a szimulációhoz. Erre azért van szükség, mert minden modellhez kell allokálni egy lpRKState struktúrát.
     *
     * \param Body lpRigidBody &: a hozzáadandó test
     * \return void
     *
     */
    virtual void AddBody(liRigidBody &Body) override;

    /** \brief A szimuláció előreléptetése RK4 módszerrel. Az algoritmusról bővebben a függvény
     *  kommentjeiben / a class rövid ismertetőjében találhat információkat.
     *
     * \param dt lmScalar: az időbeli lépésköz
     * \return void
     *
     */
    virtual void Step(lmScalar dt) override;
};

#endif
