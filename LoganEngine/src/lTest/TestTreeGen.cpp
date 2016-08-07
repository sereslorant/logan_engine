
/*
 * GL és GLUT include-ok
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class IDemo
{
public:
	//
	IDemo() {}
	virtual ~IDemo() {}
	//
	virtual void Draw() = 0;
};

#include "../TreeGen/Turtles/TurtleInterpreter.hpp"
#include "../TreeGen/Turtles/Turtle2D.hpp"


template<typename Type_T,unsigned int Dim_T>
class DrawSkeleton : public ITreeVisitor<Type_T,Dim_T>,public ITreeNode<Type_T,Dim_T>::ITreeOperation
{
private:

	class DrawSegment : public ITreeVisitor<Type_T,Dim_T>,public ITreeNode<Type_T,Dim_T>::ITreeOperation
	{
	private:
		ITrunkNode<Type_T,Dim_T> *Parent = nullptr;

	public:

		void SetParent(ITrunkNode<Type_T,Dim_T> *parent)
		{
			Parent = parent;
		}

		virtual void Operation(ITreeNode<Type_T,Dim_T> *node) override
		{
			node->Accept(this);
		}

		virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) override
		{
			const lmVectorND<Type_T,Dim_T> &p_pos = Parent->GetPosition();
			const lmVectorND<Type_T,Dim_T> &c_pos = node->GetPosition();

			if(Dim_T == 2)
			{
				glVertex2f(p_pos[0],p_pos[1]);
				glVertex2f(c_pos[0],c_pos[1]);
			}
			else if (Dim_T == 3)
			{
				glVertex3f(p_pos[0],p_pos[1],p_pos[2]);
				glVertex3f(c_pos[0],c_pos[1],c_pos[2]);
			}
		}

		virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) override
		{

		}

		DrawSegment()
		{}

		virtual ~DrawSegment() override {}
	};

	DrawSegment Puszcsy;
	bool TrunkFound = false;

public:

	virtual void Operation(ITreeNode<Type_T,Dim_T> *node) override
	{
		node->Accept(this);

		if(TrunkFound)
		{
			node->ForeachChildren(&Puszcsy);
			TrunkFound = false;
		}
	}

	virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) override
	{
		//DrawSegment Puszcsy(node);
		Puszcsy.SetParent(node);
		TrunkFound = true;

		//node->ForeachChildren(&Puszcsy);

		/*for(auto I = node->GetChildrenBegin();I != node->GetChildrenEnd();I++)
		{
			(*I)->Accept(&Puszcsy);
		}*/
	}

	virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) override
	{
		glEnd();

		glBegin(GL_TRIANGLE_FAN);
		for(int i=0;i < node->Size() /*- 1*/;i++)
		{
			const lmVectorND<Type_T,Dim_T> &pos1 = node->GetVertex(i);
			//const lmVector2D &pos2 = node->GetVertex(i+1);

			if(Dim_T == 2)
			{
				glVertex2f(pos1[0],pos1[1]);
			}
			else if(Dim_T == 3)
			{
				glVertex3f(pos1[0],pos1[1],pos1[2]);
			}

			//glVertex2f(pos1[0],pos1[1]);
			//glVertex2f(pos2[0],pos2[1]);
		}
		glEnd();

		glBegin(GL_LINES);
	}

	void Draw(ITreeNode<Type_T,Dim_T> *node)
	{
		glColor3f(0.0,0.0,0.0);
		glBegin(GL_LINES);
		/*
		std::list<TreeNode *> NodeList;
		NodeList.push_back(node);

		for(auto I = NodeList.begin();I != NodeList.end();)
		{
			(*I)->Accept(this);

			for(auto J = (*I)->GetChildrenBegin();J != (*I)->GetChildrenEnd();J++)
			{
				NodeList.push_back(*J);
			}

			I++;
			NodeList.pop_front();
		}
		*/
		BreadthFirstSearch<Type_T,Dim_T,std::list<ITreeNode<Type_T,Dim_T> *> >(node,this);

		glEnd();
	}

	DrawSkeleton(){}
	virtual ~DrawSkeleton(){}
};

typedef DrawSkeleton<double,2> DrawSkeleton2D;
typedef DrawSkeleton<double,3> DrawSkeleton3D;

