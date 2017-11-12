
#include "Q3Geometry.h"

bool Q3PlaneIntersects(const Q3Plane &plane1,const Q3Plane &plane2)
{
	float NormalDot = lmDot(plane1.GetNormal(),plane2.GetNormal());
	
	if((std::abs(NormalDot) - 1.0) < (-1e-4))
	{
		return true;
	}
	
	return false;
}

Q3Line Q3LineOfIntersection(const Q3Plane &plane1,const Q3Plane &plane2)
{
	lmVector3D Direction = lmCross(plane1.GetNormal(),plane2.GetNormal());
	
	lmVector3D Point;
	lmRayPlaneIntersection(	plane1.GetPoint(),lmCross(plane1.GetNormal(),Direction),
							plane2.GetPoint(),plane2.GetNormal(),
							(float*)nullptr,&Point
						  );
	
	return Q3Line(Point,Direction);
}

void Q3FlipLine(Q3Line &line,const Q3Plane &plane)
{
	if(lmDot(line.GetDirection(),plane.GetNormal()) > 0.0)
		{line.FlipDirection();}
}
