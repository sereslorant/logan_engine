
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

float X = 00.0;
float Y = 00.0;
float Z = -20.0;

float CameraAngle = 0.0;

#include <lUtils/Bsp/BspTree.h>

unsigned int drawnPlanes = 5;
unsigned int drawnTriangles = 8;

constexpr unsigned int NUM_PLANES = 5;
Plane planeArray[NUM_PLANES] = {
	{{ 2.0, 0.0, 0.0},{ 1.0, 1.0, 0.0}},
	{{ 0.0,-3.0, 0.0},{ 1.0,-2.0, 0.0}},
	{{-2.0,-1.0, 0.0},{-2.0, 1.0, 0.0}},
	{{ 0.0, 2.0,-3.0},{ 0.0, 0.0,-1.0}},
	{{ 0.0, 2.0, 3.0},{ 0.0, 1.0, 1.0}},
}; 

/*
 * Konvex vertex tömb
 */
constexpr unsigned int NUM_VERTICES = 6;
lmVector3D vertexArray[NUM_VERTICES] = {
	{ 3.0,-2.0,-3.0},{ 0.0, 3.0,-3.0},{-3.0,-4.0,-3.0},
	{ 0.0, 3.0, 2.0},{ 3.0,-2.0, 7.0},{-3.0,-4.0, 9.0},
};

/*
 * Konvex poligon modell.
 */
constexpr unsigned int NUM_TRIANGLES = 8;
Triangle triangleArray[NUM_TRIANGLES] = {
	{vertexArray[0],vertexArray[2],vertexArray[1]},
	{vertexArray[3],vertexArray[5],vertexArray[4]},
	{vertexArray[0],vertexArray[5],vertexArray[2]},
	{vertexArray[0],vertexArray[4],vertexArray[5]},
	{vertexArray[1],vertexArray[4],vertexArray[0]},
	{vertexArray[1],vertexArray[3],vertexArray[4]},
	{vertexArray[1],vertexArray[2],vertexArray[5]},
	{vertexArray[1],vertexArray[5],vertexArray[3]},
};

TriangleList triangleList;

BspTree *convexBspTree = nullptr;

/*
 * Konkáv vertex tömb
 */
constexpr unsigned int NUM_CONCAVE_VERTICES = 5;
lmVector3D concaveVertexArray[NUM_CONCAVE_VERTICES] = {
	{ 0.0, 2.0, 0.0},
	{ 3.0, 2.0, 3.0},
	{-3.0, 2.0, 0.0},
	{ 3.0, 2.0,-3.0},
	{-2.0, 6.0, 0.0},
};

/*
 * Konkáv poligon modell.
 */
constexpr unsigned int NUM_CONCAVE_TRIANGLES = 6;
Triangle concaveTriangleArray[NUM_CONCAVE_TRIANGLES] = {
	{concaveVertexArray[0],concaveVertexArray[1],concaveVertexArray[2]},
	{concaveVertexArray[2],concaveVertexArray[3],concaveVertexArray[0]},
	{concaveVertexArray[1],concaveVertexArray[4],concaveVertexArray[2]},
	{concaveVertexArray[3],concaveVertexArray[2],concaveVertexArray[4]},
	{concaveVertexArray[3],concaveVertexArray[4],concaveVertexArray[0]},
	{concaveVertexArray[1],concaveVertexArray[0],concaveVertexArray[4]},
};

TriangleList concaveTriangleList;

BspTree *concaveBspTree = nullptr;

/*
 * Konkáv vertex tömb 2
 */
constexpr unsigned int NUM_CONCAVE_VERTICES2 = 5;
lmVector3D concaveVertexArray2[NUM_CONCAVE_VERTICES2] = {
	{ 0.0, 0.0, 8.0},
	{ 3.0, 3.0, 8.0},
	{-3.0, 0.0, 8.0},
	{ 3.0,-3.0, 8.0},
	{-2.0, 0.0, 12.0},
};

/*
 * Konkáv poligon modell.
 */
