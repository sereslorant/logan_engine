
#include <cmath>

/*TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#include <iostream>
/*TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

using namespace std;

#include "lpDiscreteCollisionDetector.h"

bool lpParticlePlaneCollision(lpRigidBody *Particle,lpPlane *Plane,liRigidBody::liState &ParticleState,liRigidBody::liState &PlaneState,liRigidBody::liDerivative &ParticleDerivative,liRigidBody::liDerivative &PlaneDerivative,lmScalar dt/*,bool FlipNormal*/,lpContactData &ContactData)
{
	lmScalar	IntersectionTime = 0.0;
	lmVector3D	IntersectionPoint(lmVector3D::NO_INIT);

    //lmMatrix3x3	Rotation = lmRotate3x3(PlaneState.GetOrientationQuaternion());
    lmVector3D	PlaneNormal = lmQuaternionRotate(Plane->GetNormal(),PlaneState.GetOrientationQuaternion()); //Rotation * Plane->GetNormal();
    lmVector3D	ParticleVelocity = Particle->GetInvMass() * ParticleState.GetMomentum();

	if(lmRayPlaneIntersection(ParticleState.GetPosition(),ParticleVelocity,PlaneState.GetPosition(),PlaneNormal,&IntersectionTime,&IntersectionPoint))
	{
		if((IntersectionTime > 0.0) && (IntersectionTime < dt))
		{
			lmVector3D ParticleIntersection(lmVector3D::NULLVEC);
			lmVector3D PlaneIntersection = IntersectionPoint - PlaneState.GetPosition();
			lmVector3D &Normal = PlaneNormal;// if(FlipNormal){Normal *= -1.0;}
			lmScalar PenetrationDepth = sqrt((ParticleVelocity * (dt - IntersectionTime)).LengthSquared());

			ContactData.Body1 = Particle;
			ContactData.Body2 = Plane;
			ContactData.State1 = &ParticleState;
			ContactData.State2 = &PlaneState;
			ContactData.Derivative1 = &ParticleDerivative;
			ContactData.Derivative2 = &PlaneDerivative;

			ContactData.CollisionData.push_back({ParticleIntersection,PlaneIntersection,Normal,PenetrationDepth});

			return true;
		}
	}

	return false;
}

bool lpParticleSphereCollision(lpRigidBody *Particle,lpSphere *Sphere,liRigidBody::liState &ParticleState,liRigidBody::liState &SphereState,liRigidBody::liDerivative &ParticleDerivative,liRigidBody::liDerivative &SphereDerivative,lmScalar dt/*,bool FlipNormal*/,lpContactData &ContactData)
{
	lmScalar t1 = -1.0;
	lmScalar t2 = -1.0;

	if(lmRaySphereIntersection(ParticleState.GetPosition(),Particle->GetInvMass()*ParticleState.GetMomentum(),SphereState.GetPosition(),Sphere->GetRadius(),&t1,&t2))
	{
		lmScalar tMin = -1.0;

		if(t1 > 0.0 && t1 <= dt)
			{tMin = t1;}

		if(t2 > 0.0 && t2 <= dt)
			{tMin = std::min(t2,std::max(tMin,(lmScalar)0.0));}

		if(tMin < 0.0)
		{
			return false;
		}
		else
		{
			lmVector3D ParticleIntersection(lmVector3D::NULLVEC);
			lmVector3D SphereIntersection = ParticleState.GetPosition() + (Particle->GetInvMass()*ParticleState.GetMomentum() * tMin) - SphereState.GetPosition();
			lmVector3D Normal = SphereIntersection - SphereState.GetPosition();
			Normal.Normalize();
			lmScalar PenetrationDepth = std::sqrt((Particle->GetInvMass()*ParticleState.GetMomentum() * (dt - tMin)).LengthSquared());

			ContactData.Body1 = Particle;


			ContactData.Body1 = Particle;
			ContactData.Body2 = Sphere;
			ContactData.State1 = &ParticleState;
			ContactData.State2 = &SphereState;
			ContactData.Derivative1 = &ParticleDerivative;
			ContactData.Derivative2 = &SphereDerivative;

			ContactData.CollisionData.push_back({ParticleIntersection,SphereIntersection,Normal,PenetrationDepth});

			return true;
		}
	}

	return false;
}

bool lpParticleMeshCollision(lpRigidBody *Particle,lpMesh *Mesh,liBoundingVolume *bounding_volume,liRigidBody::liState &ParticleState,liRigidBody::liState &MeshState,liRigidBody::liDerivative &ParticleDerivative,liRigidBody::liDerivative &MeshDerivative,lmScalar dt/*,bool FlipNormal*/,lpContactData &ContactData)
{
    //lmVector3D MeshIntersection(lmVector3D::NO_INIT);
    lmScalar PenetrationDepth;
    bool Collides = false;

    //lmMatrix3x3 MeshRotate = lmRotate3x3(MeshState.GetOrientationQuaternion());

    //for(unsigned int i=0;i < Mesh->GetTrianglesSize();i++)
    //for(const lpTriangle &cTriangle : MeshTriangles)
    for(unsigned int i=0;i < bounding_volume->GetDataSize();i++)
    {
    	liTriangleCache *Triangle = bounding_volume->GetData(i);

    	if(!Triangle->IsValid())
		{
			Triangle->Update(Mesh,MeshState.GetPosition(),MeshState.GetOrientationQuaternion());
		}

		const lmVector3D &Vertex1 = Triangle->GetVertex(0);
        const lmVector3D &Vertex2 = Triangle->GetVertex(1);
        const lmVector3D &Vertex3 = Triangle->GetVertex(2);

        const lmVector3D &Normal = Triangle->GetNormal();

    	/*
        const lpTriangle &cTriangle = Mesh->GetTriangle(i);

        lmVector3D Vertex1 = (MeshRotate * Mesh->GetVertex(cTriangle.V[0])) + MeshState.GetPosition();
        lmVector3D Vertex2 = (MeshRotate * Mesh->GetVertex(cTriangle.V[1])) + MeshState.GetPosition();
        lmVector3D Vertex3 = (MeshRotate * Mesh->GetVertex(cTriangle.V[2])) + MeshState.GetPosition();

        lmVector3D Normal = MeshRotate * Mesh->GetNormal(cTriangle.Normal);
        Normal.Normalize();
        */

        lmScalar IntersectionTime;
        lmVector3D IntersectionPoint(lmVector3D::NO_INIT);

        lmVector3D ParticleVelocity = Particle->GetInvMass() * ParticleState.GetMomentum();
		if(lmRayTriangleIntersection(ParticleState.GetPosition(),ParticleVelocity,Vertex1,Vertex2,Vertex3,Normal,&IntersectionTime,&IntersectionPoint))
		{
			if((IntersectionTime > 0.0) && (IntersectionTime < dt))
			{
				Collides = true;

				lmVector3D ParticleIntersection(lmVector3D::NULLVEC);
				lmVector3D MeshIntersection = (IntersectionPoint - MeshState.GetPosition());
				//if(FlipNormal){Normal *= -1.0;}
				lmScalar PenetrationDepth = sqrt((ParticleVelocity * (dt - IntersectionTime)).LengthSquared());

				ContactData.CollisionData.push_back({ParticleIntersection,MeshIntersection,Normal,PenetrationDepth});
			}
		}
    }

	if(Collides)
	{
		ContactData.Body1 = Particle;
		ContactData.Body2 = Mesh;
		ContactData.State1 = &ParticleState;
		ContactData.State2 = &MeshState;
		ContactData.Derivative1 = &ParticleDerivative;
		ContactData.Derivative2 = &MeshDerivative;
	}

    return Collides;
}


