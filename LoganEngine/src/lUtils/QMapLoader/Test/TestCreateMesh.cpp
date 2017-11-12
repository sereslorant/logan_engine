
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

#include <iostream>

unsigned int screenWidth = 600;
unsigned int screenHeight = 600;

float X = 0.0;
float Y = 0.0;
float Z = 0.0;

float CameraAngle = 0.0;

#include <lCore/lMath/lMath.h>

constexpr unsigned int NUM_TRIANGLES = 6;
lmVector3D triangles[NUM_TRIANGLES][3] = 
{
	{{128,  0,  0},{128,100,  0},{128,  0,100}},
	{{256,  0,  0},{256,  0,100},{256,100,  0}},
	{{  0,128,  0},{  0,128,100},{100,128,  0}},
	{{  0,384,  0},{100,384,  0},{  0,384,100}},
	{{  0,  0, 64},{100,  0, 64},{  0,100, 64}},
	{{  0,  0,128},{  0,100,128},{100,  0,128}}
};

constexpr unsigned int NUM_TRIANGLES_2 = 4;
lmVector3D triangles2[NUM_TRIANGLES_2][3] = 
{
	{{1024,  0,  0},{1024,100,  0},{1024,  0,100}},
	{{1152,  0,  0},{1152,  0,100},{1252,  0,  0}},
	{{1152,  0,  0},{1252,  0,  0},{1252,100,  0}},
	{{1024,100,  0},{1324,-20,  0},{1024,  0,100}}
};

/*
hulye_vertices =
	{
		{ 0.0, 0.0,60.0},
		{ 0.0,30.0,60.0},
		{30.0, 0.0,60.0},
		{ 0.0, 0.0,90.0},
		{30.0, 0.0,90.0},
		{ 0.0,30.0,90.0},
	};
*/

constexpr unsigned int NUM_TRIANGLES_3 = 5;
lmVector3D triangles3[NUM_TRIANGLES_3][3] = 
{
	{{  0.0,  0.0,300.0},{100.0,  0.0,300.0},{  0.0,100.0,300.0}},
	{{  0.0,  0.0,300.0},{  0.0,100.0,300.0},{  0.0,  0.0,400.0}},
	{{  0.0,100.0,300.0},{100.0,  0.0,300.0},{100.0,  0.0,400.0}},
	{{  0.0,  0.0,300.0},{  0.0,  0.0,400.0},{100.0,  0.0,300.0}},
	{{  0.0,  0.0,400.0},{  0.0,100.0,400.0},{100.0,  0.0,400.0}}
};


constexpr unsigned int NUM_TRIANGLES_4 = 4;
lmVector3D triangles4[NUM_TRIANGLES_4][3] = 
{
	{{ 0.0,30.0, 0.0},{ 0.0, 0.0, 0.0},{30.0, 0.0, 0.0}},
	{{ 0.0,30.0, 0.0},{30.0, 0.0, 0.0},{ 0.0, 0.0,30.0}},
	{{ 0.0, 0.0, 0.0},{ 0.0, 0.0,30.0},{30.0, 0.0, 0.0}},
	{{ 0.0,30.0, 0.0},{ 0.0, 0.0,30.0},{ 0.0, 0.0, 0.0}},
};


constexpr unsigned int NUM_TRIANGLES_5 = 5;
lmVector3D triangles5[NUM_TRIANGLES_5][3] = 
{
	{{ 0.0,30.0, 0.0},{ 0.0, 0.0, 0.0},{30.0, 0.0, 0.0}},
	{{ 0.0,30.0, 0.0},{30.0, 0.0, 0.0},{ 0.0, 0.0,30.0}},
	{{ 0.0, 0.0, 0.0},{ 0.0, 0.0,30.0},{30.0, 0.0, 0.0}},
	{{ 0.0,15.0, 0.0},{15.0,15.0, 0.0},{ 0.0,15.0,15.0}},
	{{ 0.0,30.0, 0.0},{ 0.0, 0.0,30.0},{ 0.0, 0.0, 0.0}},
};