constexpr unsigned int NUM_CONCAVE_TRIANGLES2 = 6;
Triangle concaveTriangleArray2[NUM_CONCAVE_TRIANGLES2] = {
	{concaveVertexArray2[0],concaveVertexArray2[2],concaveVertexArray2[1]},
	{concaveVertexArray2[2],concaveVertexArray2[0],concaveVertexArray2[3]},
	{concaveVertexArray2[1],concaveVertexArray2[2],concaveVertexArray2[4]},
	{concaveVertexArray2[3],concaveVertexArray2[4],concaveVertexArray2[2]},
	{concaveVertexArray2[3],concaveVertexArray2[0],concaveVertexArray2[4]},
	{concaveVertexArray2[1],concaveVertexArray2[4],concaveVertexArray2[0]},
};

TriangleList concaveTriangleList2;

BspTree *concaveBspTree2 = nullptr;

Triangle splittedTriangle({ 0.0, 6.0,-8.0},{-3.0,-2.0,-8.0},{ 3.0,-2.0,-8.0});

Plane upSplittingPlane({0.0,3.0,-8.0},{0.0,1.0,0.0});

TriangleList upInsideTriangles;
TriangleList upOutsideTriangles;

Plane downSplittingPlane({0.0,3.0,-8.0},{0.0,1.0,0.0});

TriangleList downInsideTriangles;
TriangleList downOutsideTriangles;

Plane oneTestSplittingPlane({1.0,3.0,-8.0},{-1.0,0.0,0.0});

TriangleList oneTestInsideTriangles;
TriangleList oneTestOutsideTriangles;

Plane otherTestSplittingPlane({-1.0,3.0,-8.0},{-1.0,0.0,0.0});

TriangleList otherTestInsideTriangles;
TriangleList otherTestOutsideTriangles;

Triangle splittedByConvexTriangle({ -6.0, 2.0,0.0},{-6.0,-6.0, 0.0},{ 3.0,-6.0, 0.0});

TriangleList splittedByConvexInside;
TriangleList splittedByConvexOutside;

TriangleList convexMeshCsgInside;
TriangleList convexMeshCsgOutside;

TriangleList concaveMeshCsgInside;
TriangleList concaveMeshCsgOutside;



#include "Csirguru.h"

void DrawCsirkeTest(const TransfiniteInterpolation &csirguru,TriangleList &triangle_list)
{
	for(unsigned int i=0;i < VERTICES_DIM_X - 1;i++)
	{
		for(unsigned int j=0;j < VERTICES_DIM_Y;j++)
		{
			unsigned int I =  i+1;
			unsigned int J = (j+1)%VERTICES_DIM_Y;

			const lmVector3D &v0 = csirguru.Vertices[i][j];
			const lmVector3D &v1 = csirguru.Vertices[i][J];
			const lmVector3D &v2 = csirguru.Vertices[I][J];
			const lmVector3D &v3 = csirguru.Vertices[I][j];
			
			//dvec4 &n0 = Normals[i][j];
			//dvec4 &n1 = Normals[i][J];
			//dvec4 &n2 = Normals[I][J];
			//dvec4 &n3 = Normals[I][j];
			
			triangle_list.push_back({v0,v2,v1});
			triangle_list.push_back({v3,v2,v0});
		}
	}
	//
	for(int i=0;i < VERTICES_DIM_X;i++)
	{
		triangle_list.push_back({csirguru.Csor,csirguru.Vertices[0][i],csirguru.Vertices[0][(i+1) % VERTICES_DIM_X]});
	}
	//
	for(int i=0;i < VERTICES_DIM_X;i++)
	{
		triangle_list.push_back({csirguru.Farok,csirguru.Vertices[VERTICES_DIM_Y - 1][(i+1) % VERTICES_DIM_X],csirguru.Vertices[VERTICES_DIM_Y - 1][i]});
	}
}

TriangleList csirguruTriangles;
TriangleList csirguruTrianglesTranslated;
BspTree *CsirguruTree = nullptr;
BspTree *TranslatedCsirguruTree = nullptr;

TriangleList csirguru1Inside,csirguru1Outside;
TriangleList csirguru2Inside,csirguru2Outside;

#include <chrono>

float CSIRGURU_EPSILON = 1e-2;
float CSIRGURU_SPLIT_EPSILON = 1e-4;

