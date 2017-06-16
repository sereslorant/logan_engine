
#include <lPhys3/lpCollisionDetection/lpCollisionShape.h>

#include <iostream>

#include <vector>

void PrintVector(const lmVector2D &vec)
{
	std::cout << "(" << vec[0] << "," << vec[1] << ")";
}

void PrintCircle(const lpCircle &circle)
{
	std::cout << "Position = ";
	PrintVector(circle.GetPosition());
	std::cout << std::endl;
	std::cout << "Radius = " << "(" << circle.GetRadius() << std::endl;
}

void PrintCollisionData(const lpCollisionData &collision_data)
{
	std::cout << collision_data.Shape1Id << std::endl;
	std::cout << collision_data.Shape2Id << std::endl;
	PrintVector(collision_data.Normal);
	std::cout << std::endl;
	std::cout << collision_data.NormalOverlap << std::endl;
	PrintVector(collision_data.IntersectionPoint1);
	std::cout << std::endl;
	PrintVector(collision_data.IntersectionPoint2);
	std::cout << std::endl;
}

void TestCollisionTable()
{
	lpBody2DState State1(lmVector2D({-1.0f,0.0f}),lmVector2D({0.0f,0.0f}));
	lpCircle Circle1(&State1,1.5);
	//
	lpBody2DState State2(lmVector2D({1.0f,1.0f}),lmVector2D({0.0f,0.0f}));
	lpCircle Circle2(&State2,1.5);
	//
	lpBody2DState State3(lmVector2D({4.0f,3.0f}),lmVector2D({0.0f,0.0f}));
	lpCircle Circle3(&State3,1.0);
	//
	lpSpacePartition SpacePartition;
	//
	SpacePartition.AddCollisionShape(Circle1);
	SpacePartition.AddCollisionShape(Circle2);
	SpacePartition.AddCollisionShape(Circle3);
	//
	SpacePartition.CheckCollision(0.2);
	//
	std::cout << "Bodies" << std::endl;
	PrintCircle(Circle1);
	PrintCircle(Circle2);
	PrintCircle(Circle3);
	//
	unsigned int NumCollisions = SpacePartition.GetNumCollisions();
	std::cout << "NumCollisions: " << NumCollisions << std::endl;
	//
	if(NumCollisions == 1)
	{
		lpCollisionData CollisionData;
		SpacePartition.GetCollisionData(&CollisionData,1);
		PrintCollisionData(CollisionData);
		std::cout << "Test succeeded" << std::endl;
	}
	else
	{
		std::cout << "Test failed" << std::endl;
	}
}

int main(int argc, char *argv[])
{
	TestCollisionTable();
	return 0;
}