bool lpSphereSphereCollision(lpSphere *Sphere_i,lpSphere *Sphere_j,liRigidBody::liState &State_i,liRigidBody::liState &State_j,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j/*,bool FlipNormal*/,lpContactData &ContactData)
{
    lmScalar cDist;
    if(lmSphereSphereIntersection(State_i.GetPosition(),Sphere_i->GetRadius(),State_j.GetPosition(),Sphere_j->GetRadius(),&cDist))
    {
        lmVector3D Normal = State_i.GetPosition() - State_j.GetPosition();
        Normal.Normalize();

        lmVector3D Intersection_i = -1.0*Sphere_i->GetRadius() * Normal;
        lmVector3D Intersection_j =		 Sphere_j->GetRadius() * Normal;
		//if(FlipNormal){Normal *= -1.0;}
        lmScalar PenetrationDepth = abs(Sphere_i->GetRadius() + Sphere_j->GetRadius() - sqrt(cDist));

		ContactData.Body1 = Sphere_i;
		ContactData.Body2 = Sphere_j;
		ContactData.State1 = &State_i;
		ContactData.State2 = &State_j;
		ContactData.Derivative1 = &Derivative_i;
		ContactData.Derivative2 = &Derivative_j;

		ContactData.CollisionData.push_back({Intersection_i,Intersection_j,Normal,PenetrationDepth});

        return true;
    }

    return false;
}

bool lpSpherePlaneCollision(lpSphere *Sphere,lpPlane *Plane,liRigidBody::liState &SphereState,liRigidBody::liState &PlaneState,liRigidBody::liDerivative &SphereDerivative,liRigidBody::liDerivative &PlaneDerivative/*,bool FlipNormal*/,lpContactData &ContactData)
{
    lmScalar cDist;

    //lmMatrix3x3 Rotation = lmRotate3x3(PlaneState.GetOrientationQuaternion());
    lmVector3D PlaneNormal = lmQuaternionRotate(Plane->GetNormal(),PlaneState.GetOrientationQuaternion());//= Rotation * Plane->GetNormal();
    if(lmSpherePlaneIntersection(SphereState.GetPosition(),Sphere->GetRadius(),PlaneState.GetPosition(),PlaneNormal,&cDist))
    {
        lmVector3D SphereIntersection = (-Sphere->GetRadius() * PlaneNormal);
        lmVector3D PlaneIntersection(lmVector3D::NULLVEC); //A végtelen síknak nincs forgó mozgása, ezért nincs még jelentősége visszaadni bármit is.
        lmVector3D &Normal = PlaneNormal;// if(FlipNormal){Normal *= -1.0;}
        lmScalar PenetrationDepth = abs(Sphere->GetRadius() - abs(cDist));

		ContactData.Body1 = Sphere;
		ContactData.Body2 = Plane;
		ContactData.State1 = &SphereState;
		ContactData.State2 = &PlaneState;
		ContactData.Derivative1 = &SphereDerivative;
		ContactData.Derivative2 = &PlaneDerivative;

		ContactData.CollisionData.push_back({SphereIntersection,PlaneIntersection,Normal,PenetrationDepth});

        return true;
    }

    return false;
}

bool lpSphereMeshCollision(lpSphere *Sphere,lpMesh *Mesh,liBoundingVolume *bounding_volume,liRigidBody::liState &SphereState,liRigidBody::liState &MeshState,liRigidBody::liDerivative &SphereDerivative,liRigidBody::liDerivative &MeshDerivative/*,bool FlipNormal*/,lpContactData &ContactData)
{
    lmVector3D SphereIntersection(lmVector3D::NULLVEC);
    lmVector3D MeshIntersection(lmVector3D::NULLVEC);
    lmScalar PenetrationDepth;
    bool Collides = false;

    //lmMatrix3x3 MeshRotate = lmRotate3x3(MeshState.GetOrientationQuaternion());

    //for(unsigned int i=0;i < Mesh->GetTrianglesSize();i++)
    //for(const lpTriangle &cTriangle : MeshTriangles)
    //{
        //const lpTriangle &cTriangle = Mesh->GetTriangle(i);
	for(unsigned int i=0;i < bounding_volume->GetDataSize();i++)
    {
    	liTriangleCache *Triangle = bounding_volume->GetData(i);

    	if(!Triangle->IsValid())
		{
			Triangle->Update(Mesh,MeshState.GetPosition(),MeshState.GetOrientationQuaternion());
		}

		const lmVector3D &Vertex1 = Triangle->GetVertex(0);
        const lmVector3D &Vertex2 = Triangle->GetVertex(1);
        const lmVector3D &Vertex3 = Triangle->GetVertex(2);

        const lmVector3D &Normal = Triangle->GetNormal();
        /*
        lmVector3D Vertex1 = (MeshRotate * Mesh->GetVertex(cTriangle.V[0])) + MeshState.GetPosition();
        lmVector3D Vertex2 = (MeshRotate * Mesh->GetVertex(cTriangle.V[1])) + MeshState.GetPosition();
        lmVector3D Vertex3 = (MeshRotate * Mesh->GetVertex(cTriangle.V[2])) + MeshState.GetPosition();

        lmVector3D Normal = MeshRotate * Mesh->GetNormal(cTriangle.Normal);
        Normal.Normalize();
        */

        lmScalar IntersectionDist;
        lmVector3D IntersectionPoint(lmVector3D::NO_INIT);

		if(lmSphereTriangleIntersection(SphereState.GetPosition(),Sphere->GetRadius(),Vertex1,Vertex2,Vertex3,Normal,&IntersectionDist,&IntersectionPoint))
		{
			Collides = true;

			SphereIntersection = (Sphere->GetRadius() * (-1.0*Normal));
			MeshIntersection = (IntersectionPoint - MeshState.GetPosition());
			//if(FlipNormal){Normal *= -1.0;}
			PenetrationDepth = abs(Sphere->GetRadius() - abs(IntersectionDist));

			ContactData.CollisionData.push_back({SphereIntersection,MeshIntersection,Normal,PenetrationDepth});
		}
    }

	if(Collides)
	{
		ContactData.Body1 = Sphere;
		ContactData.Body2 = Mesh;
		ContactData.State1 = &SphereState;
		ContactData.State2 = &MeshState;
		ContactData.Derivative1 = &SphereDerivative;
		ContactData.Derivative2 = &MeshDerivative;
	}

    return Collides;
}
/*
struct lpTriangleData
{
	lmVector3D P1;
	lmVector3D P2;
	lmVector3D P3;

	lmVector3D Center;
	lmVector3D Normal;

	lpTriangleData(const lmVector3D &p1,const lmVector3D &p2,const lmVector3D &p3,const lmVector3D &normal)
		:P1(p1),P2(p2),P3(p3),Normal(normal)
	{
		Center = (P1 + P2 + P3) * (1.0/3.0);
	}
};
*/
int lpTriangleFindMaxPenetration(const lmVector3D &T1_Point,const lmVector3D &T1_Normal,const lmVector3D &Pos_1,const lmVector3D &T2_P0,const lmVector3D &T2_P1,const lmVector3D &T2_P2,const lmVector3D &T2_Normal,const lmVector3D &Pos_2,int T1Det[],lmScalar *PenetrationDepth = nullptr,lmVector3D *Intersection_T1 = nullptr,lmVector3D *Intersection_T2 = nullptr)
{
	lmScalar PenetrationDepth_T[3] = {0.0,0.0,0.0};
	const lmVector3D *T2_P[3] = {&T2_P0,&T2_P1,&T2_P2};

	int PMax = -1;


	for(int i=0;i < 3;i++)
	{
		if(T1Det[i] > 0)
		{
			//lmRayPlaneIntersection(*T2_P[i],T1.Normal,T1.P1,T1.Normal,&PenetrationDepth_T[i],&Intersection_T[i]);
			//PenetrationDepth_T[i] = std::abs(std::min(PenetrationDepth_T[i],(lmScalar)0.0));

			PenetrationDepth_T[i] = std::abs(std::max(lmDot(T1_Normal,*T2_P[i] - T1_Point),(lmScalar)0.0));

			//if(PenetrationDepth_T[i] != 0.0)
			if(PMax == -1 || ((PMax != -1) && (PenetrationDepth_T[PMax] < PenetrationDepth_T[i])))
			{
				PMax = i;
			}
		}
	}

	if(PMax != -1)
	{
		lmVector3D Intersection(lmVector3D::NO_INIT);
		lmRayPlaneIntersection(*T2_P[PMax],T1_Normal,T1_Point,T1_Normal,(lmScalar *)nullptr,&Intersection);

		if(PenetrationDepth != nullptr)
			{*PenetrationDepth = PenetrationDepth_T[PMax];}

		if(Intersection_T1 != nullptr)
			{*Intersection_T1 = Intersection - Pos_1;}

		if(Intersection_T2 != nullptr)
			{*Intersection_T2 = *T2_P[PMax] - Pos_2;}
	}

	return PMax;
}

