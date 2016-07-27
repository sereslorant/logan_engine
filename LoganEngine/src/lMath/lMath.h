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