void StressTest()
{
	TransfiniteInterpolation *Csirguru = InitCsirguruTest();
	DrawCsirkeTest(*Csirguru,csirguruTriangles);
	
	BspUtils::TranslateTriangleList(csirguruTriangles,lmVector3D({5.0,0.0,0.0}),csirguruTrianglesTranslated);
	
	std::cout << "Started!" << std::endl;
	auto Before = std::chrono::steady_clock::now();
	
	CsirguruTree = new BspTree(csirguruTriangles,CSIRGURU_EPSILON);
	
	auto After = std::chrono::steady_clock::now();
	auto Difference = After - Before;
	std::cout << "BSP construction time: " << std::chrono::duration<double,std::milli>(Difference).count() << std::endl;
	
	Before = std::chrono::steady_clock::now();
	
	TranslatedCsirguruTree = new BspTree(csirguruTrianglesTranslated,CSIRGURU_EPSILON);
	
	After = std::chrono::steady_clock::now();
	Difference = After - Before;
	std::cout << "BSP construction time: " << std::chrono::duration<double,std::milli>(Difference).count() << std::endl;
	
	
	Before = std::chrono::steady_clock::now();
	
	CsirguruTree->ClassifyTriangleList(csirguruTrianglesTranslated,csirguru1Inside,csirguru1Outside,CSIRGURU_SPLIT_EPSILON);
	
	After = std::chrono::steady_clock::now();
	Difference = After - Before;
	std::cout << "Classification time 1: " << std::chrono::duration<double,std::milli>(Difference).count() << std::endl;
	
	Before = std::chrono::steady_clock::now();
	
	TranslatedCsirguruTree->ClassifyTriangleList(csirguruTriangles,csirguru2Inside,csirguru2Outside,CSIRGURU_SPLIT_EPSILON);
	
	After = std::chrono::steady_clock::now();
	Difference = After - Before;
	std::cout << "Classification time 2: " << std::chrono::duration<double,std::milli>(Difference).count() << std::endl;
	
	delete Csirguru;
}

float CTR_EPSILON = 1e-1;
float CONVEX_EPSILON = 1e-1;
float SPLIT_EPSILON = 1e-1;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization()
{
	for(Triangle &triangle : triangleArray)
	{
		triangleList.push_back(triangle);
	}
	//
	bool IsConvex = BspUtils::TriangleListConvex(triangleList,CONVEX_EPSILON);
	std::cout << "TriangleListConvex: " << IsConvex << std::endl;
	//
	for(Triangle &triangle : concaveTriangleArray)
	{
		concaveTriangleList.push_back(triangle);
	}
	//
	IsConvex = BspUtils::TriangleListConvex(concaveTriangleList,CONVEX_EPSILON);
	std::cout << "ConcaveTriangleListConvex: " << IsConvex << std::endl;
	//numCsirguruTriangles
	for(Triangle &triangle : concaveTriangleArray2)
	{
		concaveTriangleList2.push_back(triangle);
	}
	//
	convexBspTree = new BspTree(triangleList,CTR_EPSILON);
	concaveBspTree = new BspTree(concaveTriangleList,CTR_EPSILON);
	concaveBspTree2 = new BspTree(concaveTriangleList2,CTR_EPSILON);
	//
	upSplittingPlane.SplitTriangle(splittedTriangle,upInsideTriangles,upOutsideTriangles,SPLIT_EPSILON);
	downSplittingPlane.SplitTriangle(splittedTriangle,downInsideTriangles,downOutsideTriangles,SPLIT_EPSILON);
	//
	oneTestSplittingPlane.SplitTriangle(splittedTriangle,oneTestInsideTriangles,oneTestOutsideTriangles,SPLIT_EPSILON);
	//
	otherTestSplittingPlane.SplitTriangle(splittedTriangle,otherTestInsideTriangles,otherTestOutsideTriangles,SPLIT_EPSILON);
	//
	convexBspTree->ClassifyTriangle(splittedByConvexTriangle,splittedByConvexInside,splittedByConvexOutside,SPLIT_EPSILON);
	//
	concaveBspTree->ClassifyTriangleList(triangleList,convexMeshCsgInside,convexMeshCsgOutside,SPLIT_EPSILON);
	convexBspTree->ClassifyTriangleList(concaveTriangleList,concaveMeshCsgInside,concaveMeshCsgOutside,SPLIT_EPSILON);
	//
	StressTest();
	//
	glPointSize(4);
}

