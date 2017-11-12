
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

#include "TestWingedEdgeUtils.h"

#include <iostream>

unsigned int screenWidth = 600;
unsigned int screenHeight = 600;

float X = 0.0;
float Y = 0.0;
float Z = 0.0;

float CameraAngle = 0.0;

#include <lCore/lMath/lMath.h>


#include "../WeMesh.h"

void CreatePyramid(WeMesh pyramid,std::vector<lmVector3D> pyramid_vertices)
{
	pyramid_vertices =
	{
		{ 0.0,30.0, 0.0},
		{30.0, 0.0, 0.0},
		{ 0.0, 0.0,30.0},
		{ 0.0, 0.0, 0.0},
	};
	
	/*
	 * Creating initial shape
	 */
	
	WeFace *Faces[3];
	for(int i=0;i < 3;i++)
	{
		Faces[i] = pyramid.CreateFace();
	}
	
	WeVertex *Vertices[4];
	for(int i=0;i < 4;i++)
	{
		Vertices[i] = pyramid.CreateVertex();
		Vertices[i]->Data = &pyramid_vertices[i];
	}
	
	WeEdge *Edges[3];
	for(int i=0;i < 3;i++)
	{
		Edges[i] = pyramid.CreateEdge();
		Edges[i]->BeginVertex = Vertices[0];
	}
	
	WeConnectFaceBorder(Vertices[0],Faces[0],Edges[1],Edges[0]);
	WeConnectFaceBorder(Vertices[0],Faces[1],Edges[0],Edges[2]);
	WeConnectFaceBorder(Vertices[0],Faces[2],Edges[2],Edges[1]);
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////
	 * Step 1
	 */
	
	/*
	 * Adding extra face
	 */
	
	WeEdge *NewEdges[2];
	for(int i=0;i < 2;i++)
	{
		NewEdges[i] = pyramid.CreateEdge();
		//NewEdges[i]->BeginVertex = Vertices[3];
	}
	
	WeFace *NewFace;
	NewFace = pyramid.CreateFace();
	
	WeConnectNewFace(Edges[0],Vertices[3],NewFace,NewEdges);
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////
	 * Step 2
	 */
	
	/*
	 * Go towards Face[1];
	 */
	
	/*
	 * Since NewEdges[1] intersects Faces[2] we need to perform this.
	 */
	
	WeCloseFaceIntoVertex(Faces[1],NewEdges[1],Edges[2],Vertices[2]);
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////
	 * Step 3
	 */
	
	/*
	 * Connecting to Face 2.
	 */
	
	
	
	WeEdge *LastEdge = pyramid.CreateEdge();
	LastEdge->BeginVertex = Vertices[2];
	
	WeEdge *PreviousNewEdge = NewEdges[1];
	WeEdge *PreviousExistingEdge = Edges[2];
	WeFace *CurrentFace = PreviousExistingEdge->Side[WE_RIGHT].Face;
	WeEdge *CurrentNewEdge = LastEdge;
	
	//We attach it to the NewFace, append it after NewEdges[1].
	WeAttachEdgeToFace(NewEdges[1],NewFace,LastEdge,WE_LEFT);
	
	//We also attach it to Faces[2], connect it to Edges[2].
	//WeAttachEdgeToFace(Edges[2],Faces[2],LastEdge,NewFace->GetOtherSideOfEdge(LastEdge));
	WeAttachEdgeToFace(PreviousExistingEdge,CurrentFace,CurrentNewEdge,NewFace->GetOtherSideOfEdge(CurrentNewEdge));
	
	/*
	 * Since LastEdge intersects Faces[0] we need to perform this.
	 */
	
	/*
	 * Now we close down Faces[2] into Vertices[1]
	 */
	//LastEdge->EndVertex = Vertices[1];
	//Edges[1]->EndVertex = Vertices[1];
	
	WeEdge *CurrentExistingEdge = Edges[1];
	
	// Face 2
	//WeCloseFace(Faces[2],LastEdge,Edges[1]);
	WeCloseFaceIntoVertex(CurrentFace,CurrentNewEdge,CurrentExistingEdge,Vertices[1]);
	
	
	
	/*
	 * Finally we close down NewFace and Faces[0] into Vertices[1]
	 */
	NewEdges[0]->EndVertex = Vertices[1];
	
	//NewFace
	WeCloseFace(NewFace,NewEdges[0],CurrentNewEdge);
	
	// Face 0
	WeCloseFace(NewEdges[0]->Side[WE_LEFT].Face,CurrentExistingEdge,NewEdges[0]);
}

