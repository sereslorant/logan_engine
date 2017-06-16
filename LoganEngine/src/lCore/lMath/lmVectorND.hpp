#ifndef LM_VECTOR_ND_H
#define LM_VECTOR_ND_H

#include "lmTypedef.h"

#include <cmath>

#include <algorithm>
#include <initializer_list>

/** \brief Egy 3D-s vektort definiáló osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int N>
class lmVectorND
{
private:
    T X[N];

public:


    enum ConstructorMode
    {
    	NO_INIT,
        NULLVEC,
    };

    lmVectorND(const lmVectorND<T,N> &V)
	{
		for(unsigned int i=0;i < N;i++)
		{
			X[i] = V.X[i];
		}
	}

    /** \brief Default konstruktor
     *
     * Nullvektornak inicializálja a vektort.
     *
     */
    lmVectorND(ConstructorMode mode = NULLVEC)
    {
    	if(mode != NO_INIT)
		{
			for(unsigned int i=0;i < N;i++)
			{
				X[i] = 0;
			}
		}
    }

    lmVectorND(std::initializer_list<T> params)
    {
    	unsigned int i=0;
    	for(auto I = params.begin();i < std::min((unsigned int)(params.size()),N);I++)
		{
			X[i] = *I;
			i++;
		}
    }

    /** \brief Destruktor - nem csinál semmit
     *
     */
    ~lmVectorND(){}

    /** \brief Visszatér a vektor hosszának négyzetével.
     *
     * \return T - a vektor hosszának a négyzete
     *
     */
    T LengthSquared() const
	{
		T LengthSqrd = 0;

		for(unsigned int i=0;i < N;i++)
		{
			LengthSqrd += X[i]*X[i];
		}

		return LengthSqrd;
	}
	
	T Length() const
	{
		return std::sqrt(LengthSquared());
	}

    /** \brief Normálja a vektort.
     *
     * \return void
     *
     */
    void Normalize()
	{
		T Length = std::sqrt(LengthSquared());

		if(Length > 1e-6)
		{
			for(unsigned int i=0;i < N;i++)
			{
				X[i] /= Length;
			}
		}
	}

    //Operátorok

    /** \brief Két vektor összeadása
     *
     * \param U const lmVectorND &: A vektorhoz hozzáadandó vektor.
     * \return lmVectorND - az összeadás eredménye
     *
     */
    lmVectorND operator+(const lmVectorND &U) const
	{
		lmVectorND V;

		for(unsigned int i=0;i < N;i++)
		{
			V[i] = X[i] + U[i];
		}

		return V;
	}

    /** \brief Vektorból másik vektor kivonása
     *
     * \param U const lmVectorND &: A vektorból kivonandó vektor.
     * \return const lmVectorND & - *this
     *
     */
    lmVectorND operator-(const lmVectorND &U) const
	{
		lmVectorND V;

		for(unsigned int i=0;i < N;i++)
		{
			V[i] = X[i] - U[i];
		}

		return V;
	}

    /** \brief Vektor skalárral való szorzása jobbról
     *
     * \param l T: A skalár.
     * \return lmVectorND - a szorzás eredménye
     *
     */
    lmVectorND operator*(T l) const
	{
		lmVectorND V;

		for(unsigned int i=0;i < N;i++)
		{
			V[i] = X[i] * l;
		}

		return V;
	}

    /** \brief Vektorhoz másik vektor hozzáadása
     *
     * \param U const lmVectorND &: A vektorhoz hozzáadandó vektor.
     * \return const lmVectorND & - *this
     *
     */
    const lmVectorND &operator+=(const lmVectorND &U)
	{
		for(unsigned int i=0;i < N;i++)
		{
			X[i] += U[i];
		}

		return *this;
	}

    /** \brief Vektorból másik vektor kivonása
     *
     * \param U const lmVectorND &: A vektorból kivonandó vektor.
     * \return const lmVectorND & - *this
     *
     */
    const lmVectorND &operator-=(const lmVectorND &U)
	{
		for(unsigned int i=0;i < N;i++)
		{
			X[i] -= U[i];
		}

		return *this;
	}

    /** \brief Vektor beszorzása skalárral
     *
     * \param l T: A skalár.
     * \return const lmVectorND & - *this
     *
     */
    const lmVectorND &operator*=(T l)
	{
		for(unsigned int i=0;i < N;i++)
		{
			X[i] *= l;
		}

		return *this;
	}

    const lmVectorND &operator=(const lmVectorND &v)
    {
    	for(unsigned int i=0;i < N;i++)
		{
			X[i] = v[i];
		}

    	return *this;
    }

    T &operator[](unsigned int i)
    {
    	if(i < N)
		{
			return X[i];
		}
		else
		{
			return X[N-1];
			//std::cout << "EPIC FAIL!!!" << std::endl;
		}
	}

    const T &operator[](unsigned int i)const
    {
		if(i < N)
		{
			return X[i];
		}
		else
		{
			return X[N-1];
			//std::cout << "EPIC FAIL!!!" << std::endl;
		}
	}

	inline friend lmVectorND<T,N> operator *(T l,const lmVectorND<T,N> &V)
	{
		return V * l;
	}
};

template<class T,unsigned int N>
T lmDot(const lmVectorND<T,N> &U,const lmVectorND<T,N> &V)
{
	T Dot = 0;

	for(unsigned int i=0;i < N;i++)
	{
		Dot += U[i]*V[i];
	}

	return Dot;
}

typedef lmVectorND<lmScalar,2> lmVector2D;
typedef lmVectorND<lmScalar,3> lmVector3D;
typedef lmVectorND<lmScalar,4> lmVector4D;

#endif // LM_VECTOR_ND_H
