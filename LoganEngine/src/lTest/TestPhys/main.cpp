

#include <iostream>


#include <SDL2/SDL_opengl.h>

const char *TestVertexShader = R"(
#version 330

uniform mat4 MvpMatrix;

in vec2 Position;
in vec2 TexCoord;

out vec2 fTexCoord;

void main()
{
	fTexCoord = TexCoord;
	vec4 TransformedPosition = MvpMatrix * vec4(Position,0.0,1.0);
	//
	gl_Position = vec4(TransformedPosition.xy,0.0,1.0);
}
)";

const char *FragmentShaderSource = R"(
#version 330

in vec2 fTexCoord;

out vec4 FragColor;

void main()  
{
	FragColor = vec4(0.0, fTexCoord.x, fTexCoord.y, 1.0);  
}
)";


#include <lTest/Utils/lSDL2GLTestExtLoader.h>
#include <lTest/Utils/lSDL2TestFramework.h>

/*
 * Shader
 */

PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLDELETESHADERPROC glDeleteShader;

PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

/*
 * Program
 */

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETEPROGRAMPROC glDeleteProgram;

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix4fv;

void InitShaderExtensions(lSDL2ExtensionLoader &ExtensionLoader)
{
	glCreateShader = (PFNGLCREATESHADERPROC) ExtensionLoader.LoadFunction("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) ExtensionLoader.LoadFunction("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC) ExtensionLoader.LoadFunction("glCompileShader");
	glDeleteShader = (PFNGLDELETESHADERPROC) ExtensionLoader.LoadFunction("glDeleteShader");
	//
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) ExtensionLoader.LoadFunction("glGetObjectParameterivARB");
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) ExtensionLoader.LoadFunction("glGetInfoLogARB");
	//
	glCreateProgram = (PFNGLCREATEPROGRAMPROC) ExtensionLoader.LoadFunction("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC) ExtensionLoader.LoadFunction("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC) ExtensionLoader.LoadFunction("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC) ExtensionLoader.LoadFunction("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC) ExtensionLoader.LoadFunction("glUseProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) ExtensionLoader.LoadFunction("glDeleteProgram");
	
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) ExtensionLoader.LoadFunction("glGetUniformLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) ExtensionLoader.LoadFunction("glGetAttribLocation");
	
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) ExtensionLoader.LoadFunction("glUniformMatrix4fv");
}

void PrintShaderCompileError(GLuint shader)
{
	GLint Compiled;
	//
	glGetObjectParameterivARB(shader,GL_COMPILE_STATUS,&Compiled);
	if(!Compiled)
	{
		GLint RetSize;
		char Log[256];
		glGetInfoLogARB(shader,sizeof(Log),&RetSize,Log);
		//
		std::cout << Log << std::endl;
	}
}

#include "GL_Sphere.h"

/*
 * Buffer object
 */

PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;

/*
 * Vertex array object
 */

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

void InitBufferExtensions(lSDL2ExtensionLoader &ExtensionLoader)
{
	glGenBuffers = (PFNGLGENBUFFERSPROC) ExtensionLoader.LoadFunction("glGenBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC) ExtensionLoader.LoadFunction("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC) ExtensionLoader.LoadFunction("glBufferData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) ExtensionLoader.LoadFunction("glDeleteBuffers");

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) ExtensionLoader.LoadFunction("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) ExtensionLoader.LoadFunction("glBindVertexArray");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) ExtensionLoader.LoadFunction("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) ExtensionLoader.LoadFunction("glVertexAttribPointer");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) ExtensionLoader.LoadFunction("glDeleteVertexArrays");
}

#include "lPhys3/lPhys3.h"


#include <stdlib.h>

void DrawShape(GL_Shader &shader,GL_Sphere &sphere,GL_Square &square,lpCollisionShape &collision_shape)
{
	lShapeGetter ShapeVisitor;
	collision_shape.Accept(ShapeVisitor);
	if(ShapeVisitor.GetSphere() != nullptr)
	{
		float Radius = ShapeVisitor.GetSphere()->GetRadius();
		
		lmMatrix4x4 ProjectionMatrix(lmMatrix4x4::IDENTITY);
		lmScale4x4(ProjectionMatrix,lmVector3D({1.0f/60.0f,1.0f/60.0f,0.0f}));
		
		lmMatrix4x4 MvpMatrix = ProjectionMatrix;
		lmTranslate4x4(MvpMatrix,ShapeVisitor.GetSphere()->GetPosition());
		lmScale4x4(MvpMatrix,lmVector3D({Radius,Radius,0.0}));
		
		shader.UploadMvpMatrix(MvpMatrix);
		
		sphere.Draw();
	}
	if(ShapeVisitor.GetComposite() != nullptr)
	{
		for(int i=0;i < ShapeVisitor.GetComposite()->GetNumChildren();i++)
		{
			DrawShape(shader,sphere,square,*ShapeVisitor.GetComposite()->GetChild(i));
		}
	}
}

