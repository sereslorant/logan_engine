

// !!!!!!!!!!TMP!!!!!!!!!!!
#include <iostream>
// !!!!!!!!!!TMP!!!!!!!!!!!

#include "lmMatrix3x3.h"
#include "lmMatrix4x4.h"

#include "lmGeometry.h"

#include "lMath.h"

using namespace std;
/*
bool lmRayPlaneIntersection(const lmVector3D &P_Ray,const lmVector3D &Dir_Ray,const lmVector3D &P_Plane,const lmVector3D &Nrm_Plane,lmScalar *iDist,lmVector3D *iPoint)
{
    lmScalar i = lmDot(Dir_Ray,Nrm_Plane);
    lmVector3D V = P_Plane - P_Ray;

    if(abs(i) > 1e-6)
    {
        lmScalar Mlt = lmDot(V,Nrm_Plane);/* / i;* /
        Mlt /= i;

        if(iDist != nullptr)
            {*iDist = Mlt;}

        if(iPoint != nullptr)
            {*iPoint = P_Ray + (Mlt * Dir_Ray);}

        return true;
    }
    else
    {
        if(sqrt(V.LengthSquared()) < 1e-6)
        {
            if(iDist != nullptr)
                {*iDist = 0.0;}

            if(iPoint != nullptr)
                {*iPoint = {0.0,0.0,0.0};}

            return true;
        }

        return false;
    }
}
*//*
bool lmSpherePlaneIntersection(const lmVector3D &C_Sphere,const lmScalar R_Sphere,const lmVector3D &P_Plane,const lmVector3D &Nrm_Plane,lmScalar *iDist,lmVector3D *iPoint)
{
    lmScalar    siDist  = 0.0;
    lmVector3D  siPoint = {0.0,0.0,0.0};

    if(lmRayPlaneIntersection(C_Sphere,-1*Nrm_Plane,P_Plane,Nrm_Plane,&siDist,&siPoint) && (abs(siDist) <= R_Sphere))
    {
        if(iDist != nullptr)
            {*iDist = siDist;}

        if(iPoint != nullptr)
            {*iPoint = siPoint;}

        return true;
    }

    if(iDist != nullptr)
        {*iDist = 0.0;}

    if(iPoint != nullptr)
        {*iPoint = {0.0,0.0,0.0};}

    return false;
}
*//*
bool lmRayTriangleIntersection(const lmVector3D &P_Ray,const lmVector3D &Dir_Ray,const lmVector3D &P1_Plane,const lmVector3D &P2_Plane,const lmVector3D &P3_Plane,const lmVector3D &Nrm_Plane,lmScalar *iDist,lmVector3D *iPoint)
{
    lmScalar    IntersectionDist  = 0.0;
    lmVector3D  IntersectionPoint = {0.0,0.0,0.0};

	if(lmRayPlaneIntersection(P_Ray,Dir_Ray,P1_Plane,Nrm_Plane,&IntersectionDist,&IntersectionPoint))
	{
		lmScalar cTriSurface = lmHeronFormula(P1_Plane,P2_Plane,P3_Plane);

		lmScalar Tri1Surface = lmHeronFormula(P1_Plane,P2_Plane,IntersectionPoint);
		lmScalar Tri2Surface = lmHeronFormula(IntersectionPoint,P2_Plane,P3_Plane);
		lmScalar Tri3Surface = lmHeronFormula(P1_Plane,IntersectionPoint,P3_Plane);

		lmScalar SumTriSurface = Tri1Surface + Tri2Surface + Tri3Surface;

		if(abs(cTriSurface - SumTriSurface) < 1e-3)
		{
			if(iDist != nullptr)
				{*iDist = IntersectionDist;}

			if(iPoint != nullptr)
				{*iPoint = IntersectionPoint;}

			return true;
		}
	}

    if(iDist != nullptr)
        {*iDist = 0.0;}

    if(iPoint != nullptr)
        {*iPoint = {0.0,0.0,0.0};}

    return false;
}
*//*
bool lmSphereTriangleIntersection(const lmVector3D &C_Sphere,const lmScalar R_Sphere,const lmVector3D &P1_Plane,const lmVector3D &P2_Plane,const lmVector3D &P3_Plane,const lmVector3D &Nrm_Plane,lmScalar *iDist,lmVector3D *iPoint)
{
    lmScalar    IntersectionDist  = 0.0;
    lmVector3D  IntersectionPoint = {0.0,0.0,0.0};

    if(lmRayTriangleIntersection(C_Sphere,-1*Nrm_Plane,P1_Plane,P2_Plane,P3_Plane,Nrm_Plane,&IntersectionDist,&IntersectionPoint) && (abs(IntersectionDist) <= R_Sphere))
    {
        if(iDist != nullptr)
            {*iDist = IntersectionDist;}

        if(iPoint != nullptr)
            {*iPoint = IntersectionPoint;}

        return true;
    }

    if(iDist != nullptr)
        {*iDist = 0.0;}

    if(iPoint != nullptr)
        {*iPoint = {0.0,0.0,0.0};}

    return false;
}
*//*
bool lmRaySphereIntersection(const lmVector3D &ray_point,const lmVector3D &ray_dir,const lmVector3D &sphere_center,lmScalar sphere_radius,lmScalar *t1,lmScalar *t2)
{
	/*
	 * Az egyenes-gömb metszéspontját másodfokú egyenlet megoldóképletével számoljuk:
	 * K2*(t^2) + K1*t + K0 = 0;
	 * /

	lmScalar K2 = ray_dir.LengthSquared();
	lmScalar K1 = 2.0*lmDot(ray_point-sphere_center,ray_dir);
	lmScalar K0 = ray_point.LengthSquared() + sphere_center.LengthSquared()  - 2.0*lmDot(ray_point,sphere_center) - (sphere_radius*sphere_radius);

	lmScalar Discriminant = K1*K1 - 4.0*K2*K0;

	if(Discriminant < 0.0)
	{
		return false;
	}
	if(Discriminant > 0.0)
	{
		if(t1 != nullptr)
			{*t1 = (K1 + std::sqrt(Discriminant))/(2.0*K2);}

		if(t2 != nullptr)
			{*t2 = (K1 - std::sqrt(Discriminant))/(2.0*K2);}

		return true;
	}
	else
	{
		if(t1 != nullptr)
			{*t1 = K1/(2.0*K2);}
		return true;
	}
}
*//*
bool lmSphereSphereIntersection(const lmVector3D &C1,const lmScalar R1,const lmVector3D &C2,const lmScalar R2,lmScalar *iDist,lmVector3D *iPoint1,lmVector3D *iPoint2)
{
    lmVector3D V = C2 + (-1 * C1);
    lmScalar Dist = /*sqrt(* /V.LengthSquared()/*)* /;

    if(Dist <= (R1 + R2)*(R1 + R2))
    {
        V.Normalize();

        if(iDist != nullptr)
        {
            *iDist = ((C2 + (R2 * V))+(-1*(C1 + (-R1 * V)))).LengthSquared();
        }

        if(iPoint1 != nullptr)
        {
            *iPoint1 = C1 + (R1 * V);
        }

        if(iPoint2 != nullptr)
        {
            *iPoint2 = C2 + (-R2 * V);
        }

        return true;
    }

    return false;
}
*/

