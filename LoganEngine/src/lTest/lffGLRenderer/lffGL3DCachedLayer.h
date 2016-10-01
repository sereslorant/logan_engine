#ifndef LFF_GL_3D_CACHED_LAYER_H
#define LFF_GL_3D_CACHED_LAYER_H

#include "lffGL3DSceneCache.h"

#include "lGL3DLayer.h"

class lffGL3DCacheDrawer
{
private:
	lffGLResourceLoader &ResourceLoader;
	lffGL3DSceneCache &SceneCache;
	//
public:
	//
	void DrawScene()
	{
		for(lffGL3DTextureList &TextureList : SceneCache.TextureLists)
		{
			lffGLTexture *Texture = ResourceLoader.GetTexture(TextureList.DiffuseMap);
			Texture->Enable();

			for(lffGL3DMeshInstances &MeshInstances : TextureList.MeshInstances)
			{
				MeshInstances.MtlGroup.DrawInstanced(	&MeshInstances.MvMatrices[0],
														&MeshInstances.Materials[0],
														MeshInstances.NumInstances()
														);
			}

			Texture->Disable();
		}
	}
	//
	lffGL3DCacheDrawer(lffGLResourceLoader &resource_loader,lffGL3DSceneCache &scene_cache)
		:ResourceLoader(resource_loader),SceneCache(scene_cache)
	{}
};

class lffGL3DCachedLayer : public lGL3DLayer
{
private:
	lffGLResourceLoader &ResourceLoader;
	//
	virtual void DrawScene() override
	{
		if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
		{return;}
		//
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		//
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//
		GLfloat LightPosition[] = {0.0,0.0,0.0,1.0};
		GLfloat LightDiffuse[] = {1.0,1.0,1.0,1.0};
		glLightfv(GL_LIGHT0,GL_POSITION,LightPosition);
		glLightfv(GL_LIGHT0,GL_DIFFUSE,LightDiffuse);
		//
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		lmMatrix4x4 ProjectionMatrix(lmMatrix4x4::IDENTITY);
		lffGLUtils::GetProjectionMatrix(*Frustum,ProjectionMatrix);
		//
		glMultMatrixf(ProjectionMatrix[0]);
		//glFrustum(	Frustum->GetLeft(),Frustum->GetRight(),
		//			Frustum->GetBottom(),Frustum->GetTop(),
		//			Frustum->GetNear(),Frustum->GetFar());
		//
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//
		lmMatrix4x4 ViewMatrix(lmMatrix4x4::IDENTITY);
		lffGLUtils::GetViewMatrix(*Camera,ViewMatrix);
		//
		glMultMatrixf(ViewMatrix[0]);
		//glRotatef(Camera->GetPitch()*(180/PI),1.0,0.0,0.0);
        //glRotatef(Camera->GetYaw()*(180/PI),0.0,1.0,0.0);
        //glTranslatef(-1.0*Camera->GetPosition()[0],-1.0*Camera->GetPosition()[1],-1.0*Camera->GetPosition()[2]);
        //
        //glColor3f(1.0,1.0,1.0);
        //
        lr3DSceneReader SceneReader(ResourceLoader);
        Scene->Draw(SceneReader);
        //
        lffGL3DSceneCache SceneCache;
        SceneReader.InitializeSceneCache(SceneCache);
        //
        lr3DSceneCacher SceneCacher(ResourceLoader,SceneCache);
        Scene->Draw(SceneCacher);
        //
        //SceneCacher.Print();
        lffGL3DCacheDrawer CacheDrawer(ResourceLoader,SceneCache);
        CacheDrawer.DrawScene();
        //
        /*
        lffGL3DSceneDrawer SceneDrawer(ResourceLoader);
        Scene->Draw(SceneDrawer);
        // */
	}
	//
public:
	//
	lffGL3DCachedLayer(lffGLResourceLoader &resource_loader)
		:ResourceLoader(resource_loader)
	{}
	//
	virtual ~lffGL3DCachedLayer() override
	{}
};


#endif // LFF_GL_3D_CACHED_LAYER_H