bool lpMeshMeshCollision(lpMesh *Body_i,lpMesh *Body_j,liBoundingVolume *bounding_volume_i,liBoundingVolume *bounding_volume_j,liRigidBody::liState &State_i,liRigidBody::liState &State_j,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j/*,bool FlipNormal*/,lpContactData &ContactData)
{
    /*
     * VIGYÁZAT!!! A MESH-MESH COLLISION DETECTION EGYELŐRE MŰKÖDÉSKÉPTELEN!!!
     * ...pontosabban mostmár csak szar...
     * ...mondjuk mostmár működőképes...
     * ...Najó, tökéletesen működik. :P Csak optimalizálatlan. :P
     * ...pontosabban nem tökéletesen, mert ha lap lapra esik, akkor átmegy az egyik a másikon. (pl. 2 kocka esetén)
     *
     */

    /*
     * MEGJEGYZÉS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
     * A meshek közötti ütközésellenőrzés egy az egyben elkerühető lenne abban az esetben, ha találunk síkot, amely
     * elválasztja egymástól a két mesht. (Konvex esetben. Nem konvex esetben fel kell darabolni a mesht konvex
     * darabokra.)
     *
     * Ez a sík az esetek többségében a mesh egyik lapja. Ezt itt még nem implementáltam, de optimalizálási lehetőség.
     * További infóért érdemes megnézni a "Separating axis theorem"-et.
     *
     * Érdemes lehet meggondolni a bounding sphere használatát is.
     *
     * A mesh térpartícionálásával a háromszög metszésdetektálások számát lehetne csökkenteni.
     * (pl. bináris térpartícionáló fákkal, vagy octreekkel.)
     *
     */

    bool Collides = false;

    for(unsigned int i=0;i < bounding_volume_i->GetDataSize();i++)
    {
    	liTriangleCache *Triangle_i = bounding_volume_i->GetData(i);

    	if(!Triangle_i->IsValid())
		{
			Triangle_i->Update(Body_i,State_i.GetPosition(),State_i.GetOrientationQuaternion());
		}

		const lmVector3D &P1i = Triangle_i->GetVertex(0);
        const lmVector3D &P2i = Triangle_i->GetVertex(1);
        const lmVector3D &P3i = Triangle_i->GetVertex(2);

        const lmVector3D &Normal_i = Triangle_i->GetNormal();

        for(unsigned int j=0;j < bounding_volume_j->GetDataSize();j++)
		{
			liTriangleCache *Triangle_j = bounding_volume_j->GetData(j);

			if(!Triangle_j->IsValid())
			{
				Triangle_j->Update(Body_j,State_j.GetPosition(),State_j.GetOrientationQuaternion());
			}

			const lmVector3D &P1j = Triangle_j->GetVertex(0);
			const lmVector3D &P2j = Triangle_j->GetVertex(1);
			const lmVector3D &P3j = Triangle_j->GetVertex(2);

			const lmVector3D &Normal_j = Triangle_j->GetNormal();

			int Pi_Sgn[3];// = {-5,-5,-5};
			int Pj_Sgn[3];// = {-5,-5,-5};

            LM_TRIANGLE_INTERSECTION Intersection = lmTriangleTriangleCollision(P1i,P2i,P3i,Normal_i,P1j,P2j,P3j,Normal_j,Pi_Sgn,Pj_Sgn);

            if(Intersection == LM_TRIANGLE_VERTEX)
            {
				lmScalar PenetrationDepth_i;
				lmVector3D Intersection_ji(lmVector3D::NO_INIT);
				lmVector3D Intersection_pj(lmVector3D::NO_INIT);

				int PiMax = lpTriangleFindMaxPenetration(P1i,Normal_i,State_i.GetPosition(),P1j,P2j,P3j,Normal_j,State_j.GetPosition(),Pi_Sgn,
															&PenetrationDepth_i,&Intersection_ji,&Intersection_pj);

				lmScalar PenetrationDepth_j;
				lmVector3D Intersection_ij(lmVector3D::NO_INIT);
				lmVector3D Intersection_pi(lmVector3D::NO_INIT);

				int PjMax = lpTriangleFindMaxPenetration(P1j,Normal_j,State_j.GetPosition(),P1i,P2i,P3i,Normal_i,State_i.GetPosition(),Pj_Sgn,
															&PenetrationDepth_j,&Intersection_ij,&Intersection_pi);

				if(PjMax != -1 && PiMax != -1)
				{
					if(PenetrationDepth_i < PenetrationDepth_j)
					{
						ContactData.CollisionData.push_back({Intersection_ji,Intersection_pj,-1.0 * Normal_i,PenetrationDepth_i});
					}
					else
					{
						ContactData.CollisionData.push_back({Intersection_pi,Intersection_ij,Normal_j,PenetrationDepth_j});
					}

					//ContactData.CollisionData.push_back({Intersection_i,Intersection_j,Normal,PenetrationDepth});
					Collides = true;
				}
            }
            else if(Intersection == LM_TRIANGLE_PLANE)
			{
				lmVector3D Average = (P1i + P2i + P3i) * (1.0 / 3.0);

				lmVector3D Intersection_i = Average - State_i.GetPosition();
				lmVector3D Intersection_j = Average - State_j.GetPosition();
				//lmVector3D Normal = -1.0*Normal_i;//if(FlipNormal){Normal *= -1.0;}
				lmScalar PenetrationDepth = 0.2;

				ContactData.CollisionData.push_back({Intersection_i,Intersection_j,-1.0*Normal_i,PenetrationDepth});
				Collides = true;
			}
        }
    }

	if(Collides)
	{
		ContactData.Body1 = Body_i;
		ContactData.Body2 = Body_j;
		ContactData.State1 = &State_i;
		ContactData.State2 = &State_j;
		ContactData.Derivative1 = &Derivative_i;
		ContactData.Derivative2 = &Derivative_j;
	}

    return Collides;
}