/*8=============================================================D
 *
 * Everything related to triangle triangle collision detection
 *
 *8=============================================================D
 */
/*
static int lmTriangleCircularPermutation(int DetP1_sgn,int DetP2_sgn,int DetP3_sgn,
										 const lmVector3D &P1,const lmVector3D &P2,const lmVector3D &P3,
										 const lmVector3D* &P1_perm,const lmVector3D* &P2_perm,const lmVector3D* &P3_perm,
										 int &P1_permSign)
{
	int P1_permIndex;

	/*TMP!!!!!!!!!!
	  Ha valamelyik determináns nulla, akkor "átkerekítjük" az egyik oldalra. :P
	  Némi gondolkodás árán érdemes lehet értelmesebb megoldást találni.
	  * /
	if(DetP1_sgn == 0)
	{
		if(DetP2_sgn == -1 || DetP3_sgn == -1)
			{DetP1_sgn = 1;}
		else
			{DetP1_sgn = -1;}
	}
	if(DetP2_sgn == 0)
	{
		if(DetP1_sgn == -1 || DetP3_sgn == -1)
			{DetP2_sgn = 1;}
		else
			{DetP2_sgn = -1;}
	}

	if(DetP3_sgn == 0)
	{
		if(DetP1_sgn == -1 || DetP2_sgn == -1)
			{DetP3_sgn = 1;}
		else
			{DetP3_sgn = -1;}
	}

	if((DetP1_sgn != DetP2_sgn) && (DetP2_sgn == DetP3_sgn))
	{
		P1_perm = &P1;
		P2_perm = &P3;
		P3_perm = &P2;

		P1_permIndex = 1;
		P1_permSign = DetP1_sgn;
	}
	else if((DetP2_sgn != DetP1_sgn) && (DetP1_sgn == DetP3_sgn))
	{
		P1_perm = &P2;
		P2_perm = &P1;
		P3_perm = &P3;

		P1_permIndex = 2;
		P1_permSign = DetP2_sgn;
	}
	else if((DetP3_sgn != DetP1_sgn) && (DetP1_sgn == DetP2_sgn))
	{
		P1_perm = &P3;
		P2_perm = &P2;
		P3_perm = &P1;

		P1_permIndex = 3;
		P1_permSign = DetP3_sgn;
	}

	return P1_permIndex;
}
*//*
static bool lmTriangleSwapVertices(const lmVector3D *p2j_perm,const lmVector3D *p3j_perm,int p1i_perm_sgn)
{
	if(p1i_perm_sgn == -1)
	{
		const lmVector3D *Tmp = p2j_perm;
		p2j_perm = p3j_perm;
		p3j_perm = Tmp;

		//std::cout << "SwapPerformed" << std::endl;
		return true;
	}

	return false;
}*/
/*
lmScalar lmTriangleDeterminant3x3(const lmVector3D &P1i,const lmVector3D &P2i,const lmVector3D &P3i,const lmVector3D &Pj)
{
	lmMatrix3x3 Matrix_i;
	Matrix_i[0][0] = P1i[0] - Pj[0];Matrix_i[0][1] = P1i[1] - Pj[1];Matrix_i[0][2] = P1i[2] - Pj[2];
	Matrix_i[1][0] = P2i[0] - Pj[0];Matrix_i[1][1] = P2i[1] - Pj[1];Matrix_i[1][2] = P2i[2] - Pj[2];
	Matrix_i[2][0] = P3i[0] - Pj[0];Matrix_i[2][1] = P3i[1] - Pj[1];Matrix_i[2][2] = P3i[2] - Pj[2];

	return Matrix_i.Determinant();
}
*//*
lmScalar lmTriangleDeterminant4x4(const lmVector3D &P1i,const lmVector3D &P2i,const lmVector3D &P3i,const lmVector3D &Pj)
{
	lmMatrix4x4 Matrix_i;
	Matrix_i[0][0] = P1i[0];Matrix_i[0][1] = P1i[1];Matrix_i[0][2] = P1i[2];Matrix_i[0][3] = 1.0;
	Matrix_i[1][0] = P2i[0];Matrix_i[1][1] = P2i[1];Matrix_i[1][2] = P2i[2];Matrix_i[1][3] = 1.0;
	Matrix_i[2][0] = P3i[0];Matrix_i[2][1] = P3i[1];Matrix_i[2][2] = P3i[2];Matrix_i[2][3] = 1.0;

	//Checking determinant of Pj
	Matrix_i[3][0] = Pj[0];Matrix_i[3][1] = Pj[1];Matrix_i[3][2] = Pj[2];Matrix_i[3][3] = 1.0;

	return Matrix_i.Determinant();
}
*//*
LM_TRIANGLE_INTERSECTION lmTriangleTriangleCollision(const lmVector3D &P0i,const lmVector3D &P1i,const lmVector3D &P2i,const lmVector3D &Normal_i,const lmVector3D &P0j,const lmVector3D &P1j,const lmVector3D &P2j,const lmVector3D &Normal_j,int Pi_Sgn[],int Pj_Sgn[])
{
    /*
     * Triangle-triangle collision detection.
     * Nem biztos, hogy ez a legoptimalizáltabb verziója az algoritmusnak.
     *
     * Forrás:
     * https://hal.inria.fr/inria-00072100/file/RR-4488.pdf
     *
     */

     /*
	 * Itt jön egy early rejection. Ha az egyik háromszög a másik háromszög síkjának az egyik oldalán van,
	 * akkor a két háromszög nem metszi egymást.
	 * Ezt úgy tudjuk lecsekkolnni, hogy beletesszük az egyik háromszög pontjait és a másik háromszög egy pontját
	 * egy mátrixba. Ennek a determinánsának az előjele adja meg, hogy a háromszög síkjának mely oldalán van.
	 * Ha nulla, akkor a másik háromszög pontja az egyik háromszög síkján van.
	 * Az nem érdekel minket, hogy konkrétan a háromszög melyik oldalán van, csak az, hogy mindhárom pont ugyanazon
	 * az oldalon van-e.
	 *
	 * /

	/*
	 * A determinánsos cuccokat lehet optimalizálni, bővebb infóért meg kell nézni a fent linkelt cikket.
	 *
	 * /

	//lmMatrix4x4 Matrix_i;
	//Matrix_i[0][0] = P0i.X;Matrix_i[0][1] = P0i.Y;Matrix_i[0][2] = P0i.Z;Matrix_i[0][3] = 1.0;
	//Matrix_i[1][0] = P1i.X;Matrix_i[1][1] = P1i.Y;Matrix_i[1][2] = P1i.Z;Matrix_i[1][3] = 1.0;
	//Matrix_i[2][0] = P2i.X;Matrix_i[2][1] = P2i.Y;Matrix_i[2][2] = P2i.Z;Matrix_i[2][3] = 1.0;

	//Checking determinant of P1j
	//Matrix_i[3][0] = P0j.X;Matrix_i[3][1] = P0j.Y;Matrix_i[3][2] = P0j.Z;Matrix_i[3][3] = 1.0;
	//lmScalar DetP0j = Matrix_i.Determinant();
	lmScalar DetP0j = lmTriangleDeterminant3x3(P0i,P1i,P2i,P0j);

	//Checking determinant of P2j
	//Matrix_i[3][0] = P1j.X;Matrix_i[3][1] = P1j.Y;Matrix_i[3][2] = P1j.Z;Matrix_i[3][3] = 1.0;
	//lmScalar DetP1j = Matrix_i.Determinant();
	lmScalar DetP1j = lmTriangleDeterminant3x3(P0i,P1i,P2i,P1j);

	//Checking determinant of P3j
	//Matrix_i[3][0] = P2j.X;Matrix_i[3][1] = P2j.Y;Matrix_i[3][2] = P2j.Z;Matrix_i[3][3] = 1.0;
	//lmScalar DetP2j = Matrix_i.Determinant();
	lmScalar DetP2j = lmTriangleDeterminant3x3(P0i,P1i,P2i,P2j);

	int &DetP0j_sgn = Pj_Sgn[0];
	int &DetP1j_sgn = Pj_Sgn[1];
	int &DetP2j_sgn = Pj_Sgn[2];
	/*
	DetP0j_sgn = lmSgn(DetP0j);
	DetP1j_sgn = lmSgn(DetP1j);
	DetP2j_sgn = lmSgn(DetP2j);
	* /
	DetP0j_sgn = sgn(DetP0j);
	DetP1j_sgn = sgn(DetP1j);
	DetP2j_sgn = sgn(DetP2j);

	if((DetP0j_sgn != 0 && DetP1j_sgn != 0 && DetP2j_sgn != 0) && ((DetP0j_sgn == DetP1j_sgn) && (DetP0j_sgn == DetP2j_sgn) && (DetP1j_sgn == DetP2j_sgn)))
	{
		/*
		 * Az egyik háromszög a másik háromszög síkjának az egyik oldalán van, tuti nem metszik egymást.
		 *
		 * /
		return LM_NO_INTERSECTION;
	}

	if((DetP0j_sgn == 0 && DetP1j_sgn == 0 && DetP2j_sgn == 0) /*|| (DetP1i_sgn == 0 && DetP2i_sgn == 0 && DetP3i_sgn == 0)* /)
	{
		/*
		 * Ha mindhárom determináns előjele nulla (bármelyik háromszög pontjairól is legyen szó), akkor a háromszögek egy síkban vannak.
		 * Ilyenkor 2D-s háromszög metszésvizsgálatra van szükség.
		 *
		 * /
		std::cout << "Bogyoro" << std::endl;

		//return LM_TRIANGLE_PLANE;

		/* !!!!!!!!!!TMP!!!!!!!!!!!!! * /
		return LM_NO_INTERSECTION;
		/* !!!!!!!!!!TMP!!!!!!!!!!!!! * /
	}

	//lmMatrix4x4 Matrix_j;
	//Matrix_j[0][0] = P1j.X;Matrix_j[0][1] = P1j.Y;Matrix_j[0][2] = P1j.Z;Matrix_j[0][3] = 1.0;
	//Matrix_j[1][0] = P2j.X;Matrix_j[1][1] = P2j.Y;Matrix_j[1][2] = P2j.Z;Matrix_j[1][3] = 1.0;
	//Matrix_j[2][0] = P3j.X;Matrix_j[2][1] = P3j.Y;Matrix_j[2][2] = P3j.Z;Matrix_j[2][3] = 1.0;

	//Checking determinant of P1i
	//Matrix_j[3][0] = P1i.X;Matrix_j[3][1] = P1i.Y;Matrix_j[3][2] = P1i.Z;Matrix_j[3][3] = 1.0;
	//lmScalar DetP1i = Matrix_j.Determinant();
	lmScalar DetP0i = lmTriangleDeterminant3x3(P0j,P1j,P2j,P0i);

	//Checking determinant of P2i
	//Matrix_j[3][0] = P2i.X;Matrix_j[3][1] = P2i.Y;Matrix_j[3][2] = P2i.Z;Matrix_j[3][3] = 1.0;
	//lmScalar DetP2i = Matrix_j.Determinant();
	lmScalar DetP1i = lmTriangleDeterminant3x3(P0j,P1j,P2j,P1i);

	//Checking determinant of P3i
	//Matrix_j[3][0] = P3i.X;Matrix_j[3][1] = P3i.Y;Matrix_j[3][2] = P3i.Z;Matrix_j[3][3] = 1.0;
	//lmScalar DetP3i = Matrix_j.Determinant();
	lmScalar DetP2i = lmTriangleDeterminant3x3(P0j,P1j,P2j,P2i);

	int &DetP0i_sgn = Pi_Sgn[0];
	int &DetP1i_sgn = Pi_Sgn[1];
	int &DetP2i_sgn = Pi_Sgn[2];
	/*
	DetP0i_sgn = lmSgn(DetP0i);
	DetP1i_sgn = lmSgn(DetP1i);
	DetP2i_sgn = lmSgn(DetP2i);
	* /
	DetP0i_sgn = sgn(DetP0i);
	DetP1i_sgn = sgn(DetP1i);
	DetP2i_sgn = sgn(DetP2i);

	if((DetP0i_sgn != 0 && DetP1i_sgn != 0 && DetP2i_sgn != 0) && ((DetP0i_sgn == DetP1i_sgn) && (DetP0i_sgn == DetP2i_sgn) && (DetP1i_sgn == DetP2i_sgn)))
	{
		/*
		 * Az egyik háromszög a másik háromszög síkjának az egyik oldalán van, tuti nem metszik egymást.
		 *
		 * /
		return LM_NO_INTERSECTION;
	}


	/*
	 * A fenti early rejectionöknek köszönhetően itt már csak az az eset fordulhat elő, amikor a két háromszög nem egy síkban vannak
	 * és metszik egymást.
	 *
	 * /

	/*
	 * Most átrendezzük a vertexeket, hogy a P1i_perm adja meg azt a csúcsot, ami az egyetlen vertex a sík által definiált
	 * félterében.
	 * A másik féltérben így 2 vertex lesz, a P2i_perm és a P3i_perm.
	 *
	 * /
	const lmVector3D *P0i_perm = nullptr;
	const lmVector3D *P1i_perm = nullptr;
	const lmVector3D *P2i_perm = nullptr;
	int P0i_permSign;

	unsigned int P1i_permIndex;
	P1i_permIndex = lmTriangleCircularPermutation(DetP0i_sgn,DetP1i_sgn,DetP2i_sgn,P0i,P1i,P2i,P0i_perm,P1i_perm,P2i_perm,P0i_permSign);

	/*
	 * Most megcsináljuk a fenti átrendezést a j-edik mesh vertexeire is.
	 * /
	const lmVector3D *P0j_perm = nullptr;
	const lmVector3D *P1j_perm = nullptr;
	const lmVector3D *P2j_perm = nullptr;
	int P0j_permSign;

	unsigned int P1j_permIndex;
	P1j_permIndex = lmTriangleCircularPermutation(DetP0j_sgn,DetP1j_sgn,DetP2j_sgn,P0j,P1j,P2j,P0j_perm,P1j_perm,P2j_perm,P0j_permSign);

	lmTriangleSwapVertices(P1i_perm,P2i_perm,P0j_permSign);
	lmTriangleSwapVertices(P1j_perm,P2j_perm,P0i_permSign);

	/*
	 * Az early rejectionöknek köszönhetően a P1i_perm-ből a másik két Pi vertexbe húzott szakasz garantáltan metszi a
	 * Pj-k által meghatározott háromszög síkját.
	 * A P1j_perm-ből a másik két Pj vertexbe húzott egyenesek is metszik a Pi-k által meghatározott háromszögek síkját.
	 *
	 * Ezek a metszéspontok a két sík metszésvonalán helyezkednek el. A fenti cikk szerint ezek alapján csak néhány
	 * determináns előjelének a lecsekkolása kell, ha meg akarjuk tudni, hogy a két háromszög ütközik-e.
	 *
	 * /
	//lmMatrix4x4 iMatrix;

	//iMatrix[0][0] = P1i_perm->X;iMatrix[0][1] = P1i_perm->Y;iMatrix[0][2] = P1i_perm->Z;iMatrix[0][3] = 1.0;
	//iMatrix[1][0] = P2i_perm->X;iMatrix[1][1] = P2i_perm->Y;iMatrix[1][2] = P2i_perm->Z;iMatrix[1][3] = 1.0;
	//iMatrix[2][0] = P1j_perm->X;iMatrix[2][1] = P1j_perm->Y;iMatrix[2][2] = P1j_perm->Z;iMatrix[2][3] = 1.0;
	//iMatrix[3][0] = P2j_perm->X;iMatrix[3][1] = P2j_perm->Y;iMatrix[3][2] = P2j_perm->Z;iMatrix[3][3] = 1.0;
	//lmScalar iDet1 = iMatrix.Determinant();
	lmScalar iDet1 = lmTriangleDeterminant3x3(*P0i_perm,*P1i_perm,*P0j_perm,*P1j_perm);

	//iMatrix[0][0] = P1i_perm->X;iMatrix[0][1] = P1i_perm->Y;iMatrix[0][2] = P1i_perm->Z;iMatrix[0][3] = 1.0;
	//iMatrix[1][0] = P3i_perm->X;iMatrix[1][1] = P3i_perm->Y;iMatrix[1][2] = P3i_perm->Z;iMatrix[1][3] = 1.0;
	//iMatrix[2][0] = P3j_perm->X;iMatrix[2][1] = P3j_perm->Y;iMatrix[2][2] = P3j_perm->Z;iMatrix[2][3] = 1.0;
	//iMatrix[3][0] = P1j_perm->X;iMatrix[3][1] = P1j_perm->Y;iMatrix[3][2] = P1j_perm->Z;iMatrix[3][3] = 1.0;
	//lmScalar iDet2 = iMatrix.Determinant();
	lmScalar iDet2 = lmTriangleDeterminant3x3(*P0i_perm,*P2i_perm,*P2j_perm,*P0j_perm);

	if((iDet1 <= 0.0) && (iDet2 <= 0.0))
	{
		return LM_TRIANGLE_VERTEX;
	}

	return LM_NO_INTERSECTION;
}
*/
LM_TRIANGLE_INTERSECTION lmTrianglePlaneIntersection(const lmVector3D &PlanePoint,const lmVector3D &PlaneNormal,
								 const lmVector3D &Triangle_P0,const lmVector3D &Triangle_P1,const lmVector3D &Triangle_P2,
								 const lmVector3D &TriangleNormal,int &Dot_P0_Sgn,int &Dot_P1_Sgn,int &Dot_P2_Sgn)
{
	lmScalar Dot = lmDot(PlaneNormal,TriangleNormal);

	if(std::abs(Dot) < 1e-4)
	{
		if(std::abs(lmDot(Triangle_P0 - PlanePoint,PlaneNormal)) < 1e-4)
		{
			return LM_TRIANGLE_PLANE;
		}

		return LM_NO_INTERSECTION;
	}
	else
	{
		lmScalar Dot_P0 = lmDot(Triangle_P0 - PlanePoint,PlaneNormal);
		lmScalar Dot_P1 = lmDot(Triangle_P1 - PlanePoint,PlaneNormal);
		lmScalar Dot_P2 = lmDot(Triangle_P2 - PlanePoint,PlaneNormal);
		/*
		Dot_P0_Sgn = lmSgn(Dot_P0);
		Dot_P1_Sgn = lmSgn(Dot_P1);
		Dot_P2_Sgn = lmSgn(Dot_P2);
		*/
		Dot_P0_Sgn = sgn(Dot_P0);
		Dot_P1_Sgn = sgn(Dot_P1);
		Dot_P2_Sgn = sgn(Dot_P2);

		if((Dot_P0_Sgn == Dot_P1_Sgn) && (Dot_P0_Sgn == Dot_P2_Sgn))
		{
			return LM_NO_INTERSECTION;
		}
		else
		{
			return LM_TRIANGLE_VERTEX;
		}
	}
}



