
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

void DisplayFunc()
{
	test->Draw();

	glutSwapBuffers();
}

void IdleFunc()
{
	test->Loop();

	glutPostRedisplay();
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
	test = new RendererTest(Renderer);

	test->Initialize();

	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(IdleFunc);

	atexit(AtExit);

	glutMainLoop();
	return 0;
}
