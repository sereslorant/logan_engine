#ifndef LM_GEOMETRY
#define LM_GEOMETRY

#include <cmath>

#include "lmVectorND.hpp"

/** \brief Egyenes-sík metszéspontvizsgálat
 *
 * \param RayPoint const lmVector3D &:    Az egyenes egy pontja
 * \param RayDirection const lmVector3D &:    Az egyenes irányvektora
 * \param PlanePoint const lmVector3D &:    A sík egy pontja
 * \param PlaneNormal const lmVector3D &:    A sík normálvektora
 * \param iDist lmScalar *:         Pointer: ha nem nullptr, akkor ide menti azt a d számot, amire Pr + d*Dr a sík és az egyenes metszéspontját adja.
 * \param iPoint lmVector3D *:      Pointer: ha nem nullptr, akkor ide írja be a sík és az egyenes metszéspontját.
 * \return bool - true, ha a két objektum metszi egymást.
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int N>
bool lmRayPlaneIntersection(const lmVectorND<T,N> &RayPoint,const lmVectorND<T,N> &RayDirection,const lmVectorND<T,N> &PlanePoint,const lmVectorND<T,N> &PlaneNormal,T *iDist = nullptr,lmVectorND<T,N> *iPoint = nullptr)
{
    T i = lmDot(RayDirection,PlaneNormal);
    lmVectorND<T,N> V = PlanePoint - RayPoint;

    if(std::abs(i) > 1e-8)
    {
        T Mlt = lmDot(V,PlaneNormal);/* / i;*/
        Mlt /= i;

        if(iDist != nullptr)
            {*iDist = Mlt;}

        if(iPoint != nullptr)
            {*iPoint = RayPoint + (Mlt * RayDirection);}

        return true;
    }
    else
    {
        //if(std::sqrt(V.LengthSquared()) < 1e-6)
        if(V.LengthSquared() < 1e-8)
        {
            if(iDist != nullptr)
                {*iDist = 0;}

            if(iPoint != nullptr)
                {*iPoint = lmVectorND<T,N>(lmVectorND<T,N>::NULLVEC);}

            return true;
        }

        return false;
    }
}

