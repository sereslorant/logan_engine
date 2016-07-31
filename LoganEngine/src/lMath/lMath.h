#ifndef L_MATH_H
#define L_MATH_H

#include <cmath>

#define PI 3.14159265359


#include "lmVectorND.hpp"
#include "lmVector3D.hpp"

#include "lmMatrixNxN.hpp"
#include "lmMatrix3x3.h"
#include "lmMatrix4x4.h"

#include "lmQuaternion.h"

#include "lmGeometry.h"

#include "lmMindenszar.hpp"

template<class Type_T,class Container_T>
class lmSymmetricTable
{
private:
	unsigned int Size;
	Container_T Container;
	//
public:
	//
	template<class StlContainer_T>
	class lmStlAdapter
	{
	private:
		StlContainer_T StlContainer;
		//
	public:
		//
		void Resize(unsigned int new_size)
		{
			StlContainer.resize(new_size);
		}
		//
		Type_T &operator[](unsigned int index)
		{
			return StlContainer[index];
		}
		//
		lmStlAdapter(unsigned int new_size)
			:StlContainer(new_size)
		{}
		//
		~lmStlAdapter()
		{}
	};
	//
	static unsigned int IndexSymmetricTable(unsigned int size,unsigned int id_1,unsigned int id_2)
	{
		if(id_1 == id_2)
		{
			return 0;
		}
		//
		if(id_1 > id_2)
		{
			unsigned int Tmp = id_1;
			id_1 = id_2;
			id_2 = Tmp;
		}
		//
		unsigned int Index = (id_1)*(size - 1) - ((id_1)*(id_1 - 1))/2 + (id_2 - id_1 - 1);
		return Index;
	}
	//
	unsigned int GetSize()
	{
		return Size;
	}
	//
	void Resize(unsigned int new_size)
	{
		Size = new_size;
		Container.Resize(Size*(Size-1)/2);
	}
	//
	Type_T &GetElement(unsigned int id_1,unsigned int id_2)
	{
		return Container[IndexSymmetricTable(Size,id_1,id_2)];
	}
	//
	lmSymmetricTable(unsigned int size = 0)
		:Size(size),Container(Size*(Size-1)/2)
	{}
	//
	~lmSymmetricTable()
	{}
};

template<typename Type_T,unsigned int Dim_T>
class liCurve
{
public:

	virtual lmVectorND<Type_T,Dim_T> GetPoint(Type_T t) const = 0;
	virtual lmVectorND<Type_T,Dim_T> GetDerivative(Type_T t) const = 0;
	virtual lmVectorND<Type_T,Dim_T> GetSecondDerivative(Type_T t) const = 0;

	liCurve(){}
	virtual ~liCurve(){}
};

template<typename Type_T>
void lmGetFrenetFrame(const liCurve<Type_T,3> &curve,Type_T t,lmVectorND<Type_T,3> *i,lmVectorND<Type_T,3> *j,lmVectorND<Type_T,3> *k,bool normalize = false)
{
	lmVectorND<Type_T,3> SecondDerivative = curve.GetSecondDerivative(t);
	//
	*j = curve.GetDerivative(t);
	*i = lmCross<Type_T,3>(SecondDerivative,*j);
	*k = lmCross<Type_T,3>(*i,*j);
	//
	if(normalize)
	{
		i->Normalize();
		j->Normalize();
		k->Normalize();
	}
}
/*
template<typename Type_T>
static void lmGetExplicitUpFrame(const liCurve<Type_T,3> &curve,const glm::vec3 &up,float t,float dt,glm::vec3 *i,glm::vec3 *j,glm::vec3 *k,bool normalize = false)
{
	if(normalize)
	{
		*j = glm::normalize(curve.GetDerivative(t));
		if(glm::length(*j) < 1e-2)
		{
			*j = glm::normalize(curve.GetSegment(t,dt));
		}
		*i = glm::normalize(glm::cross(up,*j));
		*k = glm::normalize(glm::cross(*i,*j));
	}
	else
	{
		*j = curve.GetDerivative(t);
		*i = glm::cross(up,*j);
		*k = glm::cross(*i,*j);
	}
}
*/
template<typename Type_T,unsigned int Dim_T>
lmVectorND<Type_T,Dim_T> Hermite(const lmVectorND<Type_T,Dim_T> &p0,const lmVectorND<Type_T,Dim_T> &p1,const lmVectorND<Type_T,Dim_T> &v0,const lmVectorND<Type_T,Dim_T> &v1,Type_T t0,Type_T t1,Type_T t)
{
	lmVectorND<Type_T,Dim_T> a0 = p0;
	lmVectorND<Type_T,Dim_T> a1 = v0;
	lmVectorND<Type_T,Dim_T> a2 = (3.0f*(p1-p0)/((float)pow(t1-t0,2))) - ((v1+2.0f*v0)/((float)(t1-t0)));
	lmVectorND<Type_T,Dim_T> a3 = (2.0f*(p0-p1)/((float)pow(t1-t0,3))) + ((v1+v0)/((float)pow(t1-t0,2)));

	return a0 + a1*(t-t0) + a2*((float)pow(t-t0,2)) + a3*((float)pow(t-t0,3));
}