void DrawTriangle(const Triangle &triangle)
{
	glBegin(GL_TRIANGLES);
	//
	glVertex3f(triangle.point[0][0],triangle.point[0][1],triangle.point[0][2]);
	glVertex3f(triangle.point[1][0],triangle.point[1][1],triangle.point[1][2]);
	glVertex3f(triangle.point[2][0],triangle.point[2][1],triangle.point[2][2]);
	//
	glEnd();
	//
	glBegin(GL_LINES);
	//
	glVertex3f(triangle.point[0][0],triangle.point[0][1],triangle.point[0][2]);
	glVertex3f(triangle.point[0][0] + triangle.GetNormal()[0]/7.0,
			   triangle.point[0][1] + triangle.GetNormal()[1]/7.0,
			   triangle.point[0][2] + triangle.GetNormal()[2]/7.0);
	//
	glVertex3f(triangle.point[1][0],triangle.point[1][1],triangle.point[1][2]);
	glVertex3f(triangle.point[1][0] + triangle.GetNormal()[0]/7.0,
			   triangle.point[1][1] + triangle.GetNormal()[1]/7.0,
			   triangle.point[1][2] + triangle.GetNormal()[2]/7.0);
	//
	glVertex3f(triangle.point[2][0],triangle.point[2][1],triangle.point[2][2]);
	glVertex3f(triangle.point[2][0] + triangle.GetNormal()[0]/7.0,
			   triangle.point[2][1] + triangle.GetNormal()[1]/7.0,
			   triangle.point[2][2] + triangle.GetNormal()[2]/7.0);
	//
	glEnd();
}

void DrawTriangleList(const TriangleList &triangles,unsigned int num_drawn_triangles)
{
	float green = 0.0;
	int i = 0;
	for(const Triangle &triangle : triangles)
	{
		glColor3f(1.0,green,0.0);
		DrawTriangle(triangle);
		//
		green += 1.0/triangles.size();
		//
		i++;
		if(i == num_drawn_triangles)
			{break;}
	}
}

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