/** \brief Gömb-sík ütközésvizsgálat
 *
 * \param SphereCenter const lmVector3D &:	A gömb középpontja
 * \param SphereRadius const lmVector3D &:	A gömb sugara
 * \param PlanePoint const lmVector3D &:	A sík egy pontja
 * \param PlaneNormal const lmVector3D &:	A sík normálvektora
 * \param iDist lmScalar *:					Pointer: ha nem nullptr, akkor ide menti azt a d számot, amire Cs - d*Np a gömb és a sík metszéspontját adja.
 * \param iPoint lmVector3D *:				Pointer: ha nem nullptr, akkor ide írja be a sík és a sík metszéspontját.
 * \return bool - true, ha a két objektum metszi egymást.
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int N>
bool lmSpherePlaneIntersection(const lmVectorND<T,N> &SphereCenter,const T SphereRadius,const lmVectorND<T,N> &PlanePoint,const lmVectorND<T,N> &PlaneNormal,T *iDist = nullptr,lmVectorND<T,N> *iPoint = nullptr)
{
    T siDist  = 0;
    lmVectorND<T,N>	siPoint(lmVectorND<T,N>::NO_INIT);

    if(lmRayPlaneIntersection(SphereCenter,-1*PlaneNormal,PlanePoint,PlaneNormal,&siDist,&siPoint) && (abs(siDist) <= SphereRadius))
    {
        if(iDist != nullptr)
            {*iDist = siDist;}

        if(iPoint != nullptr)
            {*iPoint = siPoint;}

        return true;
    }

    if(iDist != nullptr)
        {*iDist = 0;}

    if(iPoint != nullptr)
        {*iPoint = lmVectorND<T,N>(lmVectorND<T,N>::NO_INIT);}

    return false;
}

/** \brief A három vektor által definiált háromszög területét számolja ki a Heron képlettel.
 *
 * \param A const lmVector3D &: A háromszög első csúcsa.
 * \param B const lmVector3D &: A háromszög második csúcsa.
 * \param C const lmVector3D &: A háromszög harmadik csúcsa.
 * \return lmScalar - a háromszög területe.
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int N>
T lmHeronFormula(const lmVectorND<T,N> &A,const lmVectorND<T,N> &B,const lmVectorND<T,N> &C)
{
    T Side_A = std::sqrt((A-B).LengthSquared());
    T Side_B = std::sqrt((A-C).LengthSquared());
    T Side_C = std::sqrt((B-C).LengthSquared());

    T Semiperimeter =(Side_A+Side_B+Side_C)/2;

    return std::sqrt(Semiperimeter*(Semiperimeter-Side_A)*(Semiperimeter-Side_B)*(Semiperimeter-Side_C));
}
/*
bool lmRayTriangleIntersection(const lmVector3D &P_Ray,const lmVector3D &Dir_Ray,const lmVector3D &P1_Plane,const lmVector3D &P2_Plane,const lmVector3D &P3_Plane,const lmVector3D &Nrm_Plane,lmScalar *iDist,lmVector3D *iPoint);
*/
template<class T,unsigned int N>
bool lmRayTriangleIntersection(const lmVectorND<T,N> &RayPoint,const lmVectorND<T,N> &RayDirection,const lmVectorND<T,N> &PlaneP1,const lmVectorND<T,N> &PlaneP2,const lmVectorND<T,N> &PlaneP3,const lmVectorND<T,N> &PlaneNormal,T *iDist = nullptr,lmVectorND<T,N> *iPoint = nullptr)
{
    T IntersectionDist  = 0;
    lmVectorND<T,N>	IntersectionPoint(lmVectorND<T,N>::NO_INIT);

	if(lmRayPlaneIntersection(RayPoint,RayDirection,PlaneP1,PlaneNormal,&IntersectionDist,&IntersectionPoint))
	{
		T cTriSurface = lmHeronFormula(PlaneP1,PlaneP2,PlaneP3);

		T Tri1Surface = lmHeronFormula(PlaneP1,PlaneP2,IntersectionPoint);
		T Tri2Surface = lmHeronFormula(IntersectionPoint,PlaneP2,PlaneP3);
		T Tri3Surface = lmHeronFormula(PlaneP1,IntersectionPoint,PlaneP3);

		T SumTriSurface = Tri1Surface + Tri2Surface + Tri3Surface;

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
        {*iDist = 0;}

    if(iPoint != nullptr)
        {*iPoint = lmVectorND<T,N>(lmVectorND<T,N>::NO_INIT);}

    return false;
}

//bool lmSphereTriangleIntersection(const lmVector3D &C_Sphere,const lmScalar R_Sphere,const lmVector3D &P1_Plane,const lmVector3D &P2_Plane,const lmVector3D &P3_Plane,const lmVector3D &Nrm_Plane,lmScalar *iDist,lmVector3D *iPoint);
template<class T,unsigned int N>
bool lmSphereTriangleIntersection(const lmVectorND<T,N> &SphereCenter,const T SphereRadius,const lmVectorND<T,N> &PlaneP1,const lmVectorND<T,N> &PlaneP2,const lmVectorND<T,N> &PlaneP3,const lmVectorND<T,N> &PlaneNormal,lmScalar *iDist = nullptr,lmVectorND<T,N> *iPoint = nullptr)
{
    T IntersectionDist  = 0;
    lmVectorND<T,N>	IntersectionPoint(lmVectorND<T,N>::NO_INIT);

    if(lmRayTriangleIntersection(SphereCenter,-1*PlaneNormal,PlaneP1,PlaneP2,PlaneP3,PlaneNormal,&IntersectionDist,&IntersectionPoint) && (abs(IntersectionDist) <= SphereRadius))
    {
        if(iDist != nullptr)
            {*iDist = IntersectionDist;}

        if(iPoint != nullptr)
            {*iPoint = IntersectionPoint;}

        return true;
    }

    if(iDist != nullptr)
        {*iDist = 0;}

    if(iPoint != nullptr)
        {*iPoint = lmVectorND<T,N>(lmVectorND<T,N>::NO_INIT);}

    return false;
}

template<class T,unsigned int N>
bool lmRaySphereIntersection(const lmVectorND<T,N> &ray_point,const lmVectorND<T,N> &ray_dir,const lmVectorND<T,N> &sphere_center,T sphere_radius,T *t1 = nullptr,T *t2 = nullptr)
{
	/*
	 * Az egyenes-gömb metszéspontját másodfokú egyenlet megoldóképletével számoljuk:
	 * K2*(t^2) + K1*t + K0 = 0;
	 */

	T K2 = ray_dir.LengthSquared();
	T K1 = 2.0*lmDot(ray_point-sphere_center,ray_dir);
	T K0 = ray_point.LengthSquared() + sphere_center.LengthSquared()  - 2.0*lmDot(ray_point,sphere_center) - (sphere_radius*sphere_radius);

	T Discriminant = K1*K1 - 4.0*K2*K0;

	if(Discriminant < 0.0)
	{
		return false;
	}
	if(Discriminant > 0.0)
	{
		if(t1 != nullptr)
			{*t1 = (-K1 + std::sqrt(Discriminant))/(2.0*K2);}

		if(t2 != nullptr)
			{*t2 = (-K1 - std::sqrt(Discriminant))/(2.0*K2);}

		return true;
	}
	else
	{
		if(t1 != nullptr)
			{*t1 = -K1/(2.0*K2);}
		return true;
	}
}

/** \brief Két gömb ütközésének vizsgálata
 *
 * \param C1 const lmVector3D &:    Az 1. gömb középpontja
 * \param R1 const lmScalar:        Az 1. gömb sugara
 * \param C2 const lmVector3D &:    A 2. gömb középpontja
 * \param R2 const lmScalar:        A 2. gömb sugara
 * \param iDist lmScalar *:         A két gömb középpontjai közötti távolság
 * \param iPoint1 lmVector3D *:     Az 1. gömb támadáspontja
 * \param iPoint2 lmVector3D *:     A 2. gömb támadáspontja
 * \return bool - true, ha a két objektum metszi egymást.
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int N>
bool lmSphereSphereIntersection(const lmVectorND<T,N> &Center1,const T Radius1,const lmVectorND<T,N> &Center2,const T Radius2,T *iDist = nullptr,lmVectorND<T,N> *iPoint1 = nullptr,lmVectorND<T,N> *iPoint2 = nullptr)
{
    lmVectorND<T,N>	V = Center2 - Center1;
    T Dist = /*sqrt(*/V.LengthSquared()/*)*/;

    if(Dist <= (Radius1 + Radius2)*(Radius1 + Radius2))
    {
        V.Normalize();

        if(iDist != nullptr)
        {
            *iDist = ((Center2 + (Radius2 * V)) - (Center1 - (Radius1 * V))).LengthSquared();
        }

        if(iPoint1 != nullptr)
        {
            *iPoint1 = Center1 + (Radius1 * V);
        }

        if(iPoint2 != nullptr)
        {
            *iPoint2 = Center2 - (Radius2 * V);
        }

        return true;
    }

    return false;
}