void lmLooseAABB(/*const lmVector3D &position,*/const lmQuaternion &rotation,const lmVector3D &aabb_min,const lmVector3D &aabb_max,lmVector3D &dest_min,lmVector3D &dest_max)
{
	lmVector3D Center = (aabb_max + aabb_min) * (1.0/2.0);

	lmVector3D RotatedMin = lmQuaternionRotate((aabb_min - Center),rotation) + Center;
	lmVector3D RotatedMax = lmQuaternionRotate((aabb_max - Center),rotation) + Center;

	dest_min[0] = std::min(RotatedMin[0],RotatedMax[0]);
	dest_min[1] = std::min(RotatedMin[1],RotatedMax[1]);
	dest_min[2] = std::min(RotatedMin[2],RotatedMax[2]);

	dest_max[0] = std::max(RotatedMin[0],RotatedMax[0]);
	dest_max[1] = std::max(RotatedMin[1],RotatedMax[1]);
	dest_max[2] = std::max(RotatedMin[2],RotatedMax[2]);
}

const unsigned int CS_ABOVE	 = 32;
const unsigned int CS_BELOW  = 16;
const unsigned int CS_LEFT	 = 8;
const unsigned int CS_RIGHT	 = 4;
const unsigned int CS_AHEAD	 = 2;
const unsigned int CS_BEHIND = 1;