constexpr unsigned int NUM_TRIANGLES_6 = 6;
lmVector3D triangles6[NUM_TRIANGLES_6][3] = 
{
	{{100.0,  0.0,-300.0},{  0.0,  0.0,-300.0},{  0.0,100.0,-300.0}},
	{{  0.0,100.0,-300.0},{  0.0,  0.0,-300.0},{  0.0,  0.0,-400.0}},
	{{100.0,  0.0,-300.0},{  0.0,100.0,-300.0},{100.0,  0.0,-400.0}},
	{{  0.0,  0.0,-400.0},{  0.0,  0.0,-300.0},{100.0,  0.0,-300.0}},
	{{100.0, 60.0,-300.0},{  0.0, 60.0,-300.0},{  0.0, 60.0,-400.0}},
	{{  0.0,100.0,-400.0},{  0.0,  0.0,-400.0},{100.0,  0.0,-400.0}}
};

constexpr float DOWNSCALE = 10.0;
constexpr float NORMAL_SCALE = 5.0;


#include "../WeMesh.h"
#include "../Q3BrushMesh.h"

std::vector<Q3Plane> Planes;

std::list<lmVector3D> Vertices;
std::list<Q3Line> Lines;

WeMesh Result;


std::vector<Q3Plane>	TeglaPlanes;
std::list<lmVector3D>	TeglaVertices;
std::list<Q3Line>		TeglaLines;

WeMesh Tegla;


std::vector<Q3Plane>	HulyePlanes;
std::list<lmVector3D>	HulyeVertices;
std::list<Q3Line>		HulyeLines;

WeMesh Hulye;

std::vector<Q3Plane>	PyramidPlanes;
std::list<lmVector3D>	PyramidVertices;
std::list<Q3Line>		PyramidLines;

WeMesh Pyramid;

std::vector<Q3Plane>	GyokerPyramidPlanes;
std::list<lmVector3D>	GyokerPyramidVertices;
std::list<Q3Line>		GyokerPyramidLines;

WeMesh GyokerPyramid;


std::vector<Q3Plane>	GyokerHulyePlanes;
std::list<lmVector3D>	GyokerHulyeVertices;
std::list<Q3Line>		GyokerHulyeLines;

WeMesh GyokerHulye;

#include "TestWingedEdgeUtils.h"

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization()
{
	glPointSize(3.0);
	
	Planes.resize(NUM_TRIANGLES_2);
	unsigned int I = 0;
	for(lmVector3D *vertices : triangles2)
	{
		Planes[I] = Q3Plane(vertices[0],vertices[1],vertices[2]);
		I++;
	}
	
	ConstructBrushMesh(Planes,Lines,Vertices,Result);
	
	TeglaPlanes.resize(NUM_TRIANGLES);
	I = 0;
	for(lmVector3D *vertices : triangles)
	{
		TeglaPlanes[I] = Q3Plane(vertices[0],vertices[1],vertices[2]);
		I++;
	}
	
	ConstructBrushMesh(TeglaPlanes,TeglaLines,TeglaVertices,Tegla);
	
	HulyePlanes.resize(NUM_TRIANGLES_3);
	I = 0;
	for(lmVector3D *vertices : triangles3)
	{
		HulyePlanes[I] = Q3Plane(vertices[0],vertices[1],vertices[2]);
		I++;
	}
	
	ConstructBrushMesh(HulyePlanes,HulyeLines,HulyeVertices,Hulye);
	
	PyramidPlanes.resize(NUM_TRIANGLES_4);
	I = 0;
	for(lmVector3D *vertices : triangles4)
	{
		PyramidPlanes[I] = Q3Plane(vertices[0],vertices[1],vertices[2]);
		I++;
	}
	
	ConstructBrushMesh(PyramidPlanes,PyramidLines,PyramidVertices,Pyramid);
	
	GyokerPyramidPlanes.resize(NUM_TRIANGLES_5);
	I = 0;
	for(lmVector3D *vertices : triangles5)
	{
		GyokerPyramidPlanes[I] = Q3Plane(vertices[0],vertices[1],vertices[2]);
		I++;
	}
	
	ConstructBrushMesh(GyokerPyramidPlanes,GyokerPyramidLines,GyokerPyramidVertices,GyokerPyramid);
	
	GyokerHulyePlanes.resize(NUM_TRIANGLES_6);
	I = 0;
	for(lmVector3D *vertices : triangles6)
	{
		GyokerHulyePlanes[I] = Q3Plane(vertices[0],vertices[1],vertices[2]);
		I++;
	}
	
	//ConstructBrushMesh(GyokerHulyePlanes,GyokerHulyeLines,GyokerHulyeVertices,GyokerHulye);
	
	std::cout << "Init finished" << std::endl;
}

