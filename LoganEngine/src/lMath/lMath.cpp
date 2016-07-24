
#include "lMath.h"

/*
 * TODO A compiler double-ből floatba konvertálás miatt rinyál.
 */
lmScalar lmDegToRad(lmScalar Deg)
{
    return Deg*PI/180.0;
}

/*
 * TODO A compiler double-ből floatba konvertálás miatt rinyál.
 */
lmScalar lmRadToDeg(lmScalar Rad)
{
    return Rad*180.0/PI;
}
/*
lmScalar lmHeronFormula(const lmVector3D &A,const lmVector3D &B,const lmVector3D &C)
{
    lmScalar Side_A = sqrt((A-B).LengthSquared());
    lmScalar Side_B = sqrt((A-C).LengthSquared());
    lmScalar Side_C = sqrt((B-C).LengthSquared());

    lmScalar Semiperimeter =(Side_A+Side_B+Side_C)/2;

    return sqrt(Semiperimeter*(Semiperimeter-Side_A)*(Semiperimeter-Side_B)*(Semiperimeter-Side_C));
}
*/
int lmSgn(lmScalar value,lmScalar epsilon)
{
	if(std::abs(value) < epsilon)
	{
		return 0;
	}
	else
	{
		if(value < 0.0)
		{
			return -1;
		}
		//if(value > 0.0) //Warningot csinál, hogy control reached end of non void function.
		else
		{
			return 1;
		}
	}
}