bool lmCohenSutherlandPointAABBPosition(const lmVector3D &point,const lmVector3D &aabb_min,const lmVector3D &aabb_max,unsigned int *flags = nullptr)
{
	if(flags != nullptr)
	{
		unsigned int &Flags = *flags;
		Flags = 0;

		if(point[0] < aabb_min[0])
			{Flags |= CS_LEFT;}
		if(point[0] > aabb_max[0])
			{Flags |= CS_RIGHT;}
		if(point[1] < aabb_min[1])
			{Flags |= CS_BELOW;}
		if(point[1] > aabb_max[1])
			{Flags |= CS_ABOVE;}
		if(point[2] < aabb_min[2])
			{Flags |= CS_AHEAD;}
		if(point[2] > aabb_max[2])
			{Flags |= CS_BEHIND;}
	}
}

bool lmCohenSutherlandLineAABBCollision(const lmVector3D &line_p0,const lmVector3D &line_p1,const lmVector3D &aabb_min,const lmVector3D &aabb_max)
{
	unsigned int P0Flags = 0x0;
	unsigned int P1Flags = 0x0;

	lmCohenSutherlandPointAABBPosition(line_p0,aabb_min,aabb_max,&P0Flags);
	lmCohenSutherlandPointAABBPosition(line_p1,aabb_min,aabb_max,&P1Flags);

	if(!(P0Flags | P1Flags))
		{return true;}
	else if(P0Flags & P1Flags)
		{return false;}

	lmVector3D P0 = line_p0;
	lmVector3D P1 = line_p1;

	while(true)
	{
		if(!(P0Flags | P1Flags))
		{
			return true;
		}
		else if(P0Flags & P1Flags)
		{
			return false;
		}
		else
		{
			int NewFlags = P0Flags ? P0Flags : P1Flags;


			lmScalar x,y,z;

			if (NewFlags & CS_ABOVE)
			{
				x = P0[0] + (P1[0] - P0[0]) * (aabb_max[1] - P0[1]) / (P1[1] - P0[1]);
				y = aabb_max[1];
				z = P0[2] + (P1[2] - P0[2]) * (aabb_max[1] - P0[1]) / (P1[1] - P0[1]);
			}
			else if (NewFlags & CS_BELOW)
			{
				x = P0[0] + (P1[0] - P0[0]) * (aabb_min[1] - P0[1]) / (P1[1] - P0[1]);
				y = aabb_min[1];
				z = P0[2] + (P1[2] - P0[2]) * (aabb_min[1] - P0[1]) / (P1[1] - P0[1]);
			}
			else if (NewFlags & CS_RIGHT)
			{
				x = aabb_max[0];
				y = P0[1] + (P1[1] - P0[1]) * (aabb_max[0] - P0[0]) / (P1[0] - P0[0]);
				z = P0[2] + (P1[2] - P0[2]) * (aabb_max[0] - P0[0]) / (P1[0] - P0[0]);
			}
			else if (NewFlags & CS_LEFT)
			{
				x = aabb_min[0];
				y = P0[1] + (P1[1] - P0[1]) * (aabb_min[0] - P0[0]) / (P1[0] - P0[0]);
				z = P0[2] + (P1[2] - P0[2]) * (aabb_min[0] - P0[0]) / (P1[0] - P0[0]);

			}
			else if (NewFlags & CS_AHEAD)
			{
				x = P0[0] + (P1[0] - P0[0]) * (aabb_min[2] - P0[2]) / (P1[2] - P0[2]);
				y = P0[1] + (P1[1] - P0[1]) * (aabb_min[2] - P0[2]) / (P1[2] - P0[2]);
				z = aabb_min[2];
			}
			else if (NewFlags & CS_BEHIND)
			{
				x = P0[0] + (P1[0] - P0[0]) * (aabb_max[2] - P0[2]) / (P1[2] - P0[2]);
				y = P0[1] + (P1[1] - P0[1]) * (aabb_max[2] - P0[2]) / (P1[2] - P0[2]);
				z = aabb_max[2];
			}

			if (NewFlags == P0Flags)
			{
				P0[0] = x;
				P0[1] = y;
				P0[2] = z;

				lmCohenSutherlandPointAABBPosition(P0,aabb_min,aabb_max,&P0Flags);
			}
			else
			{
				P1[0] = x;
				P1[1] = y;
				P1[2] = z;

				lmCohenSutherlandPointAABBPosition(P1,aabb_min,aabb_max,&P1Flags);
			}
		}
	}
}