bool lpMeshPlaneCollision(lpMesh *Mesh,lpPlane *Plane,liBoundingVolume *bounding_volume,liRigidBody::liState &MeshState,liRigidBody::liState &PlaneState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &PlaneDerivative/*,bool FlipNormal*/,lpContactData &ContactData)
{
    lmVector3D PlaneIntersection(lmVector3D::NO_INIT);
    lmVector3D MeshIntersection(lmVector3D::NO_INIT);
    lmScalar PenetrationDepth;
    bool Collides = false;

    //lmMatrix3x3 MeshRotate = lmRotate3x3(MeshState.GetOrientationQuaternion());

    //lmMatrix3x3 PlaneRotate = lmRotate3x3(PlaneState.GetOrientationQuaternion());
    lmVector3D	PlaneNormal = lmQuaternionRotate(Plane->GetNormal(),PlaneState.GetOrientationQuaternion());

    //for(unsigned int i=0;i < Mesh->GetTrianglesSize();i++)
    //for(const lpTriangle &cTriangle : MeshTriangles)
    //{
        //const lpTriangle &cTriangle = Mesh->GetTriangle(i);
	for(unsigned int i=0;i < bounding_volume->GetDataSize();i++)
    {
    	liTriangleCache *Triangle = bounding_volume->GetData(i);

    	if(!Triangle->IsValid())
		{
			Triangle->Update(Mesh,MeshState.GetPosition(),MeshState.GetOrientationQuaternion());
		}

		const lmVector3D &Vertex1 = Triangle->GetVertex(0);
        const lmVector3D &Vertex2 = Triangle->GetVertex(1);
        const lmVector3D &Vertex3 = Triangle->GetVertex(2);

        const lmVector3D &TriangleNormal = Triangle->GetNormal();
        /*
        lmVector3D Vertex1 = (MeshRotate * Mesh->GetVertex(cTriangle.V[0])) + MeshState.GetPosition();
        lmVector3D Vertex2 = (MeshRotate * Mesh->GetVertex(cTriangle.V[1])) + MeshState.GetPosition();
        lmVector3D Vertex3 = (MeshRotate * Mesh->GetVertex(cTriangle.V[2])) + MeshState.GetPosition();

        lmVector3D TriangleNormal = MeshRotate * Mesh->GetNormal(cTriangle.Normal);
        TriangleNormal.Normalize();
        */
		int Dot_P0_Sgn;
		int Dot_P1_Sgn;
		int Dot_P2_Sgn;

        LM_TRIANGLE_INTERSECTION IntersectionType = lmTrianglePlaneIntersection(PlaneState.GetPosition(),PlaneNormal,Vertex1,Vertex2,Vertex3,TriangleNormal,Dot_P0_Sgn,Dot_P1_Sgn,Dot_P2_Sgn);

        if(IntersectionType != LM_NO_INTERSECTION)
		{Collides = true;}
		else
		{continue;}

		lmVector3D &Normal = PlaneNormal;

		if(IntersectionType == LM_TRIANGLE_VERTEX)
		{
			lmScalar NumSzercsy = 0.0;
			lmVector3D Szercsy;

			if(Dot_P0_Sgn == -1)
			{Szercsy += Vertex1;NumSzercsy += 1.0;}
			if(Dot_P1_Sgn == -1)
			{Szercsy += Vertex2;NumSzercsy += 1.0;}
			if(Dot_P2_Sgn == -1)
			{Szercsy += Vertex3;NumSzercsy += 1.0;}

			Szercsy *= 1.0/NumSzercsy;

			lmScalar 	IntersectionDist;
			lmVector3D	IntersectionPoint(lmVector3D::NO_INIT);
			lmRayPlaneIntersection(Szercsy,PlaneNormal,PlaneState.GetPosition(),PlaneNormal,&IntersectionDist,&IntersectionPoint);

			//std::cout << "(" << Dot_P0_Sgn << ";" << Dot_P1_Sgn << ";" << Dot_P2_Sgn << ")" << IntersectionDist << std::endl;

			PlaneIntersection = (IntersectionPoint - PlaneState.GetPosition());
			MeshIntersection = (Szercsy - MeshState.GetPosition());
			//if(FlipNormal){Normal *= -1.0;}
			PenetrationDepth = std::abs(IntersectionDist);

			ContactData.CollisionData.push_back({MeshIntersection,PlaneIntersection,Normal,PenetrationDepth});
		}
		else if(IntersectionType == LM_TRIANGLE_PLANE)
		{
			lmVector3D Average = (Vertex1 + Vertex2 + Vertex3)*(1.0/3.0);

			PlaneIntersection = (Average - PlaneState.GetPosition());
			MeshIntersection = (Average - MeshState.GetPosition());
			//if(FlipNormal){Normal *= -1.0;}
			PenetrationDepth = 0.2;

			ContactData.CollisionData.push_back({MeshIntersection,PlaneIntersection,Normal,PenetrationDepth});
		}
    }

	if(Collides)
	{
		ContactData.Body1 = Mesh;
		ContactData.Body2 = Plane;
		ContactData.State1 = &MeshState;
		ContactData.State2 = &PlaneState;
		ContactData.Derivative1 = &MeshDerivative;
		ContactData.Derivative2 = &PlaneDerivative;
	}

    return Collides;
}

bool lpBoundingVolumeCollision(liBoundingVolume *bounding_volume_1,const lmVector3D &position_1,const lmQuaternion &rotation_1,liBoundingVolume *bounding_volume_2,const lmVector3D &position_2,const lmQuaternion &rotation_2)
{
	if(bounding_volume_1->GetType() == liBoundingVolume::LP_SPHERE_TREE && bounding_volume_2->GetType() == liBoundingVolume::LP_SPHERE_TREE)
	{
		liSphere *SphereTree1 = bounding_volume_1->ToSphere();
		liSphere *SphereTree2 = bounding_volume_2->ToSphere();

		lmVector3D SpherePosition1 = position_1 + lmQuaternionRotate(SphereTree1->GetPosition(),rotation_1);
		lmVector3D SpherePosition2 = position_2 + lmQuaternionRotate(SphereTree2->GetPosition(),rotation_2);

		return lmSphereSphereIntersection(SpherePosition1,SphereTree1->GetRadius(),SpherePosition2,SphereTree2->GetRadius());
	}
	else if(bounding_volume_1->GetType() == liBoundingVolume::LP_AABB_TREE && bounding_volume_2->GetType() == liBoundingVolume::LP_SPHERE_TREE)
	{
		liAABB	 *AABBTree	 = bounding_volume_1->ToAABB();
		liSphere *SphereTree = bounding_volume_2->ToSphere();

		lmVector3D SpherePosition = lmQuaternionRotate((lmQuaternionRotate(SphereTree->GetPosition(),rotation_2) + position_2 - position_1),rotation_1.GetInverse());

		return lmSphereAABBCollision(SpherePosition,SphereTree->GetRadius(),AABBTree->GetMin(),AABBTree->GetMax());
	}
	else if(bounding_volume_1->GetType() == liBoundingVolume::LP_SPHERE_TREE && bounding_volume_2->GetType() == liBoundingVolume::LP_AABB_TREE)
	{
		liSphere *SphereTree = bounding_volume_1->ToSphere();
		liAABB	 *AABBTree	 = bounding_volume_2->ToAABB();

		lmVector3D SpherePosition = lmQuaternionRotate((lmQuaternionRotate(SphereTree->GetPosition(),rotation_1) + position_1 - position_2),rotation_2.GetInverse());

		return lmSphereAABBCollision(SpherePosition,SphereTree->GetRadius(),AABBTree->GetMin(),AABBTree->GetMax());
	}
	else if(bounding_volume_1->GetType() == liBoundingVolume::LP_AABB_TREE && bounding_volume_2->GetType() == liBoundingVolume::LP_AABB_TREE)
	{
		liAABB *AABBTree1 = bounding_volume_1->ToAABB();
		liAABB *AABBTree2 = bounding_volume_2->ToAABB();

		/*
		 * TMP!!!!!!!!!!!! Lehet, hogy jobb lenne, ha csak az egyik AABB-t vinnénk át a másik koordinátarendszerébe!
		 */

		lmVector3D AABB1Min(lmVector3D::NO_INIT);
		lmVector3D AABB1Max(lmVector3D::NO_INIT);
		lmLooseAABB(/*position_1,*/rotation_1,AABBTree1->GetMin(),AABBTree1->GetMax(),AABB1Min,AABB1Max);
		AABB1Min += position_1;
		AABB1Max += position_1;

		lmVector3D AABB2Min(lmVector3D::NO_INIT);
		lmVector3D AABB2Max(lmVector3D::NO_INIT);
		lmLooseAABB(/*position_2,*/rotation_2,AABBTree2->GetMin(),AABBTree2->GetMax(),AABB2Min,AABB2Max);
		AABB2Min += position_2;
		AABB2Max += position_2;

		return lmAABBAABBCollision(AABB1Min,AABB1Max,AABB2Min,AABB2Max);
	}
	else
	{
		std::cout << "Nem implementáltunk le ütközésdetektálást egy új bounding volume-hoz!" << std::endl;
		return false;
	}
}