/*8=============================================================D
 *
 * Everything related to triangle triangle collision detection
 *
 *8=============================================================D
 */

#include "lmMatrixNxN.hpp"

template<class T>
T lmTriangleDeterminant3x3(const lmVectorND<T,3> &P1i,const lmVectorND<T,3> &P2i,const lmVectorND<T,3> &P3i,const lmVectorND<T,3> &Pj)
{
	lmMatrixNxN<T,3> Matrix_i(lmMatrixNxN<T,3>::NO_INIT);
	Matrix_i[0][0] = P1i[0] - Pj[0];Matrix_i[0][1] = P1i[1] - Pj[1];Matrix_i[0][2] = P1i[2] - Pj[2];
	Matrix_i[1][0] = P2i[0] - Pj[0];Matrix_i[1][1] = P2i[1] - Pj[1];Matrix_i[1][2] = P2i[2] - Pj[2];
	Matrix_i[2][0] = P3i[0] - Pj[0];Matrix_i[2][1] = P3i[1] - Pj[1];Matrix_i[2][2] = P3i[2] - Pj[2];

	return Matrix_i.Determinant();
}

template<class T>
T lmTriangleDeterminant4x4(const lmVectorND<T,3> &P1i,const lmVectorND<T,3> &P2i,const lmVectorND<T,3> &P3i,const lmVectorND<T,3> &Pj)
{
	lmMatrixNxN<T,4> Matrix_i(lmMatrixNxN<T,4>::NO_INIT);
	Matrix_i[0][0] = P1i[0];Matrix_i[0][1] = P1i[1];Matrix_i[0][2] = P1i[2];Matrix_i[0][3] = 1.0;
	Matrix_i[1][0] = P2i[0];Matrix_i[1][1] = P2i[1];Matrix_i[1][2] = P2i[2];Matrix_i[1][3] = 1.0;
	Matrix_i[2][0] = P3i[0];Matrix_i[2][1] = P3i[1];Matrix_i[2][2] = P3i[2];Matrix_i[2][3] = 1.0;

	//Checking determinant of Pj
	Matrix_i[3][0] = Pj[0];Matrix_i[3][1] = Pj[1];Matrix_i[3][2] = Pj[2];Matrix_i[3][3] = 1.0;

	return Matrix_i.Determinant();
}

enum LM_TRIANGLE_INTERSECTION
{
	LM_NO_INTERSECTION,
	LM_TRIANGLE_PLANE,
	LM_TRIANGLE_VERTEX,
};