template<typename Type_T,unsigned int Dim_T>
lmVectorND<Type_T,Dim_T> HermiteDerivative(const lmVectorND<Type_T,Dim_T> &p0,const lmVectorND<Type_T,Dim_T> &p1,const lmVectorND<Type_T,Dim_T> &v0,const lmVectorND<Type_T,Dim_T> &v1,Type_T t0,Type_T t1,Type_T t)
{
	lmVectorND<Type_T,Dim_T> a1 = v0;
	lmVectorND<Type_T,Dim_T> a2 = (3.0f*(p1-p0)/((float)pow(t1-t0,2))) - ((v1+2.0f*v0)/((float)(t1-t0)));
	lmVectorND<Type_T,Dim_T> a3 = (2.0f*(p0-p1)/((float)pow(t1-t0,3))) + ((v1+v0)/((float)pow(t1-t0,2)));

	return a1 + 2.0f*a2*(t-t0) + 3.0f*a3*((float)pow(t-t0,2));
}

template<typename Type_T,unsigned int Dim_T>
lmVectorND<Type_T,Dim_T> HermiteSecondDerivative(const lmVectorND<Type_T,Dim_T> &p0,const lmVectorND<Type_T,Dim_T> &p1,const lmVectorND<Type_T,Dim_T> &v0,const lmVectorND<Type_T,Dim_T> &v1,Type_T t0,Type_T t1,Type_T t)
{
	lmVectorND<Type_T,Dim_T> a2 = (3.0f*(p1-p0)/((float)pow(t1-t0,2))) - ((v1+2.0f*v0)/((float)(t1-t0)));
	lmVectorND<Type_T,Dim_T> a3 = (2.0f*(p0-p1)/((float)pow(t1-t0,3))) + ((v1+v0)/((float)pow(t1-t0,2)));

	return 2.0f*a2 + 6.0f*a3*(t-t0);
}

template<typename Type_T,unsigned int Dim_T>
lmVectorND<Type_T,Dim_T> AverageVelocity(const lmVectorND<Type_T,Dim_T> &p0,const lmVectorND<Type_T,Dim_T> &p1,float t0,float t1)
{
	return (p1-p0)/(t1-t0);
}