void DrawTriangle(const lmVector3D triangle[])
{
	glBegin(GL_TRIANGLES);
	//
	glVertex3f(triangle[0][0]/DOWNSCALE,triangle[0][1]/DOWNSCALE,triangle[0][2]/DOWNSCALE);
	glVertex3f(triangle[1][0]/DOWNSCALE,triangle[1][1]/DOWNSCALE,triangle[1][2]/DOWNSCALE);
	glVertex3f(triangle[2][0]/DOWNSCALE,triangle[2][1]/DOWNSCALE,triangle[2][2]/DOWNSCALE);
	//
	glEnd();
	//
	glBegin(GL_LINES);
	//
	lmVector3D Normal = lmCross(triangle[2] - triangle[0],triangle[1] - triangle[0]);
	Normal.Normalize();
	//
	glVertex3f(triangle[0][0]/DOWNSCALE,triangle[0][1]/DOWNSCALE,triangle[0][2]/DOWNSCALE);
	glVertex3f(triangle[0][0]/DOWNSCALE + Normal[0]*NORMAL_SCALE,
			   triangle[0][1]/DOWNSCALE + Normal[1]*NORMAL_SCALE,
			   triangle[0][2]/DOWNSCALE + Normal[2]*NORMAL_SCALE);
	//
	glVertex3f(triangle[1][0]/DOWNSCALE,triangle[1][1]/DOWNSCALE,triangle[1][2]/DOWNSCALE);
	glVertex3f(triangle[1][0]/DOWNSCALE + Normal[0]*NORMAL_SCALE,
			   triangle[1][1]/DOWNSCALE + Normal[1]*NORMAL_SCALE,
			   triangle[1][2]/DOWNSCALE + Normal[2]*NORMAL_SCALE);
	//
	glVertex3f(triangle[2][0]/DOWNSCALE,triangle[2][1]/DOWNSCALE,triangle[2][2]/DOWNSCALE);
	glVertex3f(triangle[2][0]/DOWNSCALE + Normal[0]*NORMAL_SCALE,
			   triangle[2][1]/DOWNSCALE + Normal[1]*NORMAL_SCALE,
			   triangle[2][2]/DOWNSCALE + Normal[2]*NORMAL_SCALE);
	//
	glEnd();
}

constexpr float LINE_PARAM = 60.0;

void DrawLine(const Q3Line &line)
{
	glBegin(GL_LINES);
	//
	glVertex3f(line.GetPoint()[0]/DOWNSCALE,
			   line.GetPoint()[1]/DOWNSCALE,
			   line.GetPoint()[2]/DOWNSCALE);
	glVertex3f(line.GetPoint()[0]/DOWNSCALE + line.GetDirection()[0]*LINE_PARAM,
			   line.GetPoint()[1]/DOWNSCALE + line.GetDirection()[1]*LINE_PARAM,
			   line.GetPoint()[2]/DOWNSCALE + line.GetDirection()[2]*LINE_PARAM);
	//
	/*glVertex3f(line.GetPoint()[0]/DOWNSCALE,
			   line.GetPoint()[1]/DOWNSCALE,
			   line.GetPoint()[2]/DOWNSCALE);
	glVertex3f(line.GetPoint()[0]/DOWNSCALE - line.GetDirection()[0]*20,
			   line.GetPoint()[1]/DOWNSCALE - line.GetDirection()[1]*20,
			   line.GetPoint()[2]/DOWNSCALE - line.GetDirection()[2]*20);
	*/
	//
	glEnd();
}

