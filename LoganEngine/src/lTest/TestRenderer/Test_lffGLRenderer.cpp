
/*
 * GL és GLUT include-ok
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*
 * STL include-ok
 */
#include <iostream>

#include "RendererTest.h"
#include "DummyRM.h"

RendererTest *test;
DummyRM *dummyRM;
liRenderer *Renderer;

bool Up = false;
bool Down = false;
bool Left = false;
bool Right = false;

void KeyboardFunc(unsigned char key,int x,int y)
{
	if(key == 'w')
	{Up = true;}
	if(key == 's')
	{Down = true;}
	if(key == 'a')
	{Left = true;}
	if(key == 'd')
	{Right = true;}
}

void KeyboardUpFunc(unsigned char key,int x,int y)
{
	if(key == 'w')
	{Up = false;}
	if(key == 's')
	{Down = false;}
	if(key == 'a')
	{Left = false;}
	if(key == 'd')
	{Right = false;}
}

void IdleFunc()
{
	test->Input(Up,Down,Left,Right);
	//
	test->Loop();
	//
	glutPostRedisplay();
}

void DisplayFunc()
{
	test->Draw();
	//
	glutSwapBuffers();
}

void AtExit()
{
	//std::cout << "Exiting program" << std::endl;
	delete test;
	delete Renderer;
	delete dummyRM;
}

#include "../../lTest/lffGLRenderer/lffGLRenderer.h"

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutCreateWindow("Renderer test");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	dummyRM = new DummyRM;
	Renderer = new lffGLRenderer(600,600,*dummyRM);
	test = new RendererFunctionalityTest(Renderer);

	test->Initialize();

	glutKeyboardFunc(KeyboardFunc);
	glutKeyboardUpFunc(KeyboardUpFunc);
	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(IdleFunc);

	atexit(AtExit);

	glutMainLoop();
	return 0;
}
