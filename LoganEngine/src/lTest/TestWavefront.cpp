
/*
 * GL és GLUT include-ok
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*
 * Wavefront loader include-ok
 */
#include "../lResourceManager/lrmWfLoader.h"

/*
 * STL include-ok
 */
//#include <fstream>
#include <iostream>


lrmWfObj Obj;
bool SmoothNormals = false;
float Angle = 0.0;

void DrawWfObj(lrmWfObj &obj)
{
	for(lrmWfObj::lrmWfMatGroup *MatGroup : obj.MatGroups)
	{
		glBegin(GL_TRIANGLES);

		for(lrmWfObj::lrmWfTriangle &Triangle : MatGroup->Triangles)
		{
			lmVector3D &Vertex0 = obj.Vertices[Triangle.V1];
			lmVector3D &Vertex1 = obj.Vertices[Triangle.V2];
			lmVector3D &Vertex2 = obj.Vertices[Triangle.V3];
			//
			glVertex3f(Vertex0[0],Vertex0[1],Vertex0[2]);
			glVertex3f(Vertex1[0],Vertex1[1],Vertex1[2]);
			glVertex3f(Vertex2[0],Vertex2[1],Vertex2[2]);
		}

		glEnd();
	}
}

void DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-10.0);
	glRotatef(Angle,0.0,1.0,0.0);

	glColor3f(0.0,1.0,0.0);

	DrawWfObj(Obj);

	glutSwapBuffers();
}

void IdleFunc()
{
	Angle += 1.0;
	glutPostRedisplay();
}

void AtExit()
{
	//std::cout << "Exiting program" << std::endl;
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutCreateWindow("Wavefront loader test");

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glClearColor(0.0,0.0,0.0,1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,1.0,1.0,500.0);

	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(IdleFunc);

	atexit(AtExit);
	/*
	std::ifstream File("test/TestWavefront/Files/barrel.obj");

	if(File.is_open())
	{*/
		//std::cout << "File opened" << std::endl;
		lrmWfLoader WfLoader(Obj,/*File*/std::cin,SmoothNormals);
	/*}*/

	glutMainLoop();
	return 0;
}

