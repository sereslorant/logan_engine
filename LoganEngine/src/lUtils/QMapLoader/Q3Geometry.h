#ifndef Q3_GEOMETRY_H
#define Q3_GEOMETRY_H

#include <lCore/lMath/lMath.h>

class Q3Plane
{
private:
	lmVector3D Point;
	lmVector3D Normal;
	
public:
	
	const lmVector3D &GetPoint() const
	{
		return Point;
	}
	
	const lmVector3D &GetNormal() const
	{
		return Normal;
	}
	
	float GetDistFromOrigin() const
	{
		return lmDot(Point,Normal);
	}
	
	bool PointIsInFront(const lmVector3D &categorized_point)
	{
		return lmDot(Normal,categorized_point - Point) > 0.0;
	}
	
	Q3Plane()
	{}
	
	Q3Plane(const lmVector3D &p1,const lmVector3D &p2,const lmVector3D &p3)
		:Point(p1)
	{
		Normal = lmCross(p3 - p1,p2 - p1);
		Normal.Normalize();
	}
	
	~Q3Plane()
	{}
};

class Q3Line
{
private:
	lmVector3D Point;
	lmVector3D Direction;
	
public:
	
	void FlipDirection()
	{
		Direction *= -1.0;
	}
	
	const lmVector3D &GetPoint() const
	{
		return Point;
	}
	
	const lmVector3D &GetDirection() const
	{
		return Direction;
	}
	
	Q3Line(const lmVector3D &point,const lmVector3D &direction)
		:Point(point),Direction(direction)
	{}
	
	~Q3Line()
	{}
};

/**
 * 
 * \warning Elhanyagoljuk azt, amikor két sík egybeesik.
 * 
 */
bool Q3PlaneIntersects(const Q3Plane &plane1,const Q3Plane &plane2);

Q3Line Q3LineOfIntersection(const Q3Plane &plane1,const Q3Plane &plane2);

void Q3FlipLine(Q3Line &line,const Q3Plane &plane);

#endif // Q3_GEOMETRY_H
