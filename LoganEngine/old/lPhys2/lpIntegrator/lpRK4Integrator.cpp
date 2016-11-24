#include <iostream>

#include "lpRK4Integrator.h"
#include "../lpSimulationWorld.h"

void lpRK4Integrator::Sample(liRigidBody &Bi,lpRigidBody::lpState &State_t,lmScalar dt,lpRigidBody::lpDerivative &Derivative_t,lpRigidBody::lpDerivative *Derivative_i)
{
    const lpRigidBody::liState *State_i = Bi.GetState();

    State_t.Movable		= State_i->IsMovable();
    State_t.Rotatable	= State_i->IsRotatable();

    //Ha ez az első mintavételezés, akkor nem kapunk deriváltra mutató pointert, defaultból nullptr-re van állítva.
    if(Derivative_i == nullptr)
    {
        State_t.Position = State_i->GetPosition();
        State_t.Momentum = State_i->GetMomentum();

        State_t.Orientation     = State_i->GetOrientation();
        State_t.AngularMomentum = State_i->GetAngularMomentum();
    }
    else
    {
		State_t.Position    = State_i->GetPosition() + Bi.GetInvMass() * Derivative_i->Momentum * dt;
		if(State_t.IsMovable())
		{
			State_t.Momentum = State_i->GetMomentum() + Derivative_i->NetForce * dt;
		}
		else
		{
			State_t.Momentum = State_i->GetMomentum();
		}

		lmMatrix3x3 biRotation(lmMatrix3x3::IDENTITY);
		lmRotate3x3(biRotation,State_i->GetOrientationQuaternion());

		lmMatrix3x3 biInvInertiaTensor_t = biRotation * Bi.GetInvInertiaTensor() * biRotation.Transpose();

		State_t.Orientation = State_i->GetOrientation() + biInvInertiaTensor_t * Derivative_i->AngularMomentum * dt;
		if(State_t.IsRotatable())
		{
			State_t.AngularMomentum = State_i->GetAngularMomentum() + Derivative_i->NetTorque * dt;
		}
		else
		{
			State_t.AngularMomentum = State_i->GetAngularMomentum();
		}
    }

    Derivative_t.Momentum   = State_t.Momentum;
    Derivative_t.NetForce   = {0.0,0.0,0.0};

    Derivative_t.AngularMomentum    = State_t.AngularMomentum;
    Derivative_t.NetTorque          = {0.0,0.0,0.0};
}