void CreateHulyeAlakzat1(WeMesh &hulye_alakzat,std::vector<lmVector3D> &hulye_vertices)
{
	hulye_vertices =
	{
		{ 0.0, 0.0,60.0},
		{ 0.0,30.0,60.0},
		{30.0, 0.0,60.0},
		{ 0.0, 0.0,90.0},
		{30.0, 0.0,90.0},
		{ 0.0,30.0,90.0},
	};
	
	/*
	 * Creating initial shape
	 */
	
	WeFace *Faces[3];
	for(int i=0;i < 3;i++)
	{
		Faces[i] = hulye_alakzat.CreateFace();
	}
	
	WeVertex *Vertices[6];
	for(int i=0;i < 6;i++)
	{
		Vertices[i] = hulye_alakzat.CreateVertex();
		Vertices[i]->Data = &hulye_vertices[i];
	}
	
	WeEdge *Edges[3];
	for(int i=0;i < 3;i++)
	{
		Edges[i] = hulye_alakzat.CreateEdge();
		Edges[i]->BeginVertex = Vertices[0];
	}
	
	WeConnectFaceBorder(Vertices[0],Faces[0],Edges[1],Edges[0]);
	WeConnectFaceBorder(Vertices[0],Faces[1],Edges[0],Edges[2]);
	WeConnectFaceBorder(Vertices[0],Faces[2],Edges[2],Edges[1]);
	
	/* /////////////////////////////////////////////////////////////////////////////////////////////
	 * Step 1
	 */
	
	/*
	 * Adding extra face
	 */
	
	WeEdge *NewEdges[2];
	for(int i=0;i < 2;i++)
	{
		NewEdges[i] = hulye_alakzat.CreateEdge();
	}
	
	WeFace *NewFace;
	NewFace = hulye_alakzat.CreateFace();
	
	WeConnectNewFace(Edges[0],Vertices[3],NewFace,NewEdges);
	
	/*
	 * Since neither NewEdges[0] nor NewEdges[1] intersects Faces[2] we can create a new face.
	 */
	
	WeEdge *LastEdges[2];
	for(int i=0;i < 2;i++)
	{
		LastEdges[i] = hulye_alakzat.CreateEdge();
	}
	
	WeFace *LastFace;
	LastFace = hulye_alakzat.CreateFace();
	
	WeConnectNewFace(Edges[2],Vertices[1],LastFace,LastEdges);
	
	/*
	 * Since LastEdges[1] intersects Faces[2] we need to perform this.
	 */
	
	WeCloseFaceIntoVertex(Faces[2],LastEdges[1],Edges[1],Vertices[2]);
	
	
	
	WeEdge *NewLastEdge1 = hulye_alakzat.CreateEdge();
	NewLastEdge1->BeginVertex = Vertices[2];
	
	WeEdge *PreviousNewEdge = LastEdges[1];
	WeEdge *PreviousExistingEdge = Edges[1];
	WeFace *CurrentFace = PreviousExistingEdge->Side[WE_RIGHT].Face;
	WeEdge *CurrentNewEdge = NewLastEdge1;
	
	
	
	WeAttachEdgeToFace(PreviousNewEdge,LastFace,CurrentNewEdge,WE_LEFT);
	
	WeAttachEdgeToFace(PreviousExistingEdge,CurrentFace,CurrentNewEdge,LastFace->GetOtherSideOfEdge(CurrentNewEdge));
	
	
	
	WeEdge *CurrentExistingEdge = NewEdges[0];
	
	WeCloseFaceIntoVertex(CurrentFace,CurrentNewEdge,CurrentExistingEdge,Vertices[4]);
	
	
	
	WeEdge *NewLastEdge2 = hulye_alakzat.CreateEdge();
	NewLastEdge2->BeginVertex = Vertices[4];
	
	PreviousNewEdge = CurrentNewEdge;
	PreviousExistingEdge = CurrentExistingEdge;
	CurrentFace = PreviousExistingEdge->Side[WE_RIGHT].Face;
	CurrentNewEdge = NewLastEdge2;
	
	
	
	WeAttachEdgeToFace(PreviousNewEdge,LastFace,CurrentNewEdge,WE_LEFT);
	
	WeAttachEdgeToFace(PreviousExistingEdge,CurrentFace,CurrentNewEdge,LastFace->GetOtherSideOfEdge(CurrentNewEdge));
	
	
	
	CurrentExistingEdge = NewEdges[1];
	
	WeCloseFaceIntoVertex(CurrentFace,CurrentNewEdge,CurrentExistingEdge,Vertices[5]);
	
	
	
	/*
	 * Finally we close down LastFace and Faces[1] into Vertices[5]
	 */
	LastEdges[0]->EndVertex = Vertices[5];
	
	//LastFace
	WeCloseFace(LastFace,LastEdges[0],CurrentNewEdge);
	
	// Face 1
	WeCloseFace(LastEdges[0]->Side[WE_LEFT].Face,CurrentExistingEdge,LastEdges[0]);
}