constexpr unsigned int TCD = 3; /*Triangle collision dimension*/

template<class T>
int lmTriangleCircularPermutation(	int DetP1_sgn,int DetP2_sgn,int DetP3_sgn,
									const lmVectorND<T,TCD> &P1,const lmVectorND<T,TCD> &P2,const lmVectorND<T,TCD> &P3,
									const lmVectorND<T,TCD>* &P1_perm,const lmVectorND<T,TCD>* &P2_perm,const lmVectorND<T,TCD>* &P3_perm,
									int &P1_permSign)
{
	int P1_permIndex;

	/*TMP!!!!!!!!!!
	  Ha valamelyik determináns nulla, akkor "átkerekítjük" az egyik oldalra. :P
	  Némi gondolkodás árán érdemes lehet értelmesebb megoldást találni.
	  */
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

template<class T>
bool lmTriangleSwapVertices(const lmVectorND<T,TCD> *p2j_perm,const lmVectorND<T,TCD> *p3j_perm,int p1i_perm_sgn)
{
	if(p1i_perm_sgn == -1)
	{
		/*
		const lmVectorND<T,TCD> *Tmp = p2j_perm;
		p2j_perm = p3j_perm;
		p3j_perm = Tmp;
		*/
		lmSwap(p2j_perm,p3j_perm);

		//std::cout << "SwapPerformed" << std::endl;
		return true;
	}

	return false;
}

template<class T>
LM_TRIANGLE_INTERSECTION lmTriangleTriangleCollision(const lmVectorND<T,TCD> &P0i,const lmVectorND<T,TCD> &P1i,const lmVectorND<T,TCD> &P2i,const lmVectorND<T,TCD> &Normal_i,const lmVectorND<T,TCD> &P0j,const lmVectorND<T,TCD> &P1j,const lmVectorND<T,TCD> &P2j,const lmVectorND<T,TCD> &Normal_j,int Pi_Sgn[],int Pj_Sgn[])
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
	 */

	/*
	 * A determinánsos cuccokat lehet optimalizálni, bővebb infóért meg kell nézni a fent linkelt cikket.
	 *
	 */

	T DetP0j = lmTriangleDeterminant3x3(P0i,P1i,P2i,P0j);
	T DetP1j = lmTriangleDeterminant3x3(P0i,P1i,P2i,P1j);
	T DetP2j = lmTriangleDeterminant3x3(P0i,P1i,P2i,P2j);

	int &DetP0j_sgn = Pj_Sgn[0];
	int &DetP1j_sgn = Pj_Sgn[1];
	int &DetP2j_sgn = Pj_Sgn[2];
	/*
	DetP0j_sgn = lmSgn(DetP0j);
	DetP1j_sgn = lmSgn(DetP1j);
	DetP2j_sgn = lmSgn(DetP2j);
	*/
	DetP0j_sgn = sgn(DetP0j);
	DetP1j_sgn = sgn(DetP1j);
	DetP2j_sgn = sgn(DetP2j);

	if((DetP0j_sgn != 0 && DetP1j_sgn != 0 && DetP2j_sgn != 0) && ((DetP0j_sgn == DetP1j_sgn) && (DetP0j_sgn == DetP2j_sgn) && (DetP1j_sgn == DetP2j_sgn)))
	{
		/*
		 * Az egyik háromszög a másik háromszög síkjának az egyik oldalán van, tuti nem metszik egymást.
		 *
		 */
		return LM_NO_INTERSECTION;
	}

	if((DetP0j_sgn == 0 && DetP1j_sgn == 0 && DetP2j_sgn == 0) /*|| (DetP1i_sgn == 0 && DetP2i_sgn == 0 && DetP3i_sgn == 0)*/)
	{
		/*
		 * Ha mindhárom determináns előjele nulla (bármelyik háromszög pontjairól is legyen szó), akkor a háromszögek egy síkban vannak.
		 * Ilyenkor 2D-s háromszög metszésvizsgálatra van szükség.
		 *
		 */
		std::cout << "Bogyoro" << std::endl;

		//return LM_TRIANGLE_PLANE;

		/* !!!!!!!!!!TMP!!!!!!!!!!!!! */
		return LM_NO_INTERSECTION;
		/* !!!!!!!!!!TMP!!!!!!!!!!!!! */
	}

	T DetP0i = lmTriangleDeterminant3x3(P0j,P1j,P2j,P0i);
	T DetP1i = lmTriangleDeterminant3x3(P0j,P1j,P2j,P1i);
	T DetP2i = lmTriangleDeterminant3x3(P0j,P1j,P2j,P2i);

	int &DetP0i_sgn = Pi_Sgn[0];
	int &DetP1i_sgn = Pi_Sgn[1];
	int &DetP2i_sgn = Pi_Sgn[2];
	/*
	DetP0i_sgn = lmSgn(DetP0i);
	DetP1i_sgn = lmSgn(DetP1i);
	DetP2i_sgn = lmSgn(DetP2i);
	*/
	DetP0i_sgn = sgn(DetP0i);
	DetP1i_sgn = sgn(DetP1i);
	DetP2i_sgn = sgn(DetP2i);

	if((DetP0i_sgn != 0 && DetP1i_sgn != 0 && DetP2i_sgn != 0) && ((DetP0i_sgn == DetP1i_sgn) && (DetP0i_sgn == DetP2i_sgn) && (DetP1i_sgn == DetP2i_sgn)))
	{
		/*
		 * Az egyik háromszög a másik háromszög síkjának az egyik oldalán van, tuti nem metszik egymást.
		 *
		 */
		return LM_NO_INTERSECTION;
	}

	/*
	 * A fenti early rejectionöknek köszönhetően itt már csak az az eset fordulhat elő, amikor a két háromszög nem egy síkban vannak
	 * és metszik egymást.
	 *
	 */

	/*
	 * Most átrendezzük a vertexeket, hogy a P1i_perm adja meg azt a csúcsot, ami az egyetlen vertex a sík által definiált
	 * félterében.
	 * A másik féltérben így 2 vertex lesz, a P2i_perm és a P3i_perm.
	 *
	 */
	const lmVectorND<T,TCD> *P0i_perm = nullptr;
	const lmVectorND<T,TCD> *P1i_perm = nullptr;
	const lmVectorND<T,TCD> *P2i_perm = nullptr;
	int P0i_permSign;

	unsigned int P1i_permIndex;
	P1i_permIndex = lmTriangleCircularPermutation(DetP0i_sgn,DetP1i_sgn,DetP2i_sgn,P0i,P1i,P2i,P0i_perm,P1i_perm,P2i_perm,P0i_permSign);

	/*
	 * Most megcsináljuk a fenti átrendezést a j-edik mesh vertexeire is.
	 */
	const lmVectorND<T,TCD> *P0j_perm = nullptr;
	const lmVectorND<T,TCD> *P1j_perm = nullptr;
	const lmVectorND<T,TCD> *P2j_perm = nullptr;
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
	 */

	T iDet1 = lmTriangleDeterminant3x3(*P0i_perm,*P1i_perm,*P0j_perm,*P1j_perm);
	T iDet2 = lmTriangleDeterminant3x3(*P0i_perm,*P2i_perm,*P2j_perm,*P0j_perm);

	if((iDet1 <= 0.0) && (iDet2 <= 0.0))
	{
		return LM_TRIANGLE_VERTEX;
	}

	return LM_NO_INTERSECTION;
}

