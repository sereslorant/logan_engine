
#include <lPhys3/lpCollisionDetection/lpSpacePartition.h>

#include <iostream>

#include <vector>

void PrintVector(const lmVector3D &vec)
{
	std::cout << "(" << vec[0] << "," << vec[1] << "," << vec[2] << ")";
}

void PrintSphere(const lpSphere &circle)
{
	std::cout << "Position = ";
	PrintVector(circle.GetPosition());
	std::cout << std::endl;
	std::cout << "Radius = " << circle.GetRadius() << std::endl;
}

void PrintComposite(const lpCompositeShape &composite)
{
	std::cout << "Composite" << std::endl;
	std::cout << "Children:" << std::endl;
	
	for(int i=0;i < composite.GetNumChildren();i++)
	{
		std::cout << "Child " << i << ":" << std::endl;
		lShapeGetter Getter;
		composite.GetChild(i)->Accept(Getter);
		if(Getter.GetConstSphere() != nullptr)
		{
			PrintSphere(*Getter.GetConstSphere());
		}
		if(Getter.GetConstComposite() != nullptr)
		{
			PrintComposite(*Getter.GetConstComposite());
		}
	}
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

void TestSphereCollisionTable()
{
	lpShapeState State1({-1.0f,0.0f,0.0f},{0.0f,0.0f,0.0f});
	lpSphere Sphere1(State1,1.5);
	
	lpShapeState State2({1.0f,1.0f,0.0f},{0.0f,0.0f,0.0f});
	lpSphere Sphere2(State2,1.5);
	
	lpShapeState State3({4.0f,3.0f,0.0f},{0.0f,0.0f,0.0f});
	lpSphere Sphere3(State3,1.0);
	
	lpSpacePartition SpacePartition;
	
	SpacePartition.AddCollisionShape(0,&Sphere1);
	SpacePartition.AddCollisionShape(1,&Sphere2);
	SpacePartition.AddCollisionShape(2,&Sphere3);
	
	SpacePartition.CheckCollision(0.2);
	
	std::cout << "Bodies" << std::endl;
	PrintSphere(Sphere1);
	PrintSphere(Sphere2);
	PrintSphere(Sphere3);
	
	unsigned int NumCollisions = SpacePartition.GetNumCollisions();
	std::cout << "NumCollisions: " << NumCollisions << std::endl;
	
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

void TestCompositeCollisionTable()
{
	lpShapeState State1({-1.0f,0.0f,0.0f},{0.0f,0.0f,0.0f});
	lpSphere Sphere1(State1,1.5);
	
	lpShapeState State2({1.0f,1.0f,0.0f},{0.0f,0.0f,0.0f});
	lpSphere Sphere2(State2,1.5);
	
	lpShapeState StateComposite({0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f});
	std::list<lpCollisionShape *> CompositeChildren = {&Sphere1,&Sphere2};
	lpCompositeShape Composite(StateComposite,CompositeChildren.begin(),CompositeChildren.end());
	
	lpShapeState State3({0.0f,-1.0f,0.0f},{0.0f,0.0f,0.0f});
	lpSphere Sphere(State3,1.0);
	
	lpSpacePartition SpacePartition;
	
	SpacePartition.AddCollisionShape(0,&Composite);
	SpacePartition.AddCollisionShape(1,&Sphere);
	
	SpacePartition.CheckCollision(0.2);
	
	std::cout << "Bodies" << std::endl;
	PrintComposite(Composite);
	PrintSphere(Sphere);
	
	unsigned int NumCollisions = SpacePartition.GetNumCollisions();
	std::cout << "NumCollisions: " << NumCollisions << std::endl;
	
	if(NumCollisions == 2)
	{
		lpCollisionData CollisionData[2];
		SpacePartition.GetCollisionData(CollisionData,2);
		PrintCollisionData(CollisionData[0]);
		PrintCollisionData(CollisionData[1]);
		std::cout << "Test succeeded" << std::endl;
	}
	else
	{
		std::cout << "Test failed" << std::endl;
	}
};

int main(int argc, char *argv[])
{
	TestSphereCollisionTable();
	TestCompositeCollisionTable();
	return 0;
}