template<typename Type_T,unsigned int Dim_T>
class lmCatmullRomSpline : public liCurve<Type_T,Dim_T>
{
private:
	std::vector<lmVectorND<Type_T,Dim_T> > cps;	// control points
	std::vector<lmVectorND<Type_T,Dim_T> > Velocity; //A kontroll pontok sebességvektora
	std::vector<Type_T>  ts;	// parameter (knot) values
	//
	lmVectorND<Type_T,Dim_T> InitVelocity;
	lmVectorND<Type_T,Dim_T> EndVelocity;
	//
public:
	//
	void SetInitVelocity(const lmVectorND<Type_T,Dim_T> &init_velocity)
	{
		InitVelocity = init_velocity;
		if(Velocity.size() > 0)
		{
			Velocity[0] = InitVelocity;
			//TODO: JAVÍTANI KELL!
		}
	}
	//
	void SetEndVelocity(const lmVectorND<Type_T,Dim_T> &end_velocity)
	{
		EndVelocity = end_velocity;
		if(Velocity.size() > 0)
		{
			Velocity[Velocity.size()-1] = EndVelocity;
			//TODO: JAVÍTANI KELL!
		}
	}
	//
	void AddControlPoint(const lmVectorND<Type_T,Dim_T> &cp,float t)
	{
		cps.push_back(cp);
		ts.push_back(t);
		Velocity.push_back(EndVelocity);

		if(cps.size() == 1)
		{
			Velocity[0] = InitVelocity;
		}
		else if(cps.size() > 2)
		{
			const lmVectorND<Type_T,Dim_T> &Pprev2 = cps[cps.size() - 3]; Type_T tprev2 = ts[ts.size() - 3];
			const lmVectorND<Type_T,Dim_T> &Pprev1 = cps[cps.size() - 2]; Type_T tprev1 = ts[ts.size() - 2];
			const lmVectorND<Type_T,Dim_T> &Pfin   = cps[cps.size() - 1]; Type_T tfin   = ts[ts.size() - 1];

			Velocity[Velocity.size()-2] = (AverageVelocity(Pprev2,Pprev1,tprev2,tprev1) + AverageVelocity(Pprev1,Pfin,tprev1,tfin)) / 2.0f;
		}
	}
	//
	virtual lmVectorND<Type_T,Dim_T> GetPoint(float t) const override
	{
		lmVectorND<Type_T,Dim_T> RetVal;
		//
		for(unsigned int i = 0; i < cps.size() - 1; i++)
		{
			if (ts[i] <= t && t <= ts[i+1])
			{
				RetVal = Hermite(cps[i],cps[i+1],Velocity[i],Velocity[i+1],ts[i],ts[i+1],t);
			}
		}
		//
		return RetVal;
	}
	//
	virtual lmVectorND<Type_T,Dim_T> GetDerivative(float t) const override
	{
		lmVectorND<Type_T,Dim_T> RetVal;
		//
		for(unsigned int i = 0; i < cps.size() - 1; i++)
		{
			if (ts[i] <= t && t <= ts[i+1])
			{
				RetVal = HermiteDerivative(cps[i],cps[i+1],Velocity[i],Velocity[i+1],ts[i],ts[i+1],t);
			}
		}
		//
		return RetVal;
	}
	//
	virtual lmVectorND<Type_T,Dim_T> GetSecondDerivative(float t) const override
	{
		lmVectorND<Type_T,Dim_T> RetVal;
		//
		for(unsigned int i = 0; i < cps.size() - 1; i++)
		{
			if (ts[i] <= t && t <= ts[i+1])
			{
				RetVal = HermiteSecondDerivative(cps[i],cps[i+1],Velocity[i],Velocity[i+1],ts[i],ts[i+1],t);
			}
		}
		//
		return RetVal;
	}
	//
	lmCatmullRomSpline(const lmVectorND<Type_T,Dim_T> &init_velocity,const lmVectorND<Type_T,Dim_T> &end_velocity)
		:InitVelocity(init_velocity),EndVelocity(end_velocity)
	{}
	//
	virtual ~lmCatmullRomSpline() override
	{}
};

/*
 * TODO Át kéne ezeket írni template-té.
 */

/** \brief A paraméterként kapott szöget átkonvertálja fokból radiánba.
 *
 * \param Deg lmScalar: A kapott szög fokokban.
 * \return lmScalar - a radiánba konvertált szög.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmDegToRad(lmScalar Deg);

/** \brief A paraméterként kapott szöget átkonvertálja radiánból fokba.
 *
 * \param Rad lmScalar: A kapott szög radiánban.
 * \return lmScalar - a fokokba konvertált szög.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmRadToDeg(lmScalar Rad);

/*
 * TODO A compiler double-ből floatba konvertálás miatt rinyál.
 */
int lmSgn(lmScalar value,lmScalar epsilon = 1e-4);

#endif // L_MATH_H
