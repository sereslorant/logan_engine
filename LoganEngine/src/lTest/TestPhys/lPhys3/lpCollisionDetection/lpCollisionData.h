#ifndef LP_COLLISION_DATA_H
#define LP_COLLISION_DATA_H

#include <lCore/lMath/lMath.h>

struct lpCollisionData
{
	unsigned int Shape1Id;
	unsigned int Shape2Id;
	float NormalOverlap;
	lmVector3D Normal;
	lmVector3D IntersectionPoint1;
	lmVector3D IntersectionPoint2;
};

#endif // LP_COLLISION_DATA_H