void CreateHulyeAlakzat2(WeMesh &hulye_alakzat,std::vector<lmVector3D> &hulye_vertices)
{
	hulye_vertices =
	{
		{ 0.0, 0.0,120.0},
		{ 0.0,30.0,120.0},
		{30.0, 0.0,120.0},
		{ 0.0, 0.0,150.0},
		{30.0, 0.0,150.0},
		{ 0.0,30.0,150.0},
	};
	
	/*
	 * Creating initial shape
	 */
	
	WeFace *Faces[3];
	for(int i=0;i < 3;i++)
	{
		Faces[i] = hulye_alakzat.CreateFace();
	}
	
	WeVertex *Vertices[6];
	for(int i=0;i < 6;i++)
	{
		Vertices[i] = hulye_alakzat.CreateVertex();
		Vertices[i]->Data = &hulye_vertices[i];
	}
	
	WeEdge *Edges[3];
	for(int i=0;i < 3;i++)
	{
		Edges[i] = hulye_alakzat.CreateEdge();
		Edges[i]->BeginVertex = Vertices[0];
	}
	
	WeConnectFaceBorder(Vertices[0],Faces[0],Edges[0],Edges[1]);
	WeConnectFaceBorder(Vertices[0],Faces[1],Edges[2],Edges[0]);
	WeConnectFaceBorder(Vertices[0],Faces[2],Edges[1],Edges[2]);
	
	
	/*
	 * Adding extra face
	 */
	
	WeEdge *NewEdges[2];
	for(int i=0;i < 2;i++)
	{
		NewEdges[i] = hulye_alakzat.CreateEdge();
	}
	
	WeFace *NewFace;
	NewFace = hulye_alakzat.CreateFace();
	
	WeConnectNewFace(Edges[2],Vertices[1],NewFace,NewEdges);
	
	//WeCloseFaceIntoVertex(Faces[2],NewEdges[0],Edges[1],Vertices[2]);
	WeCloseFaceIntoVertex(Faces[2],Edges[1],NewEdges[0],Vertices[2]);
	
	
		
	
	WeEdge *NewNewEdge = hulye_alakzat.CreateEdge();
	NewNewEdge->BeginVertex = Vertices[2];
	
	WeEdge *PreviousNewEdge = NewEdges[0];
	WeEdge *PreviousExistingEdge = Edges[1];
	WeEdge *CurrentNewEdge = NewNewEdge;
	//WeEdge *CurrentExistingEdge = NewEdges[0];
	WeFace *CurrentFace = PreviousExistingEdge->Side[WE_LEFT].Face;
	
	
	
	WeAttachEdgeToFace(PreviousNewEdge,NewFace,CurrentNewEdge,WE_RIGHT);
	
	WeAttachEdgeToFace(PreviousExistingEdge,CurrentFace,CurrentNewEdge,NewFace->GetOtherSideOfEdge(CurrentNewEdge));
	
	
	
	WeEdge *LastEdges[2];
	for(int i=0;i < 2;i++)
	{
		LastEdges[i] = hulye_alakzat.CreateEdge();
	}
	
	WeFace *LastFace;
	LastFace = hulye_alakzat.CreateFace();
	
	WeConnectNewFace(NewNewEdge,Vertices[4],LastFace,LastEdges);
	
	WeCloseFaceIntoVertex(NewFace,LastEdges[1],NewEdges[1],Vertices[5]);
	
	
	
	//LastEdges[0]->EndVertex = Vertices[3];
	//LastEdges[1]->EndVertex = Vertices[5];
	
	std::cout << NewFace->GetSideOfEdge(LastEdges[0]) << " " << NewFace->GetSideOfEdge(LastEdges[1]) << std::endl;
	std::cout << Faces[0]->GetSideOfEdge(LastEdges[0]) << " " << Faces[0]->GetSideOfEdge(LastEdges[1]) << std::endl;
	std::cout << NewFace->GetSideOfEdge(NewEdges[1]) << " " << NewFace->GetSideOfEdge(NewEdges[1]) << std::endl;
	
	WeEdge *LastEdge3 = hulye_alakzat.CreateEdge();
	LastEdge3->BeginVertex = Vertices[5];
	
	PreviousNewEdge = LastEdges[1];
	PreviousExistingEdge = NewEdges[1];
	CurrentNewEdge = LastEdge3;
	//WeEdge *CurrentExistingEdge = NewEdges[0];
	CurrentFace = PreviousExistingEdge->Side[WE_RIGHT].Face;
	
	
	
	WeAttachEdgeToFace(PreviousNewEdge,LastFace,CurrentNewEdge,WE_LEFT);
	
	WeAttachEdgeToFace(PreviousExistingEdge,CurrentFace,CurrentNewEdge,LastFace->GetOtherSideOfEdge(CurrentNewEdge));
	
	//LastEdge3->EndVertex = Vertices[3];
	
	WeEdge *CurrentExistingEdge = Edges[0];
	
	std::cout << CurrentFace->GetSideOfEdge(Edges[0]) << " " << CurrentFace << " " << Faces[1] << " " << Faces[0] << NewFace << std::endl;
	
	WeCloseFaceIntoVertex(CurrentFace,CurrentNewEdge,CurrentExistingEdge,Vertices[3]);
	
	
	
	LastEdges[0]->EndVertex = Vertices[5];
	
	//LastFace
	WeCloseFace(LastFace,LastEdges[0],CurrentNewEdge);
	
	// Face 1
	WeCloseFace(LastEdges[0]->Side[WE_LEFT].Face,CurrentExistingEdge,LastEdges[0]);
	
	//NewEdges[0]->EndVertex = Vertices[2];
	//NewEdges[1]->EndVertex = Vertices[5];
	
	//Edges[0]->EndVertex = Vertices[3];
	//Edges[1]->EndVertex = Vertices[2];
}