LM_TRIANGLE_INTERSECTION lmTrianglePlaneIntersection(const lmVector3D &PlanePoint,const lmVector3D &PlaneNormal,const lmVector3D &Triangle_P0,const lmVector3D &Triangle_P1,const lmVector3D &Triangle_P2,const lmVector3D &TriangleNormal,int &Dot_P0_Sgn,int &Dot_P1_Sgn,int &Dot_P2_Sgn);

#include "lmQuaternion.h"

void lmLooseAABB(/*const lmVector3D &position,*/const lmQuaternion &rotation,const lmVector3D &aabb_min,const lmVector3D &aabb_max,lmVector3D &dest_min,lmVector3D &dest_max);

bool lmCohenSutherlandLineAABBCollision(const lmVector3D &line_p0,const lmVector3D &line_p1,const lmVector3D &aabb_min,const lmVector3D &aabb_max);

bool lmPointAABBCollision(const lmVector3D &point,const lmVector3D &aabb_min,const lmVector3D &aabb_max);

bool lmAABBAABBCollision(const lmVector3D &aabb_min0,const lmVector3D &aabb_max0,const lmVector3D &aabb_min1,const lmVector3D &aabb_max1);

void lmAABBClosestPoint(const lmVector3D &point,const lmVector3D &aabb_min,const lmVector3D &aabb_max,lmVector3D *destination = nullptr);

bool lmSphereAABBCollision(const lmVector3D &sphere_center,lmScalar sphere_radius,const lmVector3D &aabb_min,const lmVector3D &aabb_max,lmVector3D *intersection = nullptr);

#endif