class DrawCurve : public IDemo
{
protected:
	Turtle2D<double> Turtle = Turtle2D<double>(0.0f,0.0f);
	TurtleInterpreter<IsTurtle2DCommand> Turtle2DInterpreter;

	ITreeNode<double,2> *CurveSkeleton = nullptr;

public:


	virtual void Draw() override
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluOrtho2D(-256.0,256.0,-256.0,+256.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(0.0,64.0,0.0);

		DrawSkeleton2D DrawSkel;
		DrawSkel.Draw(CurveSkeleton);
	}

	DrawCurve()
	{
		char From2D[9]	= {'F','f','+','-','[',']','{','}','.'};
		char To2D[9]	= {'F','f','+','-','[',']','{','}','.'};

		for(int i=0;i < 9;i++)
		{
			Turtle2DInterpreter.AddInterpretation(From2D[i],To2D[i]);
		}
	}

	virtual ~DrawCurve() override
	{
		delete CurveSkeleton;
	}
};

#include "../TreeGen/L_Systems/DOL_System.h"

class DrawQuadraticKochCurve : public DrawCurve
{
public:

	DrawQuadraticKochCurve()
	{
		DOL_System QuadraticKochIsland; //Edge rewriting
		QuadraticKochIsland.AddProduction('F',DOL_System::Production("F - F + F + FF - F - F + F"));

		std::string Axiom = "F-F-F-F";

		std::string Str = Axiom;
		for(int i=0;i < 3;i++)
		{
			std::string Tmp;
			QuadraticKochIsland.ApplyProductions(Str,Tmp);
			Str = Tmp;
		}

		std::string Command;
		Turtle2DInterpreter.Convert(Str,Command);

		Turtle.Set_dAngle(PI/2.0);
		Turtle.Set_dDist(2.0);

		Turtle.ExecuteCommands(Command,CurveSkeleton);
	}

	virtual ~DrawQuadraticKochCurve() override
	{}
};

class DrawDragonCurve : public DrawCurve
{
public:

	DrawDragonCurve()
	{
		DOL_System DragonCurve; //Node rewriting
		DragonCurve.AddProduction('l',DOL_System::Production("l+rF+"));
		DragonCurve.AddProduction('r',DOL_System::Production("-Fl-r"));

		std::string Axiom = "Fl";

		std::string Str = Axiom;
		for(int i=0;i < 12;i++)
		{
			std::string Tmp;
			DragonCurve.ApplyProductions(Str,Tmp);
			Str = Tmp;
		}

		std::string Command;
		Turtle2DInterpreter.Convert(Str,Command);

		Turtle.Set_dAngle(PI/2.0);
		Turtle.Set_dDist(2.5);
		Turtle.ExecuteCommands(Command,CurveSkeleton);
	}

	virtual ~DrawDragonCurve() override
	{}
};

class DrawCordateLeaf : public DrawCurve
{
public:

	DrawCordateLeaf()
	{
		Turtle2DInterpreter.AddInterpretation('G','f');
		//
		DOL_System CordateLeaf; //Node rewriting
		CordateLeaf.AddProduction('A',DOL_System::Production("[+A{.].C.}"));
		CordateLeaf.AddProduction('B',DOL_System::Production("[-B{.].C.}"));
		CordateLeaf.AddProduction('C',DOL_System::Production("GC"));

		std::string Axiom = "[A][B]";

		std::string Str = Axiom;
		for(int i=0;i < 10;i++)
		{
			std::string Tmp;
			CordateLeaf.ApplyProductions(Str,Tmp);
			Str = Tmp;
		}

		//std::cout << Str << std::endl;

		std::string Command;
		Turtle2DInterpreter.Convert(Str,Command);

		//std::cout << Command << std::endl;

		Turtle.Set_dAngle(PI/8.0);
		Turtle.Set_dDist(10.0);
		Turtle.ExecuteCommands(Command,CurveSkeleton);
	}

	virtual ~DrawCordateLeaf() override
	{}
};

#include "../TreeGen/Turtles/Turtle3D.hpp"

class Demo3D : public IDemo
{
protected:
	Turtle3D<double> Turtle = Turtle3D<double>(lmVectorND<double,3>({1.0,0.0,0.0}),lmVectorND<double,3>({0.0,1.0,0.0}),2.0,PI/2.0);
	TurtleInterpreter<IsTurtle3DCommand> Turtle3DInterpreter;

public:

