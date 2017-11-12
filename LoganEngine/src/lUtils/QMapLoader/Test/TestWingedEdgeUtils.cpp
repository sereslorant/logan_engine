
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <lCore/lMath/lMath.h>

#include <iostream>

#include "TestWingedEdgeUtils.h"

void DrawEdges(WeMesh &mesh)
{
	std::cout << "Edges\n" << std::endl;
	int EdgeId = 0;
	
	glBegin(GL_LINES);
	for(WeEdge &Edge : mesh.Edges)
	{
		lmVector3D &Begin = *Edge.BeginVertex->GetData<lmVector3D>();
		lmVector3D &End = *Edge.EndVertex->GetData<lmVector3D>();
		
		std::cout << "Edge id: " << EdgeId << " Ptr: " << &Edge << std::endl;
		std::cout << "Begin: " << Edge.BeginVertex << " End: " << Edge.EndVertex << std::endl;
		
		EdgeId++;
		
		glColor3f(1.0,1.0,0.0);
		glVertex3f(Begin[0],Begin[1],Begin[2]);
		
		glColor3f(1.0,0.0,0.0);
		glVertex3f(End[0],End[1],End[2]);
	}
	glEnd();
	
	std::cout << std::endl;
}

void DrawFacesAlongSucc(WeMesh &mesh)
{
	std::cout << "Faces along succ\n" << std::endl;
	int FaceId = 0;
	for(WeFace &Face : mesh.Faces)
	{
		std::cout << "Face id: " << FaceId << " Ptr: " << &Face << std::endl;
		
		glBegin(GL_POLYGON);
		
		WeEdge *FirstEdge = Face.OneEdge;
		
		glColor3f(0.0,1.0,FaceId * (1.0 / mesh.Faces.size()));
		FaceId++;
		
		std::cout << "Edge id: " << 0 << " Ptr: " << FirstEdge << std::endl;
		
		if(Face.GetSideOfEdge(FirstEdge) == WE_RIGHT)
		{
			std::cout << "Right side. Vertex ptr: " << FirstEdge->EndVertex << std::endl;
			
			lmVector3D &Begin = *FirstEdge->EndVertex->GetData<lmVector3D>();
			glVertex3f(Begin[0],Begin[1],Begin[2]);
		}
		else
		{
			std::cout << "Left side. Vertex ptr: " << FirstEdge->BeginVertex << std::endl;
			
			lmVector3D &End = *FirstEdge->BeginVertex->GetData<lmVector3D>();
			glVertex3f(End[0],End[1],End[2]);
		}
		
		int EdgeId=1;
		for(WeEdge *IterEdge = FirstEdge->Side[Face.GetSideOfEdge(FirstEdge)].Successor;IterEdge != FirstEdge; IterEdge = IterEdge->Side[Face.GetSideOfEdge(IterEdge)].Successor)
		{
			std::cout << "Edge id: " << EdgeId << " Ptr: " << IterEdge << std::endl;
			EdgeId++;
			
			if(IterEdge == nullptr)
			{
				std::cout << "Nullptr!" << std::endl;
				break;
			}
			
			if(Face.GetSideOfEdge(IterEdge) == WE_RIGHT)
			{
				std::cout << "Right side. Vertex ptr: " << IterEdge->EndVertex << std::endl;
				
				lmVector3D &Begin = *IterEdge->EndVertex->GetData<lmVector3D>();
				glVertex3f(Begin[0],Begin[1],Begin[2]);
			}
			else if(Face.GetSideOfEdge(IterEdge) == WE_LEFT)
			{
				std::cout << "Left side. Vertex ptr: " << IterEdge->BeginVertex << std::endl;
				
				lmVector3D &End = *IterEdge->BeginVertex->GetData<lmVector3D>();
				glVertex3f(End[0],End[1],End[2]);
			}
			else
			{
				std::cout << "Error! Triangle not neighboring!" << std::endl;
				std::cout << "This triangle: " << &Face << std::endl;
				std::cout << "Left: " << IterEdge->Side[WE_LEFT].Face << std::endl;
				std::cout << "Right: " << IterEdge->Side[WE_RIGHT].Face << std::endl;
				break;
			}
		}
		
		glEnd();
	}
	
	std::cout << std::endl;
}

void DrawFacesAlongPred(WeMesh &mesh)
{
	std::cout << "Faces along pred\n" << std::endl;
	int FaceId = 0;
	for(WeFace &Face : mesh.Faces)
	{
		std::cout << "Face id: " << FaceId << " Ptr: " << &Face << std::endl;
		
		glBegin(GL_POLYGON);
		
		WeEdge *FirstEdge = Face.OneEdge;
		
		glColor3f(0.0,1.0,FaceId * (1.0 / mesh.Faces.size()));
		FaceId++;
		
		std::cout << "Edge id: " << 0 << " Ptr: " << FirstEdge << std::endl;
		
		if(Face.GetSideOfEdge(FirstEdge) == WE_RIGHT)
		{
			std::cout << "Right side. Vertex ptr: " << FirstEdge->EndVertex << std::endl;
			
			lmVector3D &Begin = *FirstEdge->EndVertex->GetData<lmVector3D>();
			glVertex3f(Begin[0],Begin[1],Begin[2]);
		}
		else
		{
			std::cout << "Left side. Vertex ptr: " << FirstEdge->BeginVertex << std::endl;
			
			lmVector3D &End = *FirstEdge->BeginVertex->GetData<lmVector3D>();
			glVertex3f(End[0],End[1],End[2]);
		}
		
		int EdgeId=1;
		for(WeEdge *IterEdge = FirstEdge->Side[Face.GetSideOfEdge(FirstEdge)].Predecessor;IterEdge != FirstEdge; IterEdge = IterEdge->Side[Face.GetSideOfEdge(IterEdge)].Predecessor)
		{
			std::cout << "Edge id: " << EdgeId << " Ptr: " << IterEdge << std::endl;
			EdgeId++;
			
			if(IterEdge == nullptr)
			{
				std::cout << "Nullptr!" << std::endl;
				break;
			}
			
			if(Face.GetSideOfEdge(IterEdge) == WE_RIGHT)
			{
				std::cout << "Right side. Vertex ptr: " << IterEdge->EndVertex << std::endl;
				
				lmVector3D &Begin = *IterEdge->EndVertex->GetData<lmVector3D>();
				glVertex3f(Begin[0],Begin[1],Begin[2]);
			}
			else if(Face.GetSideOfEdge(IterEdge) == WE_LEFT)
			{
				std::cout << "Left side. Vertex ptr: " << IterEdge->BeginVertex << std::endl;
				
				lmVector3D &End = *IterEdge->BeginVertex->GetData<lmVector3D>();
				glVertex3f(End[0],End[1],End[2]);
			}
			else
			{
				std::cout << "Error! Triangle not neighboring!" << std::endl;
				std::cout << "This triangle: " << &Face << std::endl;
				std::cout << "Left: " << IterEdge->Side[WE_LEFT].Face << std::endl;
				std::cout << "Right: " << IterEdge->Side[WE_RIGHT].Face << std::endl;
				break;
			}
		}
		
		glEnd();
	}
	
	std::cout << std::endl;
}
