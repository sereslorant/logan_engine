#ifndef BSP_TREE_H
#define BSP_TREE_H

#include <lCore/lMath/lMath.h>

/*
 * 1. cucc:
 * B. Naylor, J. Amanatides, and W. Thibualt,
 * 
 * "Merging BSP Trees Yields Polyhedral Set Operations",
 *
 * Computer Graphics (Siggraph '90), 24(3), 1990.
 * 
 * 
 * 2. cucc:
 * Samuel Ranta-Eskola,
 * 
 * "Binary Space Partioning Trees and Polygon 
 * Removal in Real Time 3D Rendering"
 * 
 * Information Technology
 * Computing Science Department
 * Uppsala University
 * 
 * 3. cucc:
 * https://pdfs.semanticscholar.org/eeb5/014f86750c54a87f214b03246799e970d114.pdf
 */

struct Triangle
{
	lmVector3D point[3];
	//
	lmVector3D GetCenter() const
	{
		return (point[0] + point[1] + point[2]) * (1.0 / 3.0);
	}
	//
	lmVector3D GetNormal() const
	{
		lmVector3D Result = lmCross(point[1] - point[0],point[2] - point[0]);
		Result.Normalize();
		return Result;
	}
	//
	Triangle()
	{}
	//
	Triangle(const lmVector3D &point1,const lmVector3D &point2,const lmVector3D &point3)
	{
		point[0] = point1;
		point[1] = point2;
		point[2] = point3;
	}
};

#include <list>

typedef std::list<Triangle> TriangleList;

enum EPointSide
{
	PT_IS_IN_FRONT,
	PT_IS_BEHIND,
	PT_COINCIDES,
};

enum ETriangleSide
{
	TRI_IS_IN_FRONT,
	TRI_IS_BEHIND,
	TRI_INTERSECTS,
	TRI_COINCIDES,
};

//constexpr double EPSILON = 1e-4;

/*
 * TMP!!!!!!!!!!!!!!!!!!!!!!!
 */
struct Plane;
/*
 * TMP!!!!!!!!!!!!!!!!!!!!!!!
 */

class BspUtils
{
public:
	//
	static void TranslateTriangleList(const TriangleList &triangle_list,const lmVector3D &translation,TriangleList &result)
	{
		for(Triangle triangle : triangle_list)
		{
			triangle.point[0] = triangle.point[0] + translation;
			triangle.point[1] = triangle.point[1] + translation;
			triangle.point[2] = triangle.point[2] + translation;
			//
			result.push_back(triangle);
		}
	}
	//
	static bool IsInFrontOrCoincides(EPointSide point_side)
	{
		return point_side == PT_IS_IN_FRONT || point_side == PT_COINCIDES;
	}
	//
	static bool IsBehindOrCoincides(EPointSide point_side)
	{
		return point_side == PT_IS_BEHIND || point_side == PT_COINCIDES;
	}
	//
	static bool HasInFront(EPointSide point1,EPointSide point2,EPointSide point3)
	{
		return point1 == PT_IS_IN_FRONT || point2 == PT_IS_IN_FRONT || point3 == PT_IS_IN_FRONT;
	}
	//
	static bool HasBehind(EPointSide point1,EPointSide point2,EPointSide point3)
	{
		return point1 == PT_IS_BEHIND || point2 == PT_IS_BEHIND || point3 == PT_IS_BEHIND;
	}
	//
	static bool TriangleListConvex(const TriangleList &triangle_list,double epsilon);
	//
	static bool FindSeparatingPlane(const TriangleList &triangle_list,Plane &separating_plane,double epsilon);
	/*
	 * End of class
	 */
};

#include <iostream>


#include <ostream>

void PrintVector(const lmVector3D &vec,std::ostream &out);

