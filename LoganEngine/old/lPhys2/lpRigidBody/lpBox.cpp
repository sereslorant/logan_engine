
#include "lpBox.h"



void lpBox::CalculateBoxInertiaTensor()
{
	InertiaTensor[0][0] = 1.0/12.0 * Mass * (Height*Height + Depth*Depth);	InertiaTensor[1][0] = 0.0;                               				InertiaTensor[2][0] = 0.0;
	InertiaTensor[0][1] = 0.0;                                				InertiaTensor[1][1] = 1.0/12.0 * Mass * (Width*Width + Depth*Depth);	InertiaTensor[2][1] = 0.0;
	InertiaTensor[0][2] = 0.0;                                				InertiaTensor[1][2] = 0.0;                               				InertiaTensor[2][2] = 1.0/12.0 * Mass * (Width*Width + Height*Height);

	if(Mass != 0.0)
	{
		InertiaTensor.Invert(InvInertiaTensor);
	}
}

void lpBox::RecalculateMesh()
{
	//lmScalar BoundingSphereRadius = std::sqrt((Width/2.0)*(Width/2.0) + (Height/2.0)*(Height/2.0) + (Depth/2.0)*(Depth/2.0));
	//BoundingSphere->SetRadius(BoundingSphereRadius);

	/*
	 * Generating vertices
	 */

	//Front
	Vertices[0] = {-Width/2.0, Height/2.0, Depth/2.0};
	Vertices[1] = {-Width/2.0,-Height/2.0, Depth/2.0};
	Vertices[2] = { Width/2.0,-Height/2.0, Depth/2.0};
	Vertices[3] = { Width/2.0, Height/2.0, Depth/2.0};

	//Back
	Vertices[4] = {-Width/2.0, Height/2.0,-Depth/2.0};
	Vertices[5] = {-Width/2.0,-Height/2.0,-Depth/2.0};
	Vertices[6] = { Width/2.0,-Height/2.0,-Depth/2.0};
	Vertices[7] = { Width/2.0, Height/2.0,-Depth/2.0};

	GenerateBoundingSphere();
}

lpBox::lpBox(bool ghost,lmScalar mass,liRigidBody::liState *state,lmScalar width,lmScalar height,lmScalar depth)
	:lpMesh(ghost,mass,state,LP_BOX),Width(width),Height(height),Depth(depth)
{
	/*
	 * Generating normals
	 */
/*
	Normals.push_back(lmVector3D( 0.0, 0.0, 1.0));//Front face
	Normals.push_back(lmVector3D( 1.0, 0.0, 0.0));//Left face
	Normals.push_back(lmVector3D( 0.0, 0.0,-1.0));//Back face
	Normals.push_back(lmVector3D(-1.0, 0.0, 0.0));//Right face
	Normals.push_back(lmVector3D( 0.0, 1.0, 0.0));//Top face
	Normals.push_back(lmVector3D( 0.0,-1.0, 0.0));//Bottom face
*/
	Normals =  {{ 0.0, 0.0, 1.0},
				{ 1.0, 0.0, 0.0},
				{ 0.0, 0.0,-1.0},
				{-1.0, 0.0, 0.0},
				{ 0.0, 1.0, 0.0},
				{ 0.0,-1.0, 0.0}
				};
	/*
	 * Generating triangles
	 */
/*
	Triangles.push_back({0,1,2,0});Triangles.push_back({0,3,2,0});//Front face
	Triangles.push_back({2,3,6,1});Triangles.push_back({3,7,6,1});//Left face
	Triangles.push_back({4,5,6,2});Triangles.push_back({4,7,6,2});//Back face
	Triangles.push_back({0,1,5,3});Triangles.push_back({0,5,4,3});//Right face
	Triangles.push_back({0,3,4,4});Triangles.push_back({3,4,7,4});//Top face
	Triangles.push_back({1,2,6,5});Triangles.push_back({1,5,6,5});//Bottom face
*/

	Triangles = {{0,1,2,0},{0,3,2,0},//Front face
				 {2,3,6,1},{3,7,6,1},//Left face
				 {4,5,6,2},{4,7,6,2},//Back face
				 {0,1,5,3},{0,5,4,3},//Right face
				 {0,3,4,4},{3,4,7,4},//Top face
				 {1,2,6,5},{1,5,6,5}//Bottom face
				 };

	Vertices.resize(8);
	RecalculateMesh();

	for(int i=0;i < Triangles.size();i++)
	{
		lmVector3D Cukcsy = lmCross(Vertices[Triangles[i].V[2]] - Vertices[Triangles[i].V[0]],Vertices[Triangles[i].V[1]] - Vertices[Triangles[i].V[0]]);

		if(lmDot(Normals[Triangles[i].Normal],Cukcsy) < 0.0)
		{
			unsigned int Tmp = Triangles[i].V[1];
			Triangles[i].V[1]  = Triangles[i].V[2];
			Triangles[i].V[2]  = Tmp;
			/*
			std::cout << i << std::endl;

			Cukcsy = lmCross(Vertices[Triangles[i].V3] - Vertices[Triangles[i].V1],Vertices[Triangles[i].V2] - Vertices[Triangles[i].V1]);
			if(!(lmDot(Normals[Triangles[i].Normal],Cukcsy) < 0.0))
			{
				std::cout << "Győzelem!" << std::endl;
			}
			*/
		}
	}

	CalculateBoxInertiaTensor();
}

lmScalar lpBox::GetWidth()
{
	return Width;
}

lmScalar lpBox::GetHeight()
{
	return Height;
}

lmScalar lpBox::GetDepth()
{
	return Depth;
}

void lpBox::SetWidth(lmScalar width)
{
	Width = width;
	RecalculateMesh();
	CalculateBoxInertiaTensor();
}

void lpBox::SetHeight(lmScalar height)
{
	Height = height;
	RecalculateMesh();
	CalculateBoxInertiaTensor();
}

void lpBox::SetDepth(lmScalar depth)
{
	Depth = depth;
	RecalculateMesh();
	CalculateBoxInertiaTensor();
}