	Demo3D()
	{
		char From3D[14]	= {'F','f','^','&','+','-','\\','/','|','[',']','{','}','.'};
		char To3D[14]	= {'F','f','(',')','<','>','+' ,'-','|','[',']','{','}','.'};

		for(int i=0;i < 14;i++)
		{
			Turtle3DInterpreter.AddInterpretation(From3D[i],To3D[i]);
		}
	}

	virtual ~Demo3D() override
	{}
};


float X = 20.0;
float Y = -20.0;
float Z = 20.0;

float CameraAngle = 0.0;

#include "../TreeGen/TrunkGenerator.hpp"

template<typename Type_T>
void DrawSpline(const liCurve<Type_T,3> &curve,Type_T t_begin,Type_T t_end,Type_T dt)
{
	glBegin(GL_LINES);

	glColor3f(1.0,0.75,0.0);

	Type_T t = t_begin;
	lmVectorND<Type_T,3> Begin = curve.GetPoint(t);
	glVertex3f(Begin[0],Begin[1],Begin[2]);

	t += dt;
	lmVectorND<Type_T,3> V = curve.GetPoint(t);
	glVertex3f(V[0],V[1],V[2]);

	while(t < t_end)
	{
		glColor3f(1.0,0.75,0.0);

		glVertex3f(V[0],V[1],V[2]);

		V = curve.GetPoint(t);
		glVertex3f(V[0],V[1],V[2]);
		/*
		glm::vec3 V1;
		glm::vec3 V2;
		glm::vec3 V3;

		const bool FRENET_FRAME = false;

		if(FRENET_FRAME)
		{
			ICurve::GetFrenetFrame(curve,t,&V1,&V2,&V3,true);
		}
		else
		{
			ICurve::GetExplicitUpFrame(curve,UP,t,1.0/25.0,&V1,&V2,&V3,true);
		}

		V1 = V + 5.0f*glm::normalize(V1);
		V2 = V + 5.0f*glm::normalize(V2);
		V3 = V + 5.0f*glm::normalize(V3);

		glColor3f(1.0,0.0,0.0);

		glVertex3f(V.x,V.y,V.z);
		glVertex3f(V1.x,V1.y,V1.z);

		glColor3f(0.0,1.0,0.0);

		glVertex3f(V.x,V.y,V.z);
		glVertex3f(V2.x,V2.y,V2.z);

		glColor3f(0.0,0.0,1.0);

		glVertex3f(V.x,V.y,V.z);
		glVertex3f(V3.x,V3.y,V3.z);
		*/

		t += dt;
	}

	//glVertex3f(Begin[0],Begin[1],Begin[2]);

	glEnd();
}

template<typename Type_T>
class lmGeneralizedCylinder
{
public:
	bool TEST_FrenetFrame = false;

	liCurve<Type_T,3> &Spine;
	Type_T Radius;

	lmVectorND<Type_T,3> Up;

	lmVectorND<Type_T,3> GetPoint(Type_T spine_param,Type_T angle) const
	{
		lmVectorND<Type_T,3> i,j,k;

		if(TEST_FrenetFrame)
		{
			lmGetFrenetFrame(Spine,spine_param,&i,&j,&k,true);
		}
		else
		{
			lmGetExplicitUpFrame(Spine,Up,spine_param,/*1.0/30.0,*/&i,&j,&k,true);
		}

		lmVectorND<Type_T,3> DiskPoint = std::cos(angle)*i + std::sin(angle)*k;

		return Spine.GetPoint(spine_param) + Radius * DiskPoint;
	}

	lmGeneralizedCylinder(liCurve<Type_T,3> &spine,Type_T radius)
		:Spine(spine),Radius(radius)
	{
		Up[0] = 0.0;
		Up[1] = 1.0;
		Up[2] = 0.0;
	}

	~lmGeneralizedCylinder()
	{}
};

void DrawTrunkSpline(TreeTrunk<double,3> *trunk)
{
	for(auto I : trunk->Branches)
	{
		DrawSpline(*I->Curve,I->t_Begin,I->t_End,(I->t_End - I->t_Begin)/20.0);
	}
}

constexpr unsigned int CYLINDER_ARRAY_WIDTH = 6;
constexpr unsigned int CYLINDER_ARRAY_HEIGHT = 6;

