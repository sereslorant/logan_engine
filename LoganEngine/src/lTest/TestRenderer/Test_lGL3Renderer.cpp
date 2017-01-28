
/*
 * GLFW include-ok
 */
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

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

void GetInput(GLFWwindow *window)
{
	int state = glfwGetKey(window,GLFW_KEY_UP);
	if(state == GLFW_PRESS)
	{Up = true;}
	else
	{Up = false;}
	//
	state = glfwGetKey(window,GLFW_KEY_DOWN);
	if(state == GLFW_PRESS)
	{Down = true;}
	else
	{Down = false;}
	//
	state = glfwGetKey(window,GLFW_KEY_LEFT);
	if(state == GLFW_PRESS)
	{Left = true;}
	else
	{Left = false;}
	//
	state = glfwGetKey(window,GLFW_KEY_RIGHT);
	if(state == GLFW_PRESS)
	{Right = true;}
	else
	{Right = false;}
}

void AtExit()
{
	//std::cout << "Exiting program" << std::endl;
	delete test;
	delete Renderer;
	delete dummyRM;
}

#include "../../lRenderer/lGLRenderer/lGLExt.h"

class lGLFW3ExtensionLoader : public liGLExtLoader::liExtFunctionLoader
{
private:

public:

	virtual void *LoadFunction(const std::string &function_name) override
	{
		return (void *)glfwGetProcAddress(function_name.c_str());
	}

	lGLFW3ExtensionLoader()
	{}

	virtual ~lGLFW3ExtensionLoader() override
	{}
};

//#include "../../lTest/lffGLRenderer/lffGLRenderer.h"
#include "../../lRenderer/lGLRenderer/lGL3Renderer/lrGL3Renderer.h"

int main(int argc, char *argv[])
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	GLFWwindow *Window = glfwCreateWindow(600,600,"Renderer test",nullptr,nullptr);
	glfwMakeContextCurrent(Window);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	lGLFW3ExtensionLoader ExtensionLoader;
	liGLExtLoader &ExtLoader = lGetGLExtLoader();

	ExtLoader.SetExtFunctionLoader(&ExtensionLoader);
	ExtLoader.LoadExtensions();

	ExtLoader.PrintExtStatus(std::cout);

	dummyRM = new DummyRM;
	//Renderer = new lffGLRenderer(600,600,*dummyRM);
	Renderer = new lrGL3Renderer(600,600);
	Renderer->SetResourceManager(dummyRM);
	test = new RendererFunctionalityTest(Renderer);

	test->Initialize();

	atexit(AtExit);

	while(!glfwWindowShouldClose(Window))
	{
		glfwPollEvents();
		GetInput(Window);
		//
		test->Input(Up,Down,Left,Right);
		test->Loop();
		test->Draw();
		//
		glfwSwapBuffers(Window);
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
	return 0;
}
