#ifndef LM_QUATERNION
#define LM_QUATERNION

#include <cmath>

#include "lmMindenszar.hpp"
#include "lmVectorND.hpp"

/** \brief Egy kvaterniót definiáló osztály.
 *
 * \author Seres Lóránt Gábor
 *
 */
 template<class T>
struct lmQuaternionT
{
    T X; /**< X koordináta */
    T Y; /**< Y koordináta */
    T Z; /**< Z koordináta */
    T W; /**< W koordináta */

    /** \brief Visszaadja a kvaternió hossznégyzetét. Konstans tagfüggvény.
     *
     * \return T - a kvaternió hossznégyzete
     *
     */
    T LengthSquared() const
	{
		return X*X + Y*Y + Z*Z + W*W;
	}

    /** \brief Visszaadja a kvaternió konjugáltját. Konstans tagfüggvény.
     *
     * \return lmQuaternionT - a kvaternió konjugáltja
     *
     */
    lmQuaternionT GetInverse() const
    {
		lmQuaternionT p(-X,-Y,-Z,W);

		return p;
	}

    /** \brief Default konstruktor, amely mindegyik koordinátáját nullára állítja
     */
    lmQuaternionT()
		:X(0),Y(0),Z(0),W(0)
	{}

    /** \brief Copy konstruktor
     *
     * \param q const lmQuaternionT& - a másolandó kvaternió
     *
     */
    lmQuaternionT(const lmQuaternionT &q)
		:X(q.X),Y(q.Y),Z(q.Z),W(q.W)
	{}

    /** \brief Konstruktor
     *
     * \param x T:   Az új X koordináta
     * \param y T:   Az új Y koordináta
     * \param z T:   Az új Z koordináta
     * \param w T:   Az új W koordináta
     *
     */
    lmQuaternionT(T x,T y,T z,T w)
		:X(x),Y(y),Z(z),W(w)
	{}

    /** \brief Konstruktor
     *
     * \param V const lmVector3D &: a kvaternió tengelye
     * \param Angle T: Szög, amivel a kvaternióval és a konjugáltjával való szorzás forgat.
     *
     */
    lmQuaternionT(const lmVectorND<T,3> &V,T Angle)
	{
		W = std::cos(Angle/2.0);

		X = V[0] * std::sin(Angle/2.0);
		Y = V[1] * std::sin(Angle/2.0);
		Z = V[2] * std::sin(Angle/2.0);
	}

    /** \brief Destruktor, nem csinál semmit.
     */
    ~lmQuaternionT(){}

    /** \brief Kvaternió összeadás
     *
     * \param q const lmQuaternionT &: a hozzáadandó kvaternió
     * \return lmQuaternionT - az összeadás eredménye
     *
     */
    lmQuaternionT operator+(const lmQuaternionT &q) const
    {
		lmQuaternionT p(X+q.X,Y+q.Y,Z+q.Z,W+q.W);

		return p;
	}

    /** \brief Kvaternió kivonás
     *
     * \param q const lmQuaternionT &: a kivonandó kvaternió
     * \return lmQuaternionT - a kivonás eredménye
     *
     */
    lmQuaternionT operator-(const lmQuaternionT &q) const
	{
		lmQuaternionT p(X-q.X,Y-q.Y,Z-q.Z,W-q.W);

		return p;
	}

    /** \brief Kvaternió valós számmal való szorzása
     *
     * \param l T: ezzel a számmal szorzunk.
     * \return lmQuaternionT - a szorzás eredménye
     *
     */
    lmQuaternionT operator*(T l) const
	{
		lmQuaternionT p(X*l,Y*l,Z*l,W*l);

		return p;
	}

    /** \brief Kvaternió kvaternióval való szorzása
     *
     * \param q const lmQuaternionT &: ezzel kvaternióval szorzunk.
     * \return lmQuaternionT - a szorzás eredménye
     *
     */
    lmQuaternionT operator*(const lmQuaternionT &q) const
	{
		lmQuaternionT p(W*q.X + X*q.W + Y*q.Z - Z*q.Y,
						W*q.Y - X*q.Z + Y*q.W + Z*q.X,
						W*q.Z + X*q.Y - Y*q.X + Z*q.W,
						W*q.W - X*q.X - Y*q.Y - Z*q.Z);

		return p;
	}