bool lpBoundingVolumePrimitiveCollision(liBoundingVolume *bounding_volume,const lmVector3D &position,const lmQuaternion &rotation,liRigidBody *body,liRigidBody::liState *state,lmScalar dt)
{
	if(bounding_volume->GetType() == liBoundingVolume::LP_SPHERE_TREE)
	{
		liSphere *sphere_tree = bounding_volume->ToSphere();
		lmVector3D SpherePosition = position + lmQuaternionRotate(sphere_tree->GetPosition(),rotation);

		if(body->GetType() == lpRigidBody::LP_PARTICLE)
		{
			lmScalar t1 = -1.0;
			lmScalar t2 = -1.0;

			if((state->GetPosition() - SpherePosition).LengthSquared() < sphere_tree->GetRadius()*sphere_tree->GetRadius())
			{
				return true;
			}
			else if(lmRaySphereIntersection(state->GetPosition(),body->GetInvMass() * state->GetMomentum(),SpherePosition,sphere_tree->GetRadius(),&t1,&t2))
			{
				lmScalar tMin = -1.0;

				if(t1 > 0.0 && t1 <= dt) {tMin = t1;}

				if(t2 > 0.0 && t2 <= dt) {tMin = std::min(t2,std::max(tMin,(lmScalar)0.0));}

				if(tMin < 0.0)
					{return false;}
				else
					{return true;}
			}
			else
			{
				return false;
			}
		}
		else if(body->GetType() == lpRigidBody::LP_PLANE)
		{
			//lmVector3D PlaneNormal = lmRotate3x3(state->GetOrientationQuaternion()) * ((lpPlane *)body)->GetNormal();
			lmVector3D PlaneNormal = lmQuaternionRotate(((lpPlane *)body)->GetNormal(),state->GetOrientationQuaternion());
			return lmSpherePlaneIntersection(SpherePosition,sphere_tree->GetRadius(),state->GetPosition(),PlaneNormal);
		}
		else if(body->GetType() == lpRigidBody::LP_SPHERE)
		{
			return lmSphereSphereIntersection(SpherePosition,sphere_tree->GetRadius(),state->GetPosition(),((lpSphere *)body)->GetRadius());
		}
	}
	else if(bounding_volume->GetType() == liBoundingVolume::LP_AABB_TREE)
	{
		liAABB *AABBTree = bounding_volume->ToAABB();

		if(body->GetType() == lpRigidBody::LP_PARTICLE)
		{
			lmVector3D P0 = lmQuaternionRotate(state->GetPosition() - position,rotation.GetInverse());

			lmVector3D Velocity = lmQuaternionRotate((body->GetInvMass() * state->GetMomentum()),rotation.GetInverse());
			lmVector3D P1 = P0 + Velocity * dt;

			return lmCohenSutherlandLineAABBCollision(P0,P1,AABBTree->GetMin(),AABBTree->GetMax());
		}
		else if(body->GetType() == lpRigidBody::LP_PLANE)
		{
			lmVector3D PlaneNormal = lmQuaternionRotate(((lpPlane *)body)->GetNormal(),rotation.GetInverse() * state->GetOrientationQuaternion());

			lmVector3D PositiveVertex = AABBTree->GetMin();
			if (PlaneNormal[0] >= 0.0)
				{PositiveVertex[0] = AABBTree->GetMax()[0];}
			if (PlaneNormal[1] >= 0.0)
				{PositiveVertex[1] = AABBTree->GetMax()[1];}
			if (PlaneNormal[2] >= 0.0)
				{PositiveVertex[2] = AABBTree->GetMax()[2];}

			lmVector3D NegativeVertex = AABBTree->GetMax();
			if (PlaneNormal[0] >= 0.0)
				{NegativeVertex[0] = AABBTree->GetMin()[0];}
			if (PlaneNormal[1] >= 0.0)
				{NegativeVertex[1] = AABBTree->GetMin()[1];}
			if (PlaneNormal[2] >= 0.0)
				{NegativeVertex[2] = AABBTree->GetMin()[2];}

			lmVector3D PlanePoint = lmQuaternionRotate(state->GetPosition()-position,rotation.GetInverse());

			//std::cout << "EPIC FAIL!!!!!!!!!!!!!\n";
			int PositiveVertexSgn = sgn(lmDot(PositiveVertex - PlanePoint,PlaneNormal));
			int NegativeVertexSgn = sgn(lmDot(NegativeVertex - PlanePoint,PlaneNormal));
			//std::cout << "EPIC FAIL!!!!!!!!!!!!!\n";

			if(PositiveVertexSgn != NegativeVertexSgn)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if(body->GetType() == lpRigidBody::LP_SPHERE)
		{
			return lmSphereAABBCollision(lmQuaternionRotate((state->GetPosition() - position),rotation.GetInverse()),((lpSphere *)body)->GetRadius(),AABBTree->GetMin(),AABBTree->GetMax());
		}
	}
	else
	{
		std::cout << "Nem implementáltunk le ütközésdetektálást egy új bounding volume-hoz!" << std::endl;
	}
}

bool lpBoundingVolumeHierarchyCollision(lpMesh *Body1,lpMesh *Body2,liRigidBody::liState &State1,liRigidBody::liState &State2,liRigidBody::liDerivative &Derivative1,liRigidBody::liDerivative &Derivative2,lpContactData &ContactData,
										liBoundingVolume *bounding_volume_1,const lmVector3D &position1,const lmQuaternion &rotation_1,
										liBoundingVolume *bounding_volume_2,const lmVector3D &position2,const lmQuaternion &rotation_2
										//,int Depth = 0
										)
{
	bool Collides = false;
	/*
	lmVector3D SpherePosition1 = position1 + RotationMatrix1 * sphere_tree1->GetPosition();
	lmVector3D SpherePosition2 = position2 + RotationMatrix2 * sphere_tree2->GetPosition();
	*/

	//lmVector3D SpherePosition1 = position1 + RotationMatrix1 * ((lSphereTree *)bounding_volume_1)->GetPosition();
	//lmVector3D SpherePosition2 = position2 + RotationMatrix2 * ((lSphereTree *)bounding_volume_2)->GetPosition();
	//std::cout << Depth << "\n";

	if(lpBoundingVolumeCollision(bounding_volume_1,position1,rotation_1,bounding_volume_2,position2,rotation_2))
	{
		/*
		if((bounding_volume_1->GetType() == lBoundingVolume::LP_AABB_TREE) && (bounding_volume_2->GetType() == lBoundingVolume::LP_AABB_TREE))
		{
			std::cout << "Szercsy" << std::endl;
		}
		*/

		if((bounding_volume_1->GetDataSize() != 0) && (bounding_volume_2->GetDataSize() != 0))
		{
			//std::cout << "Szercsy" << std::endl;
			bool RetVal = lpMeshMeshCollision(Body1,Body2,bounding_volume_1,bounding_volume_2,State1,State2,Derivative1,Derivative2,ContactData);
			if(Collides == false){Collides = RetVal;}
		}

		for(unsigned int i=0;i<bounding_volume_2->GetNumChildren();i++)
		{
			liBoundingVolume *bv_2_child = bounding_volume_2->GetChild(i);
			if(bv_2_child == nullptr){continue;}

			bool RetVal = lpBoundingVolumeHierarchyCollision(Body1,Body2,State1,State2,Derivative1,Derivative2,ContactData,bounding_volume_1,position1,rotation_1,bv_2_child,position2,rotation_2/*,Depth+1*/);
			if(Collides == false){Collides = RetVal;}
		}

		for(unsigned int i=0;i<bounding_volume_1->GetNumChildren();i++)
		{
			liBoundingVolume *bv_1_child = bounding_volume_1->GetChild(i);
			if(bv_1_child == nullptr){continue;}

			bool RetVal = lpBoundingVolumeHierarchyCollision(Body1,Body2,State1,State2,Derivative1,Derivative2,ContactData,bv_1_child,position1,rotation_1,bounding_volume_2,position2,rotation_2/*,Depth+1*/);
			if(Collides == false){Collides = RetVal;}

			for(unsigned int j=0;j < bounding_volume_2->GetNumChildren();j++)
			{
				liBoundingVolume *bv_2_child = bounding_volume_2->GetChild(j);
				if(bv_2_child == nullptr){continue;}

				bool RetVal = lpBoundingVolumeHierarchyCollision(Body1,Body2,State1,State2,Derivative1,Derivative2,ContactData,bv_1_child,position1,rotation_1,bv_2_child,position2,rotation_2/*,Depth+1*/);
				if(Collides == false){Collides = RetVal;}
			}
		}
	}

	return Collides;
}

bool lpBoundingVolumeParticleCollision(	lpMesh *Mesh,lpRigidBody *Particle,liRigidBody::liState &MeshState,liRigidBody::liState &ParticleState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &ParticleDerivative,lmScalar dt,lpContactData &ContactData,
									liBoundingVolume *bounding_volume,const lmVector3D &position,const lmQuaternion &rotation)
{
	bool Collides = false;

	//lmVector3D SpherePosition = position + rotation_matrix * sphere_tree->GetPosition();

	//if(lmRaySphereIntersection(ParticleState.GetPosition(),Particle->GetInvMass() * ParticleState.GetMomentum(),SpherePosition,sphere_tree->GetRadius()))
	if(lpBoundingVolumePrimitiveCollision(bounding_volume,position,rotation,Particle,&ParticleState,dt))
	{
		if(bounding_volume->GetDataSize() != 0)
		{
			//std::cout << "Szercsy" << std::endl;
			bool RetVal = lpParticleMeshCollision(Particle,Mesh,bounding_volume,ParticleState,MeshState,ParticleDerivative,MeshDerivative,dt/*,false*/,ContactData);
			if(Collides == false){Collides = RetVal;}
		}

		for(unsigned int i=0;i < bounding_volume->GetNumChildren();i++)
		{
			liBoundingVolume *bv_child = bounding_volume->GetChild(i);
			if(bv_child == nullptr){continue;}

			bool RetVal = lpBoundingVolumeParticleCollision(Mesh,Particle,MeshState,ParticleState,MeshDerivative,ParticleDerivative,dt,ContactData,bv_child,position,rotation);
			if(Collides == false){Collides = RetVal;}
		}
	}

	return Collides;
}

bool lpBoundingVolumePlaneCollision(lpMesh *Mesh,lpPlane *Plane,liRigidBody::liState &MeshState,liRigidBody::liState &PlaneState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &PlaneDerivative,lpContactData &ContactData,
								liBoundingVolume *bounding_volume,const lmVector3D &position,const lmQuaternion &rotation)
{
	bool Collides = false;

	//lmVector3D SpherePosition = position + rotation_matrix * sphere_tree->GetPosition();
	//lmVector3D PlaneNormal = lmRotate3x3(PlaneState.GetOrientationQuaternion()) * Plane->GetNormal();

	//if(lmSpherePlaneIntersection(SpherePosition,sphere_tree->GetRadius(),PlaneState.GetPosition(),PlaneNormal))
	if(lpBoundingVolumePrimitiveCollision(bounding_volume,position,rotation,Plane,&PlaneState,0.0))
	{
		if(bounding_volume->GetDataSize() != 0)
		{
			//std::cout << "Szercsy" << std::endl;
			bool RetVal = lpMeshPlaneCollision(Mesh,Plane,bounding_volume,MeshState,PlaneState,MeshDerivative,PlaneDerivative,ContactData);
			if(Collides == false)
			{Collides = RetVal;}
		}

		for(unsigned int i=0;i < bounding_volume->GetNumChildren();i++)
		{
			liBoundingVolume *bv_child = bounding_volume->GetChild(i);
			if(bv_child == nullptr){continue;}

			bool RetVal = lpBoundingVolumePlaneCollision(Mesh,Plane,MeshState,PlaneState,MeshDerivative,PlaneDerivative,ContactData,bv_child,position,rotation);
			if(Collides == false){Collides = RetVal;}
		}
	}

	return Collides;
}

bool lpBoundingVolumeSphereCollision(lpMesh *Mesh,lpSphere *Sphere,liRigidBody::liState &MeshState,liRigidBody::liState &SphereState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &SphereDerivative,lpContactData &ContactData,
								liBoundingVolume *bounding_volume,const lmVector3D &position,const lmQuaternion &rotation)
{
	bool Collides = false;

	//lmVector3D SpherePosition = position + rotation_matrix * sphere_tree->GetPosition();

	//if(lmSphereSphereIntersection(SpherePosition,sphere_tree->GetRadius(),SphereState.GetPosition(),Sphere->GetRadius()))
	if(lpBoundingVolumePrimitiveCollision(bounding_volume,position,rotation,Sphere,&SphereState,0.0))
	{
		if(bounding_volume->GetDataSize() != 0)
		{
			//std::cout << "Szercsy" << std::endl;
			bool RetVal = lpSphereMeshCollision(Sphere,Mesh,bounding_volume,SphereState,MeshState,SphereDerivative,MeshDerivative,ContactData);
			if(Collides == false)
			{Collides = RetVal;}
		}

		for(unsigned int i=0;i < bounding_volume->GetNumChildren();i++)
		{
			liBoundingVolume *bv_child = bounding_volume->GetChild(i);
			if(bv_child == nullptr){continue;}

			bool RetVal = lpBoundingVolumeSphereCollision(Mesh,Sphere,MeshState,SphereState,MeshDerivative,SphereDerivative,ContactData,bv_child,position,rotation);
			if(Collides == false){Collides = RetVal;}
		}
	}

	return Collides;
}

bool lpDiscreteCollisionDetector::MeshMeshCollision(lpMesh *Body_i,lpMesh *Body_j,liRigidBody::liState &State_i,liRigidBody::liState &State_j,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData)
{
	liBoundingVolume *SphereTree_i;
	liBoundingVolume *SphereTree_j;

	if(Body_i->HasBoundingVolume())
	{SphereTree_i = Body_i->GetBoundingVolume();}
	else
	{SphereTree_i = nullptr;}//Body_i->GetBoundingSphere();}

	if(Body_j->HasBoundingVolume())
	{SphereTree_j = Body_j->GetBoundingVolume();}
	else
	{SphereTree_j = nullptr;}//Body_j->GetBoundingSphere();}

	return lpBoundingVolumeHierarchyCollision(Body_i,Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData,
											  SphereTree_i,State_i.GetPosition(),State_i.GetOrientationQuaternion(),
											  SphereTree_j,State_j.GetPosition(),State_j.GetOrientationQuaternion()
											  );
}

bool lpDiscreteCollisionDetector::MeshPlaneCollision(lpMesh *Mesh,lpPlane *Plane,liRigidBody::liState &MeshState,liRigidBody::liState &PlaneState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &PlaneDerivative,lpContactData &ContactData)
{
	liBoundingVolume *SphereTree;

	if(Mesh->HasBoundingVolume())
	{SphereTree = Mesh->GetBoundingVolume();}
	else
	{SphereTree = nullptr;}

	return lpBoundingVolumePlaneCollision(Mesh,Plane,MeshState,PlaneState,MeshDerivative,PlaneDerivative,ContactData,SphereTree,MeshState.GetPosition(),MeshState.GetOrientationQuaternion());
}

bool lpDiscreteCollisionDetector::MeshSphereCollision(lpMesh *Mesh,lpSphere *Sphere,liRigidBody::liState &MeshState,liRigidBody::liState &SphereState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &SphereDerivative,lpContactData &ContactData)
{
	liBoundingVolume *SphereTree;

	if(Mesh->HasBoundingVolume())
	{SphereTree = Mesh->GetBoundingVolume();}
	else
	{SphereTree = nullptr;}

	return lpBoundingVolumeSphereCollision(Mesh,Sphere,MeshState,SphereState,MeshDerivative,SphereDerivative,ContactData,SphereTree,MeshState.GetPosition(),MeshState.GetOrientationQuaternion());
}

bool lpDiscreteCollisionDetector::MeshParticleCollision(lpMesh *Mesh,lpRigidBody *Particle,liRigidBody::liState &MeshState,liRigidBody::liState &ParticleState,liRigidBody::liDerivative &MeshDerivative,liRigidBody::liDerivative &ParticleDerivative,lmScalar dt,lpContactData &ContactData)
{
	liBoundingVolume *SphereTree;

	if(Mesh->HasBoundingVolume())
	{SphereTree = Mesh->GetBoundingVolume();}
	else
	{SphereTree = nullptr;}

	return lpBoundingVolumeParticleCollision(Mesh,Particle,MeshState,ParticleState,MeshDerivative,ParticleDerivative,dt,ContactData,SphereTree,MeshState.GetPosition(),MeshState.GetOrientationQuaternion());
}

bool lpDiscreteCollisionDetector::ParticleCollision(lpRigidBody *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData)
{
    switch(Body_j->GetType())
    {
	case liRigidBody::LP_PARTICLE:
		/*
		 * A particle egy kiterjedés nélküli pont. Annak a valószínűsége, hogy két particle ütközik, nulla,
		 * mert egy particle térfogata nulla.
		 */
		return false;

	case liRigidBody::LP_SPHERE:
		return lpParticleSphereCollision(Body_i,(lpSphere *)Body_j,State_i,State_j,Derivative_i,Derivative_j,dt,ContactData);
		//std::cout << "TODO: Üzemeld be a sphere particle collision detectiont!\n";
		//return false;

	case liRigidBody::LP_PLANE:
		return lpParticlePlaneCollision(Body_i,(lpPlane *)Body_j,State_i,State_j,Derivative_i,Derivative_j,dt,ContactData);

	case liRigidBody::LP_MESH:
		//return lpParticleMeshCollision(Body_i,(lpMesh *)Body_j,State_i,State_j,Derivative_i,Derivative_j,dt,false,ContactData);
		return MeshParticleCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,dt,ContactData);

	case liRigidBody::LP_BOX:
		//return lpParticleMeshCollision(Body_i,(lpMesh *)Body_j,State_i,State_j,Derivative_i,Derivative_j,dt,false,ContactData);
		return MeshParticleCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,dt,ContactData);
    }
}