/*
void DrawPlane(const lmVector3D &point,const lmVector3D &normal)
{
	lmVector3D i = lmCross(point,normal);
	i.Normalize();
	lmVector3D j = lmCross(i,normal);
	j.Normalize();
	//
	lmVector3D vertex0 = point + i * 15 + j * 15;
	lmVector3D vertex1 = point + i * 15 - j * 15;
	lmVector3D vertex2 = point - i * 15 - j * 15;
	lmVector3D vertex3 = point - i * 15 + j * 15;
	//
	glBegin(GL_POINTS);
	//
	glVertex3f(point[0],point[1],point[2]);
	//
	glEnd();
	//
	glBegin(GL_QUADS);
	//
	glVertex3f(vertex0[0],vertex0[1],vertex0[2]);
	glVertex3f(vertex1[0],vertex1[1],vertex1[2]);
	glVertex3f(vertex2[0],vertex2[1],vertex2[2]);
	glVertex3f(vertex3[0],vertex3[1],vertex3[2]);
	//
	glEnd();
}
*/

void onDisplay()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glViewport(0, 0, screenWidth, screenHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90,1.0,1.0,-200.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    glTranslatef(X,Y,Z);
	
	glRotatef(CameraAngle,0.0,1.0,0.0);

	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_CULL_FACE);
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	for(int i=0;i < NUM_TRIANGLES;i++)
	{
		glColor3f((float)i / (float) NUM_TRIANGLES,1.0,0.0);
		DrawTriangle(triangles[i]);
	}
	
	for(int i=0;i < NUM_TRIANGLES_2;i++)
	{
		glColor3f((float)i / (float) NUM_TRIANGLES_2,1.0,0.0);
		DrawTriangle(triangles2[i]);
	}
	
	for(int i=0;i < NUM_TRIANGLES_3;i++)
	{
		glColor3f((float)i / (float) NUM_TRIANGLES_3,1.0,0.0);
		DrawTriangle(triangles3[i]);
	}
	
	for(int i=0;i < NUM_TRIANGLES_4;i++)
	{
		glColor3f((float)i / (float) NUM_TRIANGLES_4,1.0,0.0);
		DrawTriangle(triangles4[i]);
	}
	
	for(int i=0;i < NUM_TRIANGLES_5;i++)
	{
		glColor3f((float)i / (float) NUM_TRIANGLES_5,1.0,0.0);
		DrawTriangle(triangles5[i]);
	}
	
	for(int i=0;i < NUM_TRIANGLES_6;i++)
	{
		glColor3f((float)i / (float) NUM_TRIANGLES_6,1.0,0.0);
		DrawTriangle(triangles6[i]);
	}
	
	int I = 0;
	for(const Q3Line &Line : Lines)
	{
		glColor3f(1.0,(float)I / (float) Lines.size(),0.0);
		I++;
		DrawLine(Line);
	}
	
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_POINTS);
	for(lmVector3D &Vertex : Vertices)
	{
		glVertex3f(Vertex[0]/DOWNSCALE,Vertex[1]/DOWNSCALE,Vertex[2]/DOWNSCALE);
		
		std::cout << "[" << Vertex[0] << "," << Vertex[1] << "," << Vertex[2] <<  "]" << std::endl;
	}
	glEnd();
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-200.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawEdges(Result);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-400.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongPred(Result);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-600.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongSucc(Result);
	glPopMatrix();
	
	I = 0;
	for(const Q3Line &Line : TeglaLines)
	{
		glColor3f(1.0,(float)I / (float) Lines.size(),0.0);
		I++;
		DrawLine(Line);
	}
	
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_POINTS);
	for(lmVector3D &Vertex : TeglaVertices)
	{
		glVertex3f(Vertex[0]/DOWNSCALE,Vertex[1]/DOWNSCALE,Vertex[2]/DOWNSCALE);
		
		std::cout << "[" << Vertex[0] << "," << Vertex[1] << "," << Vertex[2] <<  "]" << std::endl;
	}
	glEnd();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-200.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawEdges(Tegla);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-400.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongPred(Tegla);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-600.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongSucc(Tegla);
	glPopMatrix();
	
	I = 0;
	for(const Q3Line &Line : HulyeLines)
	{
		glColor3f(1.0,(float)I / (float) Lines.size(),0.0);
		I++;
		DrawLine(Line);
	}
	
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_POINTS);
	for(lmVector3D &Vertex : HulyeVertices)
	{
		glVertex3f(Vertex[0]/DOWNSCALE,Vertex[1]/DOWNSCALE,Vertex[2]/DOWNSCALE);
		
		std::cout << "[" << Vertex[0] << "," << Vertex[1] << "," << Vertex[2] <<  "]" << std::endl;
	}
	glEnd();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,200.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawEdges(Hulye);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,400.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongPred(Hulye);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,600.0);
		
		glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongSucc(Hulye);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-100.0);
		
		//glTranslatef(-1024.0,0.0,0.0);
		
		DrawEdges(Pyramid);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-200.0);
		
		//glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongPred(Pyramid);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-300.0);
		
		//glTranslatef(-1024.0,0.0,0.0);
		
		DrawFacesAlongSucc(Pyramid);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-100.0);
		
		glTranslatef(-128.0,0.0,0.0);
		
		DrawEdges(GyokerPyramid);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-200.0);
		
		glTranslatef(-128.0,0.0,0.0);
		
		DrawFacesAlongPred(GyokerPyramid);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-300.0);
		
		glTranslatef(-128.0,0.0,0.0);
		
		DrawFacesAlongSucc(GyokerPyramid);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-100.0);
		
		glTranslatef(-128.0,0.0,0.0);
		
		DrawEdges(GyokerHulye);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-300.0);
		
		glTranslatef(-128.0,0.0,0.0);
		
		DrawFacesAlongPred(GyokerHulye);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(100,0,0);
		
		//glScalef(1.0/10.0,1.0/10.0,1.0/10.0);
		glTranslatef(0.0,0.0,-500.0);
		
		glTranslatef(-128.0,0.0,0.0);
		
		DrawFacesAlongSucc(GyokerHulye);
	glPopMatrix();
	
    glutSwapBuffers();
}

void onKeyboard(unsigned char key, int x, int y)
{
	if(tolower(key) == 'q')
	{
		CameraAngle -= 10.0;
	}
	if(tolower(key) == 'e')
	{
		CameraAngle += 10.0;
	}

	if(tolower(key) == 'w')
	{
		Z -= 5.0;
	}
	if(tolower(key) == 's')
	{
		Z += 5.0;
	}

	if(tolower(key) == 'a')
	{
		X -= 5.0;
	}
	if(tolower(key) == 'd')
	{
		X += 5.0;
	}

	if(tolower(key) == 'f')
	{
		Y -= 5.0;
	}
	if(tolower(key) == 'r')
	{
		Y += 5.0;
	}
}

void onKeyboardUp(unsigned char key, int x, int y) {

}

void onMouse(int button, int state, int x, int y) {

}

void onMouseMotion(int x, int y) {

}

void onIdle( ) {


	glutPostRedisplay( );
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Demo");

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    onInitialization();

    glutDisplayFunc(onDisplay);
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();

    return 0;
}