struct Plane
{
	lmVector3D point;
	lmVector3D normal;
	//
	EPointSide ClassifyPoint(const lmVector3D &classified_point,double epsilon) const
	{
		double distance = lmDot((classified_point - point),normal);
		//
		if(distance > epsilon)
		{
			return PT_IS_IN_FRONT;
		}
		else if (distance < -epsilon)
		{
			return PT_IS_BEHIND;
		}
		//
		return PT_COINCIDES;
	}
	//
	ETriangleSide ClassifyTriangle(const Triangle &classified_triangle,double epsilon) const
	{
		EPointSide point1Class = ClassifyPoint(classified_triangle.point[0],epsilon);
		EPointSide point2Class = ClassifyPoint(classified_triangle.point[1],epsilon);
		EPointSide point3Class = ClassifyPoint(classified_triangle.point[2],epsilon);
		//
		if(point1Class == PT_COINCIDES && point2Class == PT_COINCIDES && point3Class == PT_COINCIDES)
		{
			return TRI_COINCIDES;
		}
		//
		if(BspUtils::HasInFront(point1Class,point2Class,point3Class) && BspUtils::IsInFrontOrCoincides(point1Class) && BspUtils::IsInFrontOrCoincides(point2Class) && BspUtils::IsInFrontOrCoincides(point3Class))
		{
			return TRI_IS_IN_FRONT;
		}
		//
		if(BspUtils::HasBehind(point1Class,point2Class,point3Class) && BspUtils::IsBehindOrCoincides(point1Class) && BspUtils::IsBehindOrCoincides(point2Class) && BspUtils::IsBehindOrCoincides(point3Class))
		{
			return TRI_IS_BEHIND;
		}
		//
		return TRI_INTERSECTS;
	}
	//
	void SplitTriangle(Triangle intersecting_triangle,TriangleList &inside_triangles,TriangleList &outside_triangles,double epsilon) const
	{
		double pointDist[3];
		int insideCount = 0;
		int outsideCount = 0;
		//int coincidingCount = 0;
		for(unsigned int i=0;i < 3;i++)
		{
			pointDist[i] = lmDot((intersecting_triangle.point[i] - point),normal);
			//
			if(pointDist[i] > epsilon)
				{outsideCount++;}
			else //if(pointDist[i] < -EPSILON)
				{insideCount++;}
			/*else
				{coincidingCount++;}*/
		}
		//
		bool OneInside = insideCount == 1;
		double oneDist = 0.0;
		lmVector3D *reorderedVertices[3] = {nullptr,nullptr,nullptr};
		if((OneInside && !(pointDist[0] > epsilon)) || (!OneInside && pointDist[0] > epsilon))
		{
			oneDist = OneInside ? -pointDist[0] : pointDist[0];
			reorderedVertices[0] = &intersecting_triangle.point[0];
			reorderedVertices[1] = &intersecting_triangle.point[1];
			reorderedVertices[2] = &intersecting_triangle.point[2];
		}
		else if((OneInside && !(pointDist[1] > epsilon)) || (!OneInside && pointDist[1] > epsilon))
		{
			oneDist = OneInside ? -pointDist[1] : pointDist[1];
			reorderedVertices[0] = &intersecting_triangle.point[1];
			reorderedVertices[1] = &intersecting_triangle.point[2];
			reorderedVertices[2] = &intersecting_triangle.point[0];
		}
		else if((OneInside && !(pointDist[2] > epsilon)) || (!OneInside && pointDist[2] > epsilon))
		{
			oneDist = OneInside ? -pointDist[2] : pointDist[2];
			reorderedVertices[0] = &intersecting_triangle.point[2];
			reorderedVertices[1] = &intersecting_triangle.point[0];
			reorderedVertices[2] = &intersecting_triangle.point[1];
		}
		//
		Triangle newTriangles[3];
		lmVector3D v0tov1 = *reorderedVertices[1] - *reorderedVertices[0];
		lmVector3D v0tov2 = *reorderedVertices[2] - *reorderedVertices[0];
		double fullDistV1 = lmDot(v0tov1,normal);
		if(!OneInside)
			{fullDistV1 = -fullDistV1;}
		double fullDistV2 = lmDot(v0tov2,normal);
		if(!OneInside)
			{fullDistV2 = -fullDistV2;}
		lmVector3D newVertexTov1 = *reorderedVertices[0] + v0tov1*(oneDist/fullDistV1);
		lmVector3D newVertexTov2 = *reorderedVertices[0] + v0tov2*(oneDist/fullDistV2);
		//
		newTriangles[0].point[0] = *reorderedVertices[0];
		newTriangles[0].point[1] = newVertexTov1;
		newTriangles[0].point[2] = newVertexTov2;
		//
		newTriangles[1].point[0] = newVertexTov1;
		newTriangles[1].point[1] = *reorderedVertices[1];
		newTriangles[1].point[2] = *reorderedVertices[2];
		//
		newTriangles[2].point[0] = *reorderedVertices[2];
		newTriangles[2].point[1] = newVertexTov2;
		newTriangles[2].point[2] = newVertexTov1;
		//
		if(OneInside)
		{
			inside_triangles.push_back(newTriangles[0]);
			outside_triangles.push_back(newTriangles[1]);
			outside_triangles.push_back(newTriangles[2]);
		}
		else
		{
			outside_triangles.push_back(newTriangles[0]);
			inside_triangles.push_back(newTriangles[1]);
			inside_triangles.push_back(newTriangles[2]);
		}
	}
	//
	Plane()
	{
		//
	}
	//
	Plane(const lmVector3D &plane_point,const lmVector3D &plane_normal)
		:point(plane_point),normal(plane_normal)
	{
		normal.Normalize();
	}
	//
	Plane(const Triangle &triangle)
		:point(triangle.point[0]),normal(triangle.GetNormal())
	{
		//
	}
	/*
	 * End of class
	 */
};


