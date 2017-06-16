
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
md5Anim Anim;
bool Y_Up = true;
bool SmoothNormals = false;
float Angle = 0.0;

std::vector<lrmSkeletalMesh> SkeletalMeshes;

md5SkeletonExtrinsic BindPoseSkeleton;
std::vector<md5SkeletonExtrinsic> Frames;

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
	lrmMd5SkeletonAdapter BindPoseHierarchy(File.BindPoseSkeletonMetadata,false,false,true);
	lrmMd5SkeletonAdapter SkeletonHierarchy(Anim.SkeletonHierarchy,true,true,false);
	//
	static int FrameId = 0;
	//
	for(lrmSkeletalMesh &SkeletalMesh :  SkeletalMeshes)
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
			for(unsigned int i = 0;i < 3;i++)
			{
				lmVector4D TransfVertex;
				lmVector3D &Vertex = SkeletalMesh.BindPoseMesh.Vertices[V[i]];
				//
				for(unsigned int j=0;j < lrmSkeletalMesh::MAX_WEIGHT_COUNT;j++)
				{
					int BoneId = SkeletalMesh.BoneId[V[i]][j];
					float Weight = SkeletalMesh.Weights[V[i]][j];
					//
					if(BoneId != -1)
					{
						lmMatrix4x4 Transform = SkeletonHierarchy.GetTransformation(BoneId,Frames[FrameId]) * BindPoseHierarchy.GetInverseTransformation(BoneId,BindPoseSkeleton);
						//
						TransfVertex += Weight * Transform * lmVector4D({Vertex[0],Vertex[1],Vertex[2],1.0f});
					}
				}
				//
				glVertex3f(TransfVertex[0],TransfVertex[1],TransfVertex[2]);
			}
		}
		//
		glEnd();
	}
	//
	FrameId++;
	if(FrameId >= Frames.size())
	{
		FrameId = FrameId%Frames.size();
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
	//
	glutCreateWindow("Md5 loader test");
	//
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glClearColor(0.0,0.0,0.0,1.0);
	//
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,1.0,1.0,500.0);
	//
	glutDisplayFunc(DisplayFunc);
	glutIdleFunc(IdleFunc);
	//
	atexit(AtExit);
	//
	if(argc < 2)
	{return 1;}
	//
	std::string ModelFilename = argv[1];
	std::ifstream fin;
	fin.open(ModelFilename.c_str());
	//
	if(!fin.is_open())
	{
		return 2;
	}
	//
	md5LoadFile(fin,File);
	//
	if(Y_Up)
	{
		md5Utility::Skeleton_YUp(File.BindPoseSkeleton,BindPoseSkeleton);
	}
	else
	{
		BindPoseSkeleton = File.BindPoseSkeleton;
	}
	//
	SkeletalMeshes.resize(File.Meshes.size());
	for(unsigned int i=0;i < SkeletalMeshes.size();i++)
	{
		md5ExtractSkeletalMesh(File,Y_Up,i,SkeletalMeshes[i]);
	}
	//
	md5LoadAnim(std::cin,Anim);
	//
	Frames.resize(Anim.NumFrames);
	for(unsigned int i=0;i < Frames.size();i++)
	{
		md5ExtractAnimFrame(Anim,Y_Up,i,Frames[i]);
	}
	//
	glutMainLoop();
	return 0;
}