template<typename Type_T>
void DrawGeneralizedCylinder(const lmGeneralizedCylinder<Type_T> &cylinder,Type_T t0,Type_T t_fin)
{
	lmVectorND<Type_T,3> CylinderArray[CYLINDER_ARRAY_HEIGHT][CYLINDER_ARRAY_WIDTH];

	Type_T dt = (t_fin - t0)/(CYLINDER_ARRAY_HEIGHT - 1);

	Type_T dAngle = 2.0*PI/CYLINDER_ARRAY_WIDTH;

	for(unsigned int i=0;i < CYLINDER_ARRAY_HEIGHT;i++)
	{
		for(unsigned int j=0;j < CYLINDER_ARRAY_WIDTH;j++)
		{
			CylinderArray[i][j] = cylinder.GetPoint(t0 + i*dt,j*dAngle);
		}
	}

	glColor3f(1.0,0.0,1.0);

	glBegin(GL_QUADS);

	for(unsigned int i=0;i < CYLINDER_ARRAY_HEIGHT - 1;i++)
	{
		for(unsigned int j=0;j < CYLINDER_ARRAY_WIDTH;j++)
		{
			unsigned int I = (i+1);
			unsigned int J = (j+1) % CYLINDER_ARRAY_WIDTH;

			glVertex3f(CylinderArray[i][j][0],CylinderArray[i][j][1],CylinderArray[i][j][2]);
			glVertex3f(CylinderArray[I][j][0],CylinderArray[I][j][1],CylinderArray[I][j][2]);
			glVertex3f(CylinderArray[I][J][0],CylinderArray[I][J][1],CylinderArray[I][J][2]);
			glVertex3f(CylinderArray[i][J][0],CylinderArray[i][J][1],CylinderArray[i][J][2]);
		}
	}

	glEnd();
}


void DrawTrunkCylinder(TreeTrunk<double,3> *trunk)
{
	//std::cout << "Begin" << std::endl;
	for(auto I : trunk->Branches)
	{
		//std::cout << I->Depth << std::endl;
		DrawGeneralizedCylinder(lmGeneralizedCylinder<double>(*I->Curve,2.0*pow(0.6,I->Depth)),I->t_Begin,I->t_End);
	}
	//
	GLUquadricObj *Quadric = gluNewQuadric();
	gluQuadricDrawStyle(Quadric,GLU_FILL);
	//
	for(auto I : trunk->Junctions)
	{
		lmVectorND<double,3> Pos;
		double Radius;
		if(I->Root == nullptr)
		{
			Radius = 2.0;
			glPushMatrix();
				gluSphere(Quadric,Radius,8,8);
			glPopMatrix();
		}
		else
		{
			Pos = I->Root->Curve->GetPoint(I->Root->t_End);
			Radius = 2.0;

			glPushMatrix();
				glTranslatef(Pos[0],Pos[1],Pos[2]);
				gluSphere(Quadric,Radius,8,8);
			glPopMatrix();
		}
	}
	//
	gluDeleteQuadric(Quadric);
}

bool DrawSplineFlag = true;
bool DrawCylinderFlag = true;

class DrawCurve3D : public Demo3D
{
protected:
	ITreeNode<double,3> *CurveSkeleton = nullptr;
	//
	TreeTrunk<double,3> *Result = nullptr;
	//
public:

	virtual void Draw() override
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(90,1.0,1.0,-200.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(X,Y,Z);

		glRotatef(CameraAngle,0.0,1.0,0.0);
		glRotatef(-90.0,1.0,0.0,0.0);

		DrawSkeleton3D DrawSkel;
		DrawSkel.Draw(CurveSkeleton);

		if(DrawSplineFlag && Result != nullptr)
		{
			DrawTrunkSpline(Result);
		}

		if(DrawCylinderFlag && Result != nullptr)
		{
			DrawTrunkCylinder(Result);
		}
	}

	DrawCurve3D(){}
	virtual ~DrawCurve3D() override
	{
		delete CurveSkeleton;
	}
};

class DrawHilbertCurve : public DrawCurve3D
{
public:

	DrawHilbertCurve()
	{
		DOL_System HilbertCurve;

		HilbertCurve.AddProduction('A',DOL_System::Production("B-F+CFC+F-D&F^D-F+&&CFC+F+B//"));
		HilbertCurve.AddProduction('B',DOL_System::Production("A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//"));
		HilbertCurve.AddProduction('C',DOL_System::Production("|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//"));
		HilbertCurve.AddProduction('D',DOL_System::Production("|CFB-F+B|FA&F^A&&FB-F+B|FC//"));

		std::string Axiom = "A";

		std::string Str = Axiom;
		for(int i=0;i < 4;i++)
		{
			std::string Tmp;
			HilbertCurve.ApplyProductions(Str,Tmp);
			Str = Tmp;
		}

		std::string Command;
		Turtle3DInterpreter.Convert(Str,Command);

		Turtle.Set_dAngle(PI/2.0);
		Turtle.Set_dDist(2.0);
		Turtle.ExecuteCommands(Command,CurveSkeleton);
	}

	virtual ~DrawHilbertCurve() override
	{}
};

class DrawTree : public DrawCurve3D
{
public:

	DrawTree(unsigned int iterations,int variant)
	{
		DOL_System Tree;

		if(variant == 1)
		{
			Tree.AddProduction('A',DOL_System::Production("[&FL!A]///’[&FL!A]////’[&FL!A]///’[&FL!A]"));
		}
		else if(variant == 2)
		{
			Tree.AddProduction('A',DOL_System::Production("[&FL!A]///’[&FL!A]////’[&FL!A]///’[&FL!A]////’[&FL!A]"));
		}
		else
		{
			Tree.AddProduction('A',DOL_System::Production("[&FL!A]/////’[&FL!A]///////’[&FL!A]"));
		}
		//
		Tree.AddProduction('F',DOL_System::Production("S ///// F"));
		Tree.AddProduction('S',DOL_System::Production("F L"));
		Tree.AddProduction('L',DOL_System::Production("[’’’^^{.-f.+f.+f.-|-f.+f.+f}]"));

		std::string Axiom = "A";

		std::string Str = Axiom;
		for(unsigned int i=0;i < iterations;i++)
		{
			std::string Tmp;
			Tree.ApplyProductions(Str,Tmp);
			Str = Tmp;
		}

		std::string Command;
		Turtle3DInterpreter.Convert(Str,Command);

		Turtle.Set_dAngle((PI/180.0)*22.5);
		Turtle.Set_dDist(7.0);
		Turtle.ExecuteCommands(Command,CurveSkeleton);

		TrunkGenerator<double,3> TrunkGen;
		TrunkGen.Process(CurveSkeleton,Result);
	}

	virtual ~DrawTree() override
	{}
};


unsigned int DemoId = 0;
std::vector<IDemo *> DemoArray;

// Inicializacio, a program futasanak kezdeten, az OpenGL kontextus letrehozasa utan hivodik meg (ld. main() fv.)
void onInitialization() {
	DemoArray.push_back(new DrawQuadraticKochCurve);
	DemoArray.push_back(new DrawDragonCurve);
	DemoArray.push_back(new DrawCordateLeaf);
	DemoArray.push_back(new DrawHilbertCurve);
	//
	for(int j=3;j < 10;j++)
	{
		DemoArray.push_back(new DrawTree(j,0));
	}
	//
}

unsigned int screenWidth = 512;
unsigned int screenHeight = 512;

// Rajzolas, ha az alkalmazas ablak ervenytelenne valik, akkor ez a fuggveny hivodik meg
void onDisplay()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		// torlesi szin beallitasa
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // kepernyo torles

	glViewport(0, 0, screenWidth, screenHeight);

	/*if(DemoId < DemoArray.size())
	{
		DemoArray[DemoId]->Draw();
	}
	else
	{*/
		DemoArray[DemoId%DemoArray.size()]->Draw();
	//}

    glutSwapBuffers();     				// Buffercsere: rajzolas vege

}

// Billentyuzet esemenyeket lekezelo fuggveny (lenyomas)
void onKeyboard(unsigned char key, int x, int y)
{
	if(tolower(key) == '1')
	{
		DemoId--;
	}
	if(tolower(key) == '2')
	{
		DemoId++;
	}

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
void onMouseMotion(int x, int y)
{

}

// `Idle' esemenykezelo, jelzi, hogy az ido telik, az Idle esemenyek frekvenciajara csak a 0 a garantalt minimalis ertek
void onIdle( ) {


	glutPostRedisplay( ); // Ilyenkor rajzold ujra a kepet
}

// ...Idaig modosithatod
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

