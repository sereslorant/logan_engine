
#include "BspTree.h"

BspTree BspTree::NullObject;


void PrintVector(const lmVector3D &vec,std::ostream &out)
{
	out << '[' << vec[0] << ',' << vec[1] << ',' << vec[2] << ']';
}

void PrintTriangle(const Triangle &triangle,std::ostream &out)
{
	out << '{';
	out << "p1:";
	PrintVector(triangle.point[0],out);
	out << ',';
	out << "p2:";
	PrintVector(triangle.point[1],out);
	out << ',';
	out << "p3:";
	PrintVector(triangle.point[2],out);
	out << '}';
}

void PrintTriangleList(const TriangleList &triangle_list,std::ostream &out)
{
	out << '[';
	int i=0;
	for(const Triangle &triangle : triangle_list)
	{
		PrintTriangle(triangle,out);
		i++;
		out << (i == triangle_list.size() ? ']' : ',');
	}
}

bool BspUtils::TriangleListConvex(const TriangleList &triangle_list,double epsilon)
{
	for(auto I = triangle_list.begin();I != triangle_list.end();I++)
	{
		Plane planeOfClassifier(*I);
		for(auto J = triangle_list.begin();J != triangle_list.end();J++)
		{
			if(I == J)
				{continue;}
			//
			ETriangleSide triangleSide = planeOfClassifier.ClassifyTriangle(*J,epsilon);
			if(!(triangleSide == TRI_IS_BEHIND || triangleSide == TRI_COINCIDES))
				{return false;}
		}
	}
	return true;
}

struct SeparatingPlaneStatistics
{
	unsigned int numIntersectingTriangles = 0;
	unsigned int numTrianglesInside = 0;
	unsigned int numTrianglesOutside = 0;
	//
	double GetInsideOutsideRatio()
	{
		return (double)numTrianglesInside / (double)numTrianglesOutside;
	}
};

bool CompareNewStatisticsBetter(SeparatingPlaneStatistics &old_statistics,SeparatingPlaneStatistics &new_statistics)
{
	double oldImbalancedness = std::abs(old_statistics.GetInsideOutsideRatio() - 1.0);
	double newImbalancedness = std::abs(new_statistics.GetInsideOutsideRatio() - 1.0);
	//
	if(oldImbalancedness - newImbalancedness > 1e-2)
	{
		return true;
	}
	else
	{
		if(new_statistics.numIntersectingTriangles < old_statistics.numIntersectingTriangles)
		{
			return true;
		}
		//
		return false;
	}
}

bool CheckPlaneIsSeparating(SeparatingPlaneStatistics &statistics)
{
	if(statistics.numIntersectingTriangles == 0 && (statistics.numTrianglesInside == 0 || statistics.numTrianglesOutside == 0))
	{
		return false;
	}
	//
	return true;
}

bool BspUtils::FindSeparatingPlane(const TriangleList &triangle_list,Plane &separating_plane,double epsilon)
{
	SeparatingPlaneStatistics bestStatistics;
	bool foundPlane = false;
	for(const Triangle &triangle : triangle_list)
	{
		SeparatingPlaneStatistics currentStatistics;
		Plane currentPlane(triangle);
		for(const Triangle &classifiedTriangle : triangle_list)
		{
			ETriangleSide triangleSide = currentPlane.ClassifyTriangle(classifiedTriangle,epsilon);
			switch(triangleSide)
			{
			case TRI_IS_IN_FRONT:
				currentStatistics.numTrianglesOutside++;
				break;
			case TRI_INTERSECTS:
				currentStatistics.numIntersectingTriangles++;
				break;
			default:
				currentStatistics.numTrianglesInside++;
			}
		}
		//
		if(CheckPlaneIsSeparating(currentStatistics))
		{
			//
			//std::cout << "Triangle list length: " << triangle_list.size() << std::endl;;
			//std::cout << "Current statistics: " << std::endl;
			//std::cout << "Triangles inside: " << currentStatistics.numTrianglesInside << std::endl;
			//std::cout << "Triangles outside: " << currentStatistics.numTrianglesOutside << std::endl;
			//std::cout << "Triangles intersecting: " << currentStatistics.numIntersectingTriangles << std::endl;
			if(!foundPlane)
			{
				bestStatistics		= currentStatistics;
				separating_plane	= currentPlane;
				foundPlane = true;
			}
			else if(CompareNewStatisticsBetter(bestStatistics,currentStatistics))
			{
				bestStatistics		= currentStatistics;
				separating_plane	= currentPlane;
			}
		}
	}
	/*
	std::cout << "Chosen plane:" << std::endl;
	std::cout << separating_plane.normal[0] << ";" << separating_plane.normal[1] << ";" << separating_plane.normal[2] <<std::endl;
	std::cout << separating_plane.point[0] << ";" << separating_plane.point[1] << ";" << separating_plane.point[2] <<std::endl;
	std::cout << "Triangle list length: " << triangle_list.size() << std::endl;;
	std::cout << "Current statistics: " << std::endl;
	std::cout << "Triangles inside: " << bestStatistics.numTrianglesInside << std::endl;
	std::cout << "Triangles outside: " << bestStatistics.numTrianglesOutside << std::endl;
	std::cout << "Triangles intersecting: " << bestStatistics.numIntersectingTriangles << std::endl;
	*/
	return foundPlane;
}