void DrawBsp(const BspTree &bsp_tree)
{
	//std::cout << "DrawingBsp" << std::endl;
	
	//DrawPlane(bsp_tree.GetSeparatingPlane().point,bsp_tree.GetSeparatingPlane().normal);
	DrawTriangleList(bsp_tree.GetTriangleList(),bsp_tree.GetTriangleList().size());
	
	if(!bsp_tree.GetInside().IsNull())
	{
		//std::cout << "Going inside" << std::endl;
		DrawBsp(bsp_tree.GetInside());
	}
	
	if(!bsp_tree.GetOutside().IsNull())
	{
		//std::cout << "Going outside" << std::endl;
		DrawBsp(bsp_tree.GetOutside());
	}
}

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
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
	
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	//glColor3f(1.0,0.5,0.0);
	//for(int i=0;i < std::min(drawnPlanes,NUM_PLANES);i++)
	//{
	//	DrawPlane(planeArray[i].point,planeArray[i].normal);
	//}
	DrawBsp(*convexBspTree);
	
	//DrawTriangleList(triangleList,drawnTriangles);
	//DrawTriangleList(concaveTriangleList,concaveTriangleList.size());
	DrawBsp(*concaveBspTree);
	DrawBsp(*concaveBspTree2);
	
	DrawTriangle(splittedTriangle);
	
	DrawTriangle(splittedByConvexTriangle);
	
	glPushMatrix();
		glTranslatef(-2.5,-2.5,-2.5);
		DrawTriangleList(splittedByConvexInside,splittedByConvexInside.size());
		
		glTranslatef( 0.0, 0.0,-2.5);
		DrawTriangleList(splittedByConvexOutside,splittedByConvexOutside.size());
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(-27.0, 0.0, 0.0);
		DrawBsp(*concaveBspTree);
		DrawBsp(*convexBspTree);
		
		glTranslatef(-27.0, 0.0, 0.0);
		DrawBsp(*concaveBspTree);
		glTranslatef(-12.0, 0.0, 0.0);
		DrawBsp(*convexBspTree);
	
		glTranslatef(-27.0, 0.0, 0.0);
		DrawTriangleList(convexMeshCsgInside,convexMeshCsgInside.size());

		glTranslatef(-12.0, 0.0, 0.0);
		DrawTriangleList(convexMeshCsgOutside,convexMeshCsgOutside.size());

		glTranslatef(-12.0, 0.0, 0.0);
		DrawTriangleList(concaveMeshCsgInside,concaveMeshCsgInside.size());

		glTranslatef(-12.0, 0.0, 0.0);
		DrawTriangleList(concaveMeshCsgOutside,concaveMeshCsgOutside.size());
	glPopMatrix();
	
	glTranslatef(0.0,0.0,-5.0);
	
	DrawTriangleList(upInsideTriangles,upInsideTriangles.size());
	DrawTriangleList(upOutsideTriangles,upOutsideTriangles.size());
	
	glTranslatef(0.0,0.0,-5.0);
	
	DrawTriangleList(downInsideTriangles,downInsideTriangles.size());
	DrawTriangleList(downOutsideTriangles,downOutsideTriangles.size());
	
	glTranslatef(0.0,0.0,-5.0);
	
	DrawTriangleList(oneTestInsideTriangles,oneTestInsideTriangles.size());
	DrawTriangleList(oneTestOutsideTriangles,oneTestOutsideTriangles.size());
	
	glTranslatef(0.0,0.0,-5.0);
	
	DrawTriangleList(otherTestInsideTriangles,otherTestInsideTriangles.size());
	DrawTriangleList(otherTestOutsideTriangles,otherTestOutsideTriangles.size());
	
	glPushMatrix();
		glTranslatef(10.0,0.0,0.0);
		DrawTriangleList(csirguruTriangles,csirguruTriangles.size());
		DrawTriangleList(csirguruTrianglesTranslated,csirguruTrianglesTranslated.size());
		
		glPushMatrix();
			glTranslatef(0.0,0.0,20.0);
			DrawBsp(*CsirguruTree);
		glPopMatrix();
		
		glTranslatef(15.0,0.0,0.0);
		DrawTriangleList(csirguru1Inside,csirguru1Inside.size());
		
		glTranslatef(15.0,0.0,0.0);
		DrawTriangleList(csirguru1Outside,csirguru1Outside.size());
		
		glTranslatef(15.0,0.0,0.0);
		DrawTriangleList(csirguru2Inside,csirguru2Inside.size());
		
		glTranslatef(15.0,0.0,0.0);
		DrawTriangleList(csirguru2Outside,csirguru2Outside.size());
	glPopMatrix();
	
    glutSwapBuffers();     				// Buffercsere: rajzolas vege
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
	
	if(tolower(key) == 't')
	{
		drawnPlanes = (drawnPlanes + 1) % (NUM_PLANES + 1);
	}
	if(tolower(key) == 'g')
	{
		drawnTriangles = (drawnTriangles + 1) % (NUM_TRIANGLES + 1);
	}
}

// Billentyuzet esemenyeket lekezelo fuggveny (felengedes)
void onKeyboardUp(unsigned char key, int x, int y) {

}

// Eger esemenyeket lekezelo fuggveny
void onMouse(int button, int state, int x, int y) {

}

// Eger mozgast lekezelo fuggveny
void onMouseMotion(int x, int y) {

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {


	glutPostRedisplay( ); // Ilyenkor rajzold ujra a kepet
}

// A C++ program belepesi pontja, a main fuggvenyt mar nem szabad bantani
int main(int argc, char **argv) {
    glutInit(&argc, argv); 				// GLUT inicializalasa
    glutInitWindowSize(screenWidth, screenHeight);			// Alkalmazas ablak kezdeti merete 600x600 pixel
    glutInitWindowPosition(100, 100);			// Az elozo alkalmazas ablakhoz kepest hol tunik fel
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// 8 bites R,G,B,A + dupla buffer + melyseg buffer

    glutCreateWindow("Demo");		// Alkalmazas ablak megszuletik es megjelenik a kepernyon

    glMatrixMode(GL_MODELVIEW);				// A MODELVIEW transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);			// A PROJECTION transzformaciot egysegmatrixra inicializaljuk
    glLoadIdentity();

    onInitialization();					// Az altalad irt inicializalast lefuttatjuk

    glutDisplayFunc(onDisplay);				// Esemenykezelok regisztralasa
    glutMouseFunc(onMouse);
    glutIdleFunc(onIdle);
    glutKeyboardFunc(onKeyboard);
    glutKeyboardUpFunc(onKeyboardUp);
    glutMotionFunc(onMouseMotion);

    glutMainLoop();					// Esemenykezelo hurok

    return 0;
}