void lpRK4Integrator::Integrate(liRigidBody &Bi,const lpRigidBody::lpDerivative k[],lmScalar dt)
{
    lpRigidBody::liState *State_i = Bi.GetState();

    //A Runge-Kutta módszerrel vesszük a deriváltakból vett minták súlyozott átlagát, és ez alapján előremozdítjuk a testet az idő szerint:
    //y' = 1/6 * (k[0] + 2*k[1] + 2*k[2] + k[3])
    //y[n+1] = y[n] + dx * y'

	State_i->IncPosition(dt * Bi.GetInvMass() * (1.0/6.0)*((k[0].Momentum) + 2.0 * (k[1].Momentum) + 2.0 * (k[2].Momentum) + (k[3].Momentum))); //A pozíciót a sebesség alapján mozdítjuk előre
	if(State_i->IsMovable())
	{
		State_i->IncMomentum(dt * (1.0/6.0)*((k[0].NetForce) + 2.0 * (k[1].NetForce) + 2.0 * (k[2].NetForce) + (k[3].NetForce))); //Az impulzust az eredő erő alapján mozdítjuk előre
	}

	lmMatrix3x3 biRotation(lmMatrix3x3::IDENTITY);
	lmRotate3x3(biRotation,State_i->GetOrientationQuaternion());
	lmMatrix3x3 biInvInertiaTensor_t = biRotation * Bi.GetInvInertiaTensor() * biRotation.Transpose();
/*
	std::cout << biInvInertiaTensor_t[0][0] << ";" << biInvInertiaTensor_t[1][0] << ";" << biInvInertiaTensor_t[2][0] << std::endl;
	std::cout << biInvInertiaTensor_t[0][1] << ";" << biInvInertiaTensor_t[1][1] << ";" << biInvInertiaTensor_t[2][1] << std::endl;
	std::cout << biInvInertiaTensor_t[0][2] << ";" << biInvInertiaTensor_t[1][2] << ";" << biInvInertiaTensor_t[2][2] << std::endl;
*/
	State_i->IncOrientation(dt * (biInvInertiaTensor_t * ((1.0/6.0)*((k[0].AngularMomentum) + 2.0 * (k[1].AngularMomentum) + 2.0 * (k[2].AngularMomentum) + (k[3].AngularMomentum))))); //A szögelfordulást a szögsebesség alapján mozdítjuk előre
	if(State_i->IsRotatable())
	{
		State_i->IncAngularMomentum(dt * (1.0/6.0)*((k[0].NetTorque) + 2.0 * (k[1].NetTorque) + 2.0 * (k[2].NetTorque) + (k[3].NetTorque))); //Az impulzusmomentumot a forgatónyomaték alapján mozdítjuk előre
	}
/*
	std::cout << std::endl;
	std::cout << k[0].NetTorque[0] << ";" << k[0].NetTorque[1] << ";" << k[0].NetTorque[2] << std::endl;
	std::cout << k[1].NetTorque[0] << ";" << k[1].NetTorque[1] << ";" << k[1].NetTorque[2] << std::endl;
	std::cout << k[2].NetTorque[0] << ";" << k[2].NetTorque[1] << ";" << k[2].NetTorque[2] << std::endl;
	std::cout << k[3].NetTorque[0] << ";" << k[3].NetTorque[1] << ";" << k[3].NetTorque[2] << std::endl;
	std::cout << std::endl;
*/
    //Debugolási célokra - ha kíváncsiak vagyunk a test jelenlegi állapotára
    /*
    std::cout   << State_i.GetPosition().X << ";" << State_i.GetPosition().Y << ";" << State_i.GetPosition().Z << std::endl
                << State_i.GetMomentum().X << ";" << State_i.GetMomentum().Y << ";" << State_i.GetMomentum().Z << std::endl
                << std::endl
                << State_i.GetOrientation().X << ";" << State_i.GetOrientation().Y << ";" << State_i.GetOrientation().Z << std::endl
                << State_i.GetAngularMomentum().X << ";" << State_i.GetAngularMomentum().Y << ";" << State_i.GetAngularMomentum().Z << std::endl
                << std::endl;
    //          */
}

void lpRK4Integrator::AddBody(liRigidBody &Body)
{
    lpRKState *newState = new lpRKState(Body);

    Bodies.push_back(newState);
}