bool lmPointAABBCollision(const lmVector3D &point,const lmVector3D &aabb_min,const lmVector3D &aabb_max)
{
	if(point[0] < aabb_min[0])
		{return false;}
	if(point[0] > aabb_max[0])
		{return false;}
	if(point[1] < aabb_min[1])
		{return false;}
	if(point[1] > aabb_max[1])
		{return false;}
	if(point[2] < aabb_min[2])
		{return false;}
	if(point[2] > aabb_max[2])
		{return false;}

	return true;
}

bool lmAABBAABBCollision(const lmVector3D &aabb_min0,const lmVector3D &aabb_max0,const lmVector3D &aabb_min1,const lmVector3D &aabb_max1)
{
	if(aabb_max0[0] < aabb_min1[0])
		{return false;}
	if(aabb_min0[0] > aabb_max1[0])
		{return false;}
	if(aabb_max0[1] < aabb_min1[1])
		{return false;}
	if(aabb_min0[1] > aabb_max1[1])
		{return false;}
	if(aabb_max0[2] < aabb_min1[2])
		{return false;}
	if(aabb_min0[2] > aabb_max1[2])
		{return false;}

	return true;
}


void lmAABBClosestPoint(const lmVector3D &point,const lmVector3D &aabb_min,const lmVector3D &aabb_max,lmVector3D &destination)
{
	if(point[0] < aabb_min[0])
	{destination[0] = aabb_min[0];}
	else if(point[0] > aabb_max[0])
	{destination[0] = aabb_max[0];}
	else
	{destination[0] = point[0];}

	if(point[1] < aabb_min[1])
	{destination[1] = aabb_min[1];}
	else if(point[1] > aabb_max[1])
	{destination[1] = aabb_max[1];}
	else
	{destination[1] = point[1];}

	if(point[2] < aabb_min[2])
	{destination[2] = aabb_min[2];}
	else if(point[2] > aabb_max[2])
	{destination[2] = aabb_max[2];}
	else
	{destination[2] = point[2];}
}

bool lmSphereAABBCollision(const lmVector3D &sphere_center,lmScalar sphere_radius,const lmVector3D &aabb_min,const lmVector3D &aabb_max,lmVector3D *intersection)
{
	lmVector3D SphereMin = sphere_center;
	SphereMin[0] -= sphere_radius/2.0;
	SphereMin[1] -= sphere_radius/2.0;
	SphereMin[2] -= sphere_radius/2.0;

	lmVector3D SphereMax = sphere_center;
	SphereMax[0] += sphere_radius/2.0;
	SphereMax[1] += sphere_radius/2.0;
	SphereMax[2] += sphere_radius/2.0;

	if(lmPointAABBCollision(sphere_center,aabb_min,aabb_max))
	{
		return true;
	}
	else
	{
		lmVector3D Closest;
		lmAABBClosestPoint(sphere_center,aabb_min,aabb_max,Closest);

		if((Closest-sphere_center).LengthSquared() < (sphere_radius * sphere_radius))
		{
			if(intersection != nullptr)
			{
				*intersection = Closest;
			}

			return true;
		}

		return false;
	}
}