#include <ostream>

#include <chrono>

//void PrintTriangle(const Triangle &triangle,std::ostream &out);

//void PrintTriangleList(const TriangleList &triangle_list,std::ostream &out);

class BspTree
{
private:
	Plane separatingPlane;
	TriangleList containedTriangles;
	//
	static BspTree NullObject;
	//
	BspTree *insideChild = &NullObject;
	BspTree *outsideChild = &NullObject;
	//
	BspTree()
	{
		//
	}
	//
	void ClassifyAgainstPolygons(const Triangle &triangle,TriangleList &inside_triangles,TriangleList &outside_triangles,double epsilon) const
	{
		std::list<Triangle> splittedTriangles;
		splittedTriangles.push_back(triangle);
		
		while(splittedTriangles.size() != 0)
		{
			Triangle nextTriangle = splittedTriangles.back();
			splittedTriangles.pop_back();
			//
			bool IsInside = true;
			for(const Triangle &containedTriangle : containedTriangles)
			{
				Plane trianglePlane(containedTriangle);
				ETriangleSide triangleSide = trianglePlane.ClassifyTriangle(nextTriangle,epsilon);
				//
				switch(triangleSide)
				{
				case TRI_IS_IN_FRONT:
					outside_triangles.push_back(nextTriangle);
					IsInside = false;
					//
					break;
				case TRI_INTERSECTS:
					{
						TriangleList insideTriangles;
						TriangleList outsideTriangles;
						trianglePlane.SplitTriangle(nextTriangle,insideTriangles,outsideTriangles,epsilon);
						//
						splittedTriangles.splice(splittedTriangles.end(),std::move(insideTriangles));
						splittedTriangles.splice(splittedTriangles.end(),std::move(outsideTriangles));
					}
					IsInside = false;
					break;
				default:
					//inside_triangles.push_back(nextTriangle);
					//Do nothing
					break;
				}
				//
				if(!IsInside)
				{
					break;
				}
			}
			//
			if(IsInside)
			{
				inside_triangles.push_back(nextTriangle);
			}
		}
	}
	//
public:
	//
	/*
	 * Minden, ami bináris fa specifikus
	 */
	//
	bool IsNull() const
	{
		return this == &NullObject;
	}
	//
	const BspTree &GetInside() const
	{
		return *insideChild;
	}
	//
	const BspTree &GetOutside() const
	{
		return *outsideChild;
	}
	//
	/*
	 * Minden, ami domain specifikus
	 */
	//
	const Plane &GetSeparatingPlane() const
	{
		return separatingPlane;
	}
	//
	const TriangleList &GetTriangleList() const
	{
		return containedTriangles;
	}
	//
	void ClassifyTriangle(const Triangle &triangle,TriangleList &inside_triangles,TriangleList &outside_triangles,double epsilon) const
	{
		ETriangleSide triangleSide = separatingPlane.ClassifyTriangle(triangle,epsilon);
		//
		switch(triangleSide)
		{
		case TRI_IS_IN_FRONT:
			if(GetOutside().IsNull())
				{outside_triangles.push_back(triangle);}
			else
				{GetOutside().ClassifyTriangle(triangle,inside_triangles,outside_triangles,epsilon);}
			//
			break;
		case TRI_INTERSECTS:
			{
				TriangleList insideTriangles;
				TriangleList outsideTriangles;
				separatingPlane.SplitTriangle(triangle,insideTriangles,outsideTriangles,epsilon);
				//
				for(const Triangle &outsideTriangle : outsideTriangles)
				{
					if(GetOutside().IsNull())
						{outside_triangles.push_back(outsideTriangle);}
					else
						{GetOutside().ClassifyTriangle(outsideTriangle,inside_triangles,outside_triangles,epsilon);}
				}
				//
				for(const Triangle &insideTriangle : insideTriangles)
				{
					if(GetInside().IsNull())
						{ClassifyAgainstPolygons(insideTriangle,inside_triangles,outside_triangles,epsilon);}
					else
						{GetInside().ClassifyTriangle(insideTriangle,inside_triangles,outside_triangles,epsilon);}
				}
			}
			//
			break;
		default:
			if(GetInside().IsNull())
				{ClassifyAgainstPolygons(triangle,inside_triangles,outside_triangles,epsilon);}
			else
				{GetInside().ClassifyTriangle(triangle,inside_triangles,outside_triangles,epsilon);}
		}
	}
	//
	void ClassifyTriangleList(const TriangleList &triangle_list,TriangleList &inside_triangles,TriangleList &outside_triangles,double epsilon)
	{
		for(const Triangle &triangle :triangle_list)
		{
			ClassifyTriangle(triangle,inside_triangles,outside_triangles,epsilon);
		}
	}
	//
	/*
	 * TODO: Át kéne írni move operátorral és move konstruktorral.
	 */
	BspTree(const TriangleList &triangle_list,double epsilon /*= EPSILON*/)
	{
		if(!BspUtils::FindSeparatingPlane(triangle_list,separatingPlane,epsilon))
		{
			containedTriangles = triangle_list;
		}
		else
		{
			TriangleList insideTriangles;
			TriangleList outsideTriangles;
			//
			//for(auto triangle = triangle_list.begin();triangle != triangle_list.end();triangle++)
			for(const Triangle &triangle : triangle_list)
			{
				ETriangleSide triangleSide = separatingPlane.ClassifyTriangle(triangle,epsilon);
				//
				switch(triangleSide)
				{
				case TRI_IS_IN_FRONT:
					outsideTriangles.push_back(triangle);
					break;
				case TRI_INTERSECTS:
					separatingPlane.SplitTriangle(triangle,insideTriangles,outsideTriangles,epsilon);
					break;
				default:
					insideTriangles.push_back(triangle);
				}
			}
			//
			if(insideTriangles.size() != 0)
			{
				insideChild = new BspTree(insideTriangles,epsilon);
			}
			if(outsideTriangles.size() != 0)
			{
				outsideChild = new BspTree(outsideTriangles,epsilon);
			}
		}
	}
	//
	//BspTree(const BspTree &one_tree,const BspTree &other_tree)
	//{
	//	//
	//}
	//
	~BspTree()
	{
		if(!insideChild->IsNull())
			{delete insideChild;}
		if(!outsideChild->IsNull())
			{delete outsideChild;}
	}
	/*
	 * End of class
	 */
};

#endif // BSP_TREE_H
