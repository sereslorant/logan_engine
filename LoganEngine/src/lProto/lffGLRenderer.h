#ifndef LFFGL_RENDERER_H
#define LFFGL_RENDERER_H

#include <SDL2/SDL_opengl.h>

class lffGL2DSceneDrawer : public li2DSceneDrawer, public li2DSceneVisitor
{
public:
	//
	virtual void VisitRectangle(li2DRectangle &rectangle) override
	{
		glBegin(GL_QUADS);

		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1] + rectangle.GetHeight());
		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1] + rectangle.GetHeight());

		glEnd();
	}
	//
	virtual void Draw(li2DElement &element) override
	{
		if(!element.IsHidden())
		{
			glColor3f(element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue());

			element.Accept(*this);
		}
	}
	//
	lffGL2DSceneDrawer(){}
	virtual ~lffGL2DSceneDrawer() override {}
};

#include "../lRenderer/lrLayer.h"

class lffGL2DLayer : public lrLayer, public li2DLayer
{
private:
	li2DScene *Scene = nullptr;
	li2DCamera *Camera = nullptr;

	virtual void DrawScene() override
	{
		if((Scene == nullptr) || (Camera == nullptr))
		{return;}
		//
		glDisable(GL_DEPTH_TEST);
		//
		if(Scene->HasBackgroundColor())
		{
			glClearColor(Scene->GetBackgroundColor().GetRed(),Scene->GetBackgroundColor().GetGreen(),Scene->GetBackgroundColor().GetBlue(),Scene->GetBackgroundColor().GetAlpha());
			glClear(GL_COLOR_BUFFER_BIT);
		}
		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(Camera->GetPosition()[0],
				Camera->GetPosition()[0] + Camera->GetWidth(),
				Camera->GetPosition()[1] + Camera->GetHeight(),
				Camera->GetPosition()[1],
				-1,1);
		//
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//
		lffGL2DSceneDrawer SceneDrawer;
		Scene->Draw(SceneDrawer);
	}

public:

	virtual void Accept(liLayerVisitor &layer_visitor) override
	{
		layer_visitor.Visit2DLayer(*this);
	}

	virtual void SetScene(li2DScene *scene) override
	{
		Scene = scene;
	}

	virtual void SetCamera(li2DCamera *camera) override
	{
		Camera = camera;
	}

	lffGL2DLayer(){}
	virtual ~lffGL2DLayer() override {}
};

#include "../lResourceManager/lResourceManager.h"

class lffGL3DDrawMesh
{
private:
	lrmStaticMesh &StaticMesh;

public:

	void Draw()
	{
		glBegin(GL_TRIANGLES);
		//
		for(lrmStaticMesh::lrmMtlGroup *MtlGroup : StaticMesh.MtlGroups)
		{
			for(unsigned int i=0;i < MtlGroup->IndexBuffer.size();i++)
			{
				unsigned int Index = MtlGroup->IndexBuffer[i];
				//glTexCoord2f(StaticMesh.TexCoords[Index][0],StaticMesh.TexCoords[Index][1]);
				//glNormal3f(StaticMesh.Normals[Index][0],StaticMesh.Normals[Index][1],StaticMesh.Normals[Index][2]);
				glVertex3f(StaticMesh.Vertices[Index][0],StaticMesh.Vertices[Index][1],StaticMesh.Vertices[Index][2]);
			}
		}
		//
		glEnd();
	}

	lffGL3DDrawMesh(lrmStaticMesh &static_mesh)
		:StaticMesh(static_mesh)
	{}
	virtual ~lffGL3DDrawMesh()
	{}
};

class lffGL3DLayer : public lrLayer,public li3DLayer
{
private:
	liFrustum *Frustum = nullptr;
	li3DCamera *Camera = nullptr;
	//
	lrmResourceManager &ResourceManager;