    /** \brief Kvaternió összeadás és értékadás
     *
     * \param q const lmQuaternionT &: a hozzáadandó kvaternió
     * \return const lmQuaternionT & - *this
     *
     */
    const lmQuaternionT &operator+=(const lmQuaternionT &q)
	{
		X += q.X;
		Y += q.Y;
		Z += q.Z;
		W += q.W;

		return *this;
	}

    /** \brief Kvaternió kivonás és értékadás
     *
     * \param q const lmQuaternionT &: a kivonandó kvaternió
     * \return const lmQuaternionT & - *this
     *
     */
    const lmQuaternionT &operator-=(const lmQuaternionT &q)
	{
		X -= q.X;
		Y -= q.Y;
		Z -= q.Z;
		W -= q.W;

		return *this;
	}

    /** \brief Kvaternió valós számmal való szorzása és értékadás
     *
     * \param l T: ezzel a számmal szorzunk.
     * \return const lmQuaternionT & - *this
     *
     */
    const lmQuaternionT &operator*=(T l)
	{
		X *= l;
		Y *= l;
		Z *= l;
		W *= l;

		return *this;
	}

    /** \brief Kvaternió kvaternióval való szorzása és értékadás
     *
     * \param q const lmQuaternionT &: ezzel kvaternióval szorzunk.
     * \return const lmQuaternionT & - *this
     *
     */
    const lmQuaternionT &operator*=(const lmQuaternionT &q)
	{
		T x = W*q.X + X*q.W + Y*q.Z - Z*q.Y;
		T y = W*q.Y - X*q.Z + Y*q.W + Z*q.X;
		T z = W*q.Z + X*q.Y - Y*q.X + Z*q.W;
		T w = W*q.W - X*q.X - Y*q.Y - Z*q.Z;

		X = x; Y = y; Z = z; W = w;

		return *this;
	}

    /** \brief Értékadás
     *
     * \param q const lmQuaternionT &: az értékül adott kvaternió
     * \return const lmQuaternionT & - *this
     *
     */
    const lmQuaternionT &operator=(const lmQuaternionT &q)
	{
		X = q.X;
		Y = q.Y;
		Z = q.Z;
		W = q.W;

		return *this;
	}
};

/** \brief Kvaternió valós számmal való szorzása balról
 *
 * \param l T:               ezzel a számmal szorzunk.
 * \param q const lmQuaternionT &:   a beszorzandó kvaternió.
 * \return lmQuaternionT - a szorzás eredménye
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T>
lmQuaternionT<T> operator*(T l,const lmQuaternionT<T> &q)
{
    lmQuaternionT<T> p(q.X*l,q.Y*l,q.Z*l,q.W*l);

    return p;
}

template<class T>
lmQuaternionT<T> lmPow(const lmQuaternionT<T> &q,T t)
{
	T q_Angle		= acos(lmClamp((T)-1.0,(T)1.0,q.W));
	T q_SinAngle	= sin(q_Angle);

	lmVectorND<T,3> q_axis;

	if(std::abs(q_SinAngle) > 1e-8)
	{
		q_axis = {q.X / q_SinAngle,q.Y / q_SinAngle,q.Z / q_SinAngle};
	}

	/*
	 * Ha a 2.0 nincs ott a q_Angle előtt, akkor a slerp nem működik,
	 * mert a q_Angle a forgatás szögének a fele, és a slerp során
	 * a várt szög felét fogja produkálni, így az interpolációval
	 * kapott animáció szaggatni fog.
	 */
	return lmQuaternionT<T>(q_axis,(2.0 * q_Angle) * t);
}

template<class T>
lmQuaternionT<T> lmSlerp(const lmQuaternionT<T> &q0,const lmQuaternionT<T> &q1,T weight)
{
	return q0*lmPow(q0.GetInverse()*q1,weight);
	//return q1*lmPow(q1.GetInverse()*q0,1.0-weight);
	//return lmPow(q0*q1.GetInverse(),1.0-weight) * q1;
	//return lmPow(q1*q0.GetInverse(),weight) * q0;
}

template<class T>
lmVectorND<T,3> lmQuaternionRotate(const lmVectorND<T,3> &v,const lmQuaternionT<T> &q)
{
	lmQuaternionT<T> Qv(v[0],v[1],v[2],0.0);

	Qv = q*Qv*q.GetInverse();

	lmVector3D V;
	V[0] = Qv.X;
	V[1] = Qv.Y;
	V[2] = Qv.Z;

	return V;//{Qv.X,Qv.Y,Qv.Z};
}

typedef lmQuaternionT<lmScalar> lmQuaternion;

#endif