void lpRK4Integrator::Step(lmScalar dt)
{
    /*
     * A függvény a Runge-Kutta módszerrel 4-szer mintát vesz a test pillanatnyi állapotát megadó
     * változók deriváltjából.
     *
     * Az RK4 módszer lényege:
     * legyen y' = f(x,y) összefüggésünk y(x0) = y0 kezdeti feltételekkel.
     *
     * Ekkor az y'-ból vett 4 minta:
     * k[0] = f(x0,y0)                      //a t=0 időbeli állapot.
     * k[1] = f(x0 + dx/2,y0 + dx/2 * k[0]) //2 db minta a t= dt/2 időbeli állapotból.
     * k[2] = f(x0 + dx/2,y0 + dx/2 * k[1])
     * k[3] = f(x0 + dx,y0 + dx * k[2])     //egy utolsó minta a t = dt időbeli állapotból.
     *
     * Ezután az integrálás során vesszük a deriváltakból vett minták súlyozott átlagát:
     * y[i+1] = y[i] + dx * 1/6 *(k[0] + 2k[1] + 2k[2] + k[3])
     */

    //Végigmegyünk minden testen

    for(std::list<lpRKState *>::iterator I=Bodies.begin();I != Bodies.end();)
    {
    	if((*I)->Body.IsRemovable())
		{
			auto Tmp = I;
			auto J = std::next(I);

			SimulationWorld->RemoveBody(&(*I)->Body);

			delete *Tmp;
			Bodies.erase(Tmp);
			I = J;
		}
    	else
		{
			//Lekérdezzük a test pillanatnyi állapotát, tehát az y(x0) = y0 kezdeti feltételeket.
			//Elvégezzük a mintavételezést:
			//k[0] = f(x0,y0)
			Sample((*I)->Body,(*I)->State[0],0.0,(*I)->Derivative[0]);

			I++;
		}
    }

    for(std::list<lpRKState *>::iterator i=Bodies.begin();i != Bodies.end();i++)
    {
        InteractionModel->ApplyForces((*i)->Body,(*i)->State[0],(*i)->Derivative[0],dt/2.0);

        for(std::list<lpRKState *>::iterator j = std::next(i);j != Bodies.end();j++)
        {
        	if(((*i)->Body.GetMass() == 0.0) && ((*j)->Body.GetMass() == 0.0))
			{
				continue;
			}

            if(CollisionDetector->CheckCollision(&(*i)->Body,(*i)->State[0],&(*j)->Body,(*j)->State[0],dt/2.0,(*i)->Derivative[0],(*j)->Derivative[0],InteractionModel))
            {
                //Itt esetleg le lehet hívni egy collision callbacket.
            }
        }

		(*i)->UpdateConstraints(0);
		if((*i)->Body.HasBoundingVolume())
		{
			(*i)->Body.GetBoundingVolume()->InvalidateCache();
		}
		/*
		std::cout << (*i)->Derivative[0].Momentum[0] << ";" << (*i)->Derivative[0].Momentum[1] << ";" << (*i)->Derivative[0].Momentum[2] << std::endl;
		std::cout << (*i)->Derivative[0].NetForce[0] << ";" << (*i)->Derivative[0].NetForce[1] << ";" << (*i)->Derivative[0].NetForce[2] << std::endl;

		std::cout << (*i)->Derivative[0].AngularMomentum[0] << ";" << (*i)->Derivative[0].AngularMomentum[1] << ";" << (*i)->Derivative[0].AngularMomentum[2] << std::endl;
		std::cout << (*i)->Derivative[0].NetTorque[0] << ";" << (*i)->Derivative[0].NetTorque[1] << ";" << (*i)->Derivative[0].NetTorque[2] << std::endl;
		std::cout << std::endl;
		*/
    }

	InteractionModel->ApplyConstraintForces(dt/2.0);

    for(lpRKState *i : Bodies)
	{
        //Elvégezzük a mintavételezést:
        //k[1] = f(x0 + dx/2,y0 + dx/2 * k[0])
        Sample(i->Body,i->State[1],dt/(2.0),i->Derivative[1],&i->Derivative[0]);
	}

    for(std::list<lpRKState *>::iterator i=Bodies.begin();i != Bodies.end();i++)
    {
        InteractionModel->ApplyForces((*i)->Body,(*i)->State[1],(*i)->Derivative[1],dt/2.0);
        for(std::list<lpRKState *>::iterator j = std::next(i);j != Bodies.end();j++)
        {
        	if(((*i)->Body.GetMass() == 0.0) && ((*j)->Body.GetMass() == 0.0))
			{
				continue;
			}

            if(CollisionDetector->CheckCollision(&(*i)->Body,(*i)->State[1],&(*j)->Body,(*j)->State[1],dt/2.0,(*i)->Derivative[1],(*j)->Derivative[1],InteractionModel))
            {
                //Itt esetleg le lehet hívni egy collision callbacket.
            }
        }

		(*i)->UpdateConstraints(1);
		if((*i)->Body.HasBoundingVolume())
		{
			(*i)->Body.GetBoundingVolume()->InvalidateCache();
		}
		/*
		std::cout << (*i)->Derivative[1].Momentum[0] << ";" << (*i)->Derivative[1].Momentum[1] << ";" << (*i)->Derivative[1].Momentum[2] << std::endl;
		std::cout << (*i)->Derivative[1].NetForce[0] << ";" << (*i)->Derivative[1].NetForce[1] << ";" << (*i)->Derivative[1].NetForce[2] << std::endl;

		std::cout << (*i)->Derivative[1].AngularMomentum[0] << ";" << (*i)->Derivative[1].AngularMomentum[1] << ";" << (*i)->Derivative[1].AngularMomentum[2] << std::endl;
		std::cout << (*i)->Derivative[1].NetTorque[0] << ";" << (*i)->Derivative[1].NetTorque[1] << ";" << (*i)->Derivative[1].NetTorque[2] << std::endl;
		std::cout << std::endl;
		*/
    }

	InteractionModel->ApplyConstraintForces(dt/2.0);

    for(lpRKState *i : Bodies)
	{
        //Elvégezzük a mintavételezést:
        //k[2] = f(x0 + dx/2,y0 + dx/2 * k[1])
        Sample(i->Body,i->State[2],dt/(2.0),i->Derivative[2],&(i->Derivative[1]));
	}

    for(std::list<lpRKState *>::iterator i=Bodies.begin();i != Bodies.end();i++)
    {
        InteractionModel->ApplyForces((*i)->Body,(*i)->State[2],(*i)->Derivative[2],dt/2.0);
        for(std::list<lpRKState *>::iterator j = std::next(i);j != Bodies.end();j++)
        {
        	if(((*i)->Body.GetMass() == 0.0) && ((*j)->Body.GetMass() == 0.0))
			{
				continue;
			}

            if(CollisionDetector->CheckCollision(&(*i)->Body,(*i)->State[2],&(*j)->Body,(*j)->State[2],dt/2.0,(*i)->Derivative[2],(*j)->Derivative[2],InteractionModel))
            {
                //Itt esetleg le lehet hívni egy collision callbacket.
            }
        }

        (*i)->UpdateConstraints(2);
		if((*i)->Body.HasBoundingVolume())
		{
			(*i)->Body.GetBoundingVolume()->InvalidateCache();
		}
		/*
		std::cout << (*i)->Derivative[2].Momentum[0] << ";" << (*i)->Derivative[2].Momentum[1] << ";" << (*i)->Derivative[2].Momentum[2] << std::endl;
		std::cout << (*i)->Derivative[2].NetForce[0] << ";" << (*i)->Derivative[2].NetForce[1] << ";" << (*i)->Derivative[2].NetForce[2] << std::endl;

		std::cout << (*i)->Derivative[2].AngularMomentum[0] << ";" << (*i)->Derivative[2].AngularMomentum[1] << ";" << (*i)->Derivative[2].AngularMomentum[2] << std::endl;
		std::cout << (*i)->Derivative[2].NetTorque[0] << ";" << (*i)->Derivative[2].NetTorque[1] << ";" << (*i)->Derivative[2].NetTorque[2] << std::endl;
		std::cout << std::endl;
		*/
    }

	InteractionModel->ApplyConstraintForces(dt/2.0);

    for(lpRKState *i : Bodies)
	{
        //Elvégezzük a mintavételezést:
        //k[3] = f(x0 + dx,y0 + dx * k[2])
        Sample(i->Body,i->State[3],dt,i->Derivative[3],&(i->Derivative[2]));
	}

    for(std::list<lpRKState *>::iterator i=Bodies.begin();i != Bodies.end();i++)
    {
        InteractionModel->ApplyForces((*i)->Body,(*i)->State[3],(*i)->Derivative[3],dt);
        for(std::list<lpRKState *>::iterator j = std::next(i);j != Bodies.end();j++)
        {
        	if(((*i)->Body.GetMass() == 0.0) && ((*j)->Body.GetMass() == 0.0))
			{
				continue;
			}

            if(CollisionDetector->CheckCollision(&(*i)->Body,(*i)->State[3],&(*j)->Body,(*j)->State[3],dt,(*i)->Derivative[3],(*j)->Derivative[3],InteractionModel))
            {
                //Itt esetleg le lehet hívni egy collision callbacket.
            }
        }

		(*i)->UpdateConstraints(3);
		if((*i)->Body.HasBoundingVolume())
		{
			(*i)->Body.GetBoundingVolume()->InvalidateCache();
		}
		/*
		std::cout << (*i)->Derivative[3].Momentum[0] << ";" << (*i)->Derivative[3].Momentum[1] << ";" << (*i)->Derivative[3].Momentum[2] << std::endl;
		std::cout << (*i)->Derivative[3].NetForce[0] << ";" << (*i)->Derivative[3].NetForce[1] << ";" << (*i)->Derivative[3].NetForce[2] << std::endl;

		std::cout << (*i)->Derivative[3].AngularMomentum[0] << ";" << (*i)->Derivative[3].AngularMomentum[1] << ";" << (*i)->Derivative[3].AngularMomentum[2] << std::endl;
		std::cout << (*i)->Derivative[3].NetTorque[0] << ";" << (*i)->Derivative[3].NetTorque[1] << ";" << (*i)->Derivative[3].NetTorque[2] << std::endl;
		std::cout << std::endl;
		*/
    }

	InteractionModel->ApplyConstraintForces(dt);

    for(lpRKState *i : Bodies)
	{
        Integrate(i->Body,i->Derivative,dt);
	}
}