bool lpDiscreteCollisionDetector::SphereCollision(lpSphere *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData)
{
	//lpMesh *Bj_p;

    switch(Body_j->GetType())
    {
	case liRigidBody::LP_PARTICLE:
		return lpParticleSphereCollision((lpSphere *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,dt,ContactData);

	case liRigidBody::LP_SPHERE:
		return lpSphereSphereCollision(Body_i,(lpSphere *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);

	case liRigidBody::LP_PLANE:
		return lpSpherePlaneCollision(Body_i,(lpPlane  *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);

	case liRigidBody::LP_MESH:
		return MeshSphereCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,ContactData);

	case liRigidBody::LP_BOX:
		return MeshSphereCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,ContactData);
    }
}

bool lpDiscreteCollisionDetector::PlaneCollision(lpPlane *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData)
{
    switch(Body_j->GetType())
    {
	case liRigidBody::LP_PARTICLE:
		return lpParticlePlaneCollision((lpRigidBody *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,dt,ContactData);

	case liRigidBody::LP_SPHERE:
		return lpSpherePlaneCollision((lpSphere *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,ContactData);

	case liRigidBody::LP_PLANE:
		/*
		 * Na, az egy érdekes kérdés, hogy itt mit a halált kéne csinálni. :P
		 * Mi értelme egy szimulációban annak, hogy két végtelen sík ütközik? :P
		 */
		return false;

	case liRigidBody::LP_MESH:
		//return lpMeshPlaneCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,false,ContactData);
		return MeshPlaneCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,ContactData);
		//std::cout << "TODO: Be kéne üzemelni a plane mesh collision detectiont!\n";
		//return false;

	case liRigidBody::LP_BOX:
		//return lpMeshPlaneCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,false,ContactData);
		return MeshPlaneCollision((lpMesh *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,ContactData);
		//std::cout << "TODO: Be kéne üzemelni a plane box collision detectiont, ami ugyanaz, mint a meshé.\n";
		//return false;
    }
}

bool lpDiscreteCollisionDetector::MeshCollision(lpMesh *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData)
{
	//lpSphere	*pSphere_j;
	//lpMesh		*pMesh_j;

    switch(Body_j->GetType())
    {
	case liRigidBody::LP_PARTICLE:
		//return lpParticleMeshCollision(Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,dt,false,ContactData);
		return MeshParticleCollision(Body_i,(lpRigidBody *)Body_j,State_i,State_j,Derivative_i,Derivative_j,dt,ContactData);

	case liRigidBody::LP_SPHERE:
        /*
        pSphere_j = (lpSphere *)Body_j;

        if(lmSphereSphereIntersection(Body_i->GetPosition(),Body_i->GetBoundingVolume()->GetRadius(),pSphere_j->GetPosition(),pSphere_j->GetRadius(),nullptr,nullptr,nullptr))
        {
        	return lpSphereMeshCollision((lpSphere *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,false,ContactData);
        }
		return false;
		*/
		return MeshSphereCollision(Body_i,(lpSphere *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);

	case liRigidBody::LP_PLANE:
		//return lpMeshPlaneCollision(Body_i,(lpPlane *)Body_j,State_i,State_j,Derivative_i,Derivative_j,false,ContactData);
		return MeshPlaneCollision(Body_i,(lpPlane *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);
		//std::cout << "TODO: Be kéne üzemelni a plane mesh collision detectiont! 8===========D\n";
		//return false;

	case liRigidBody::LP_MESH:
        /*
        pMesh_j = (lpMesh *)Body_j;

        if(lmSphereSphereIntersection(Body_i->GetPosition(),Body_i->GetBoundingSphere()->GetRadius(),pMesh_j->GetPosition(),pMesh_j->GetBoundingSphere()->GetRadius(),nullptr,nullptr,nullptr))
        {
        	return lpMeshMeshCollision(Body_i,(lpMesh *)Body_j,Body_i->GetBoundingSphere()->GetData(),((lpMesh *)Body_j)->GetBoundingSphere()->GetData(),State_i,State_j,Derivative_i,Derivative_j,false,ContactData);
        }
		return false;
		*/
		return MeshMeshCollision(Body_i,(lpMesh *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);

	case liRigidBody::LP_BOX:
        /*
        pMesh_j = (lpMesh *)Body_j;

        if(lmSphereSphereIntersection(Body_i->GetPosition(),Body_i->GetBoundingSphere()->GetRadius(),pMesh_j->GetPosition(),pMesh_j->GetBoundingSphere()->GetRadius(),nullptr,nullptr,nullptr))
        {
        	return lpMeshMeshCollision(Body_i,(lpMesh *)Body_j,Body_i->GetBoundingSphere()->GetData(),((lpMesh *)Body_j)->GetBoundingSphere()->GetData(),State_i,State_j,Derivative_i,Derivative_j,false,ContactData);
        }
		return false;
		*/
		return MeshMeshCollision(Body_i,(lpMesh *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);
    }
}

bool lpDiscreteCollisionDetector::BoxCollision(lpBox *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,lpContactData &ContactData)
{
	//lpSphere	*pSphere_j;
	//lpMesh		*pMesh_j;

    switch(Body_j->GetType())
    {
	case liRigidBody::LP_PARTICLE:
		//return lpParticleMeshCollision(Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,dt,false,ContactData);
		return MeshParticleCollision(Body_i,(lpRigidBody *)Body_j,State_i,State_j,Derivative_i,Derivative_j,dt,ContactData);

	case liRigidBody::LP_SPHERE:
        /*
        pSphere_j = (lpSphere *)Body_j;

        if(lmSphereSphereIntersection(Body_i->GetPosition(),Body_i->GetBoundingVolume()->GetRadius(),pSphere_j->GetPosition(),pSphere_j->GetRadius(),nullptr,nullptr,nullptr))
        {
        	return lpSphereMeshCollision((lpSphere *)Body_j,Body_i,State_j,State_i,Derivative_j,Derivative_i,false,ContactData);
        }
		return false;
		*/
		return MeshSphereCollision(Body_i,(lpSphere *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);

	case liRigidBody::LP_PLANE:
		return MeshPlaneCollision(Body_i,(lpPlane *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);
		//return lpMeshPlaneCollision(Body_i,(lpPlane *)Body_j,State_i,State_j,Derivative_i,Derivative_j,false,ContactData);

	case liRigidBody::LP_MESH:
        /*
        pMesh_j = (lpMesh *)Body_j;

        if(lmSphereSphereIntersection(Body_i->GetPosition(),Body_i->GetBoundingSphere()->GetRadius(),pMesh_j->GetPosition(),pMesh_j->GetBoundingSphere()->GetRadius(),nullptr,nullptr,nullptr))
        {
        	return lpMeshMeshCollision(Body_i,(lpMesh *)Body_j,Body_i->GetBoundingSphere()->GetData(),((lpMesh *)Body_j)->GetBoundingSphere()->GetData(),State_i,State_j,Derivative_i,Derivative_j,false,ContactData);
        }
		return false;
		*/
		return MeshMeshCollision(Body_i,(lpMesh *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);

	case liRigidBody::LP_BOX:
        /*
        pMesh_j = (lpMesh *)Body_j;

        if(lmSphereSphereIntersection(Body_i->GetPosition(),Body_i->GetBoundingSphere()->GetRadius(),pMesh_j->GetPosition(),pMesh_j->GetBoundingSphere()->GetRadius(),nullptr,nullptr,nullptr))
        {
        	return lpMeshMeshCollision(Body_i,(lpMesh *)Body_j,Body_i->GetBoundingSphere()->GetData(),((lpMesh *)Body_j)->GetBoundingSphere()->GetData(),State_i,State_j,Derivative_i,Derivative_j,false,ContactData);
        }
		return false;
		*/
		return MeshMeshCollision(Body_i,(lpMesh *)Body_j,State_i,State_j,Derivative_i,Derivative_j,ContactData);
    }
}

bool lpDiscreteCollisionDetector::CheckCollision(liRigidBody *Body_i,liRigidBody::liState &State_i,liRigidBody *Body_j,liRigidBody::liState &State_j,lmScalar dt,liRigidBody::liDerivative &Derivative_i,liRigidBody::liDerivative &Derivative_j,liInteractionModel *InteractionModel)
{
    bool Collides = false;

    lpContactData ContactData;

    switch(Body_i->GetType())
    {
	case liRigidBody::LP_PARTICLE:
		Collides = ParticleCollision((lpRigidBody *)Body_i,State_i,Body_j,State_j,dt,Derivative_i,Derivative_j,ContactData);
		break;

	case liRigidBody::LP_SPHERE:
		Collides = SphereCollision((lpSphere *)Body_i,State_i,Body_j,State_j,dt,Derivative_i,Derivative_j,ContactData);
		break;

	case liRigidBody::LP_PLANE:
		Collides = PlaneCollision((lpPlane *)Body_i,State_i,Body_j,State_j,dt,Derivative_i,Derivative_j,ContactData);
		break;

	case liRigidBody::LP_MESH:
		Collides = MeshCollision((lpMesh *)Body_i,State_i,Body_j,State_j,dt,Derivative_i,Derivative_j,ContactData);
		break;

	case liRigidBody::LP_BOX:
		Collides = BoxCollision((lpBox *)Body_i,State_i,Body_j,State_j,dt,Derivative_i,Derivative_j,ContactData);
		break;
    }

    if(Collides && (!Body_i->IsGhost() && !Body_j->IsGhost()))
	{
		liInteractionModel::liContact *Contact = InteractionModel->AddContact(ContactData.Body1,ContactData.Body2,ContactData.State1,ContactData.State2,ContactData.Derivative1,ContactData.Derivative2);

		for(lpCollisionData &Data : ContactData.CollisionData)
		{
			Contact->AddContact(Data.Intersection1,Data.Intersection2,Data.Normal,Data.PenetrationDepth);
		}
	}

	unsigned int CollisionDataIndex = 0;
	for(lpCollisionData &Data : ContactData.CollisionData)
	{
		ContactData.Body1->CollisionCallback(ContactData.Body2,ContactData.State2,Data.Intersection1,CollisionDataIndex);
		ContactData.Body2->CollisionCallback(ContactData.Body1,ContactData.State1,Data.Intersection2,CollisionDataIndex);

		CollisionDataIndex++;
	}

    return Collides;
}
