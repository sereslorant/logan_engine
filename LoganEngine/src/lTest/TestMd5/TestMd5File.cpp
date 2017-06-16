
/*
 * GL és GLUT include-ok
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/*
 * Resource manager include-ok
 */
#include <lCore/lResourceManager/lLegacyResourceManager/lrmSkeletalMesh.h>

/*
 * Md5 loader include-ok
 */
#include <lUtils/Md5Loader/md5Utility.h>
#include <lUtils/Md5Loader/lrmMd5Loader.h>

/*
 * STL include-ok
 */
#include <iostream>
#include <vector>

md5File File;
bool SmoothNormals = false;
float Angle = 0.0;

std::vector<lrmSkeletalMesh> SkeletalMeshes;

void DisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-10.0);
	glRotatef(Angle,0.0,1.0,0.0);
	//
	glColor3f(0.0,1.0,0.0);
	//
	for(lrmSkeletalMesh &SkeletalMesh : SkeletalMeshes)
	{
		glBegin(GL_TRIANGLES);
		//
		for(unsigned int i=0;i < SkeletalMesh.BindPoseMesh.GetMaterialGroup(0).IndexBuffer.size()/3;i++)
		{
			unsigned int V[3];
			V[0] = SkeletalMesh.BindPoseMesh.GetMaterialGroup(0).IndexBuffer[i*3];
			V[1] = SkeletalMesh.BindPoseMesh.GetMaterialGroup(0).IndexBuffer[i*3 + 1];
			V[2] = SkeletalMesh.BindPoseMesh.GetMaterialGroup(0).IndexBuffer[i*3 + 2];
			//
			lmVector3D &Vertex0 = SkeletalMesh.BindPoseMesh.Vertices[V[0]];
			lmVector3D &Vertex1 = SkeletalMesh.BindPoseMesh.Vertices[V[1]];
			lmVector3D &Vertex2 = SkeletalMesh.BindPoseMesh.Vertices[V[2]];
			//
			glVertex3f(Vertex0[0],Vertex0[1],Vertex0[2]);
			glVertex3f(Vertex1[0],Vertex1[1],Vertex1[2]);
			glVertex3f(Vertex2[0],Vertex2[1],Vertex2[2]);
		}
		//
		glEnd();
	}
	//
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

	glutCreateWindow("Md5 loader test");

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

	md5LoadFile(std::cin,File);

	SkeletalMeshes.resize(File.Meshes.size());
	for(unsigned int i=0;i < SkeletalMeshes.size();i++)
	{
		md5ExtractBindPoseMesh(File,true,i,SkeletalMeshes[i].BindPoseMesh);
	}

	glutMainLoop();
	return 0;
}