int main(int argc, char **argv)
{
	lSDL2GLTestFramework GLTestFramework(3,3,600,600);

	lSDL2ExtensionLoader ExtensionLoader;
	
	InitBufferExtensions(ExtensionLoader);
	InitShaderExtensions(ExtensionLoader);
	
	GLint MajorVersion;
	GLint MinorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &MajorVersion); 
	glGetIntegerv(GL_MINOR_VERSION, &MinorVersion);
		std::cout << "gl version: " << MajorVersion << "." << MinorVersion << std::endl;
	
	GL_Shader Shader;
	
	GL_Sphere Sphere(10,Shader);
	GL_Square Square(Shader);
	
	lpSimulationWorld World;
	float SmallRadius = 2.0;
	float BigRadius = 30.0;
	
	std::list<lpBody *> Bodies;
	
	{
		lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({{-40,-40,0.0f},{0.0,0.0,0.0f}}).SetCollisionResponse(lpBody::IMMOVABLE);
		Builder.CreateSphere().SetRadius(BigRadius).Construct();
		
		lpBody *NewBody = Builder.Construct().GetBody();
		Bodies.push_back(NewBody);
	}
	
	{
		lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({{40,-50,0.0f},{0.0,0.0,0.0f}}).SetCollisionResponse(lpBody::IMMOVABLE);
		Builder.CreateSphere().SetRadius(BigRadius).Construct();
		
		lpBody *NewBody = Builder.Construct().GetBody();
		Bodies.push_back(NewBody);
	}
	
	{
		lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({{80,-20,0.0f},{0.0,0.0,0.0f}}).SetCollisionResponse(lpBody::IMMOVABLE);
		Builder.CreateSphere().SetRadius(BigRadius).Construct();
		
		lpBody *NewBody = Builder.Construct().GetBody();
		Bodies.push_back(NewBody);
	}
	
	{
		lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({{0.0,-60,0.0f},{0.0,0.0,0.0f}}).SetCollisionResponse(lpBody::IMMOVABLE);
		Builder.CreateSphere().SetRadius(BigRadius).Construct();
		
		lpBody *NewBody = Builder.Construct().GetBody();
		Bodies.push_back(NewBody);
	}
	
	{
		lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({{0.0,60,0.0f},{0.0,0.0,0.0f}}).SetCollisionResponse(lpBody::IMMOVABLE);
		Builder.CreateSphere().SetRadius(BigRadius).Construct();
		
		lpBody *NewBody = Builder.Construct().GetBody();
		Bodies.push_back(NewBody);
	}
	
	bool Running = true;
	int j=0;
	const int NUM_SAMPLES = 10;
	int Next = 0;
	float dAngle = PI / NUM_SAMPLES;
	while(Running)
	{
		//Input
		SDL_Event Event;
		while(SDL_PollEvent(&Event))
		{
			if(Event.type == SDL_QUIT)
			{
				Running = false;
			}
		}
		
		//Logic
		if(j == 20)
		{
			if(Next == NUM_SAMPLES / 2)
			{
				lmVector3D NewPos = lmVector3D({std::cos(Next * dAngle),std::sin(Next * dAngle),0.0}) * 20.0f;
				
				lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({NewPos,{0.0,0.0,0.0f}});
				
				lmVector3D NewPos1 = lmVector3D({0.0,0.0,0.0});
				lmVector3D NewPos2 = lmVector3D({0.0,3.0f,0.0});
				lmVector3D NewPos3 = lmVector3D({3.0f,0.0,0.0});
				
				lpCompositeBuilder &CompositeBuilder = Builder.CreateComposite();
				
				CompositeBuilder.CreateSphere().SetPosition(NewPos1).SetRadius(SmallRadius).Construct();
				CompositeBuilder.CreateSphere().SetPosition(NewPos2).SetRadius(SmallRadius).Construct();
				CompositeBuilder.CreateSphere().SetPosition(NewPos3).SetRadius(SmallRadius).Construct();
				CompositeBuilder.Construct();
				
				lpBody *NewBody = Builder.Construct().GetBody();
				Bodies.push_back(NewBody);
			}
			else
			{
				lmVector3D NewPos = lmVector3D({std::cos(Next * dAngle),std::sin(Next * dAngle),0.0}) * 20.0f;
				
				lpBodyBuilder &Builder = World.GetBodyFactory().CreateBody().SetState({NewPos,{0.0,0.0,0.0f}});
				Builder.CreateSphere().SetRadius(SmallRadius).Construct();
				
				lpBody *NewBody = Builder.Construct().GetBody();
				Bodies.push_back(NewBody);
			}
			Next++;
			j = 0;
			
			if(Next > NUM_SAMPLES)
			{
				Next = 0;
			}
		}
		else
		{
			j++;
		}
		
		World.Simulate(1.0/60.0);
		
		//Rendering
		glClearColor(1.0, 1.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
		
		Shader.UseProgram();
		
		for(lpBody *Body : Bodies)
		{
			DrawShape(Shader,Sphere,Square,Body->GetCollisionShape());
		}
		
		Shader.DisableProgram();
		
		GLTestFramework.SwapWindow();
	}
    
    return 0;
}