	virtual void DrawScene() override
	{
		if(Frustum == nullptr || Camera == nullptr)
		{return;}

		glEnable(GL_DEPTH_TEST);
		//
		glClearColor(1.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(	Frustum->GetLeft(),Frustum->GetRight(),
					Frustum->GetBottom(),Frustum->GetTop(),
					Frustum->GetNear(),Frustum->GetFar());
		//
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//
		glRotatef(Camera->GetPitch()*(180/PI),1.0,0.0,0.0);
        glRotatef(Camera->GetYaw()*(180/PI),0.0,1.0,0.0);
        glTranslatef(-1.0*Camera->GetPosition()[0],-1.0*Camera->GetPosition()[1],-1.0*Camera->GetPosition()[2]);
		//
		lrmStaticMesh *StaticMesh = ResourceManager.GetStaticMesh("Model");
		//
		lffGL3DDrawMesh DrawMesh(*StaticMesh);
		//
		glColor3f(0.0,1.0,0.0);
		//
		DrawMesh.Draw();
	}

public:

	virtual void Accept(liLayerVisitor &layer_visitor) override
	{
		layer_visitor.Visit3DLayer(*this);
	}

	virtual void SetFrustum(liFrustum *frustum) override
	{
		Frustum = frustum;
	}

	virtual void SetCamera(li3DCamera *camera) override
	{
		Camera = camera;
	}

	//
	lffGL3DLayer(lrmResourceManager &resource_manager)
		:ResourceManager(resource_manager)
	{}
	//
	virtual ~lffGL3DLayer() override
	{}
};

#include "../lRenderer/lrViewport.h"
#include <list>

class lrGLViewport : public lrViewport
{
private:
	std::list<lrLayer *> Layers;
	lrmResourceManager &ResourceManager;

public:

	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = new lffGL2DLayer;
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}

	virtual liLayer *Create3DLayer() override
	{
		lrLayer *NewLayer = new lffGL3DLayer(ResourceManager);
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}

	void Draw()
	{
		glViewport(X,Y,Width,Height);
		//Ez kell, hogy a glClear() csak a viewportot érintse.
		glScissor(X,Y,Width,Height);
		//
		for(lrLayer *Layer : Layers)
		{
			Layer->Draw();
		}
	}

	lrGLViewport(int x,int y,int width,int height,lrmResourceManager &resource_manager)
		:lrViewport(x,y,width,height),ResourceManager(resource_manager)
	{}

	virtual ~lrGLViewport() override
	{
		for(lrLayer *Layer : Layers)
		{
			delete Layer;
		}
	}
};

#include "../lRenderer/lrFramebuffer.h"

class lffGLFramebuffer : public lrFramebuffer
{
private:
	std::list<lrGLViewport *> Viewports;
	lrmResourceManager &ResourceManager;

public:

	virtual liViewport *CreateViewport(int x,int y,int width,int height) override
	{
		lrGLViewport *NewViewport = new lrGLViewport(x,y,width,height,ResourceManager);
		Viewports.push_back(NewViewport);
		//
		return NewViewport;
	}

	void Draw()
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			Viewport->Draw();
		}
	}

	lffGLFramebuffer(unsigned int width,unsigned int height,lrmResourceManager &resource_manager)
		:lrFramebuffer(width,height),ResourceManager(resource_manager)
	{}

	virtual ~lffGLFramebuffer() override
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			delete Viewport;
		}
	}
};

class lffGLRenderer : public liRenderer
{
private:
	lffGLFramebuffer MainFramebuffer;
	//
	lrmResourceManager &ResourceManager;
	//
public:
	//
	virtual liFramebuffer &GetMainFramebuffer() override
	{
		return MainFramebuffer;
	}
	//
	virtual void Render() override
	{
		MainFramebuffer.Draw();
	}

	lffGLRenderer(lrmResourceManager &resource_manager)
		:MainFramebuffer(800,600,resource_manager),ResourceManager(resource_manager)
	{
		glShadeModel(GL_SMOOTH);
		//
		glEnable(GL_BLEND);
		glEnable(GL_SCISSOR_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual ~lffGLRenderer() override
	{
    	//
	}
};

#endif // LFFGL_RENDERER_H