std::vector<lmVector3D> PyramidVertices;
WeMesh Pyramid;

std::vector<lmVector3D> HulyeVertices1;
WeMesh HulyeAlakzat1;

std::vector<lmVector3D> HulyeVertices2;
WeMesh HulyeAlakzat2;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization()
{
	glPointSize(3.0);
	
	CreatePyramid(Pyramid,PyramidVertices);
	CreateHulyeAlakzat1(HulyeAlakzat1,HulyeVertices1);
	CreateHulyeAlakzat2(HulyeAlakzat2,HulyeVertices2);
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
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	
	glPushMatrix();
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_POINTS);
		for(lmVector3D &Vertex : PyramidVertices)
		{
			glVertex3f(Vertex[0],Vertex[1],Vertex[2]);
		}
		glEnd();
		
		DrawEdges(Pyramid);
		
		glTranslatef(40,0,0);
		DrawFacesAlongSucc(Pyramid);
		
		glTranslatef(40,0,0);
		DrawFacesAlongPred(Pyramid);
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_POINTS);
		for(lmVector3D &Vertex : HulyeVertices1)
		{
			glVertex3f(Vertex[0],Vertex[1],Vertex[2]);
		}
		glEnd();
		
		DrawEdges(HulyeAlakzat1);
		
		glTranslatef(40,0,0);
		DrawFacesAlongSucc(HulyeAlakzat1);
		
		glTranslatef(40,0,0);
		DrawFacesAlongPred(HulyeAlakzat1);
	glPopMatrix();
	
	glPushMatrix();
		glColor3f(0.0,0.0,1.0);
		glBegin(GL_POINTS);
		for(lmVector3D &Vertex : HulyeVertices2)
		{
			glVertex3f(Vertex[0],Vertex[1],Vertex[2]);
		}
		glEnd();
		
		DrawEdges(HulyeAlakzat2);
		
		glTranslatef(40,0,0);
		DrawFacesAlongSucc(HulyeAlakzat2);
		
		glTranslatef(40,0,0);
		DrawFacesAlongPred(HulyeAlakzat2);
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

