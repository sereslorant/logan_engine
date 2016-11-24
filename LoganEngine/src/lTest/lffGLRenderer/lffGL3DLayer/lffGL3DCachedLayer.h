#ifndef LFF_GL_3D_CACHED_LAYER_H
#define LFF_GL_3D_CACHED_LAYER_H

#include "../../../lRenderer/lr3DSceneReader.h"

//#define L_DEBUG_PRINT_SCENE_CACHE

#include "lffGL3DSceneCache.h"
#include "../../../lRenderer/lrLayer.h"

/*
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
		glClearColor(0.0,0.0,0.0,1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE,GL_ONE);
		//
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//
		// /*
		bool First = true;
		//
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		// * /
		glEnable(GL_TEXTURE_2D);
		//
		for(unsigned int i=0;i < SceneCache.Lights.Lights.size();i++)
		{
			SceneCache.Lights.Lights[i].Apply();
			//
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMultMatrixf(SceneCache.ProjectionMatrix[0]);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glMultMatrixf(SceneCache.ViewMatrix[0]);

			for(lffGL3DTextureList &TextureList : SceneCache.TextureLists)
			{
				lffGLTexture *Texture = ResourceLoader.GetTexture(TextureList.DiffuseMap);
				Texture->Enable();

				for(lffGL3DMeshInstances &MeshInstances : TextureList.MeshInstances)
				{
					MeshInstances.DrawInstanced();
				}

				Texture->Disable();
			}
			// /*
			if(First)
			{
				//glDisable(GL_BLEND);
				glDepthFunc(GL_LEQUAL);
				First = false;
			}
			/*else
			{
				glDepthFunc(GL_EQUAL);
				//glEnable(GL_BLEND);
				//glBlendEquation(GL_FUNC_ADD);
				//glBlendFunc(GL_ONE,GL_ONE);
			}* /
			// * /
		}
	}
	//
	lffGL3DCacheDrawer(lffGLResourceLoader &resource_loader,lffGL3DSceneCache &scene_cache)
		:ResourceLoader(resource_loader),SceneCache(scene_cache)
	{}
};
*/
class lffGL3DCachedLayer : public lr3DLayer
{
private:
	lffGLResourceLoader &ResourceLoader;
	//
	virtual void DrawScene() override
	{
		if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
		{return;}
		//
		lffGLModelCacher ModelCacher(ResourceLoader);
		lr3DSceneCacheStats SceneCacheStats(ModelCacher);
        lr3DSceneReader SceneReader(SceneCacheStats);
        Scene->Draw(SceneReader);
        //
        lffGL3DSceneCache SceneCache(*Frustum,*Camera,SceneCacheStats,ResourceLoader);
        //
        lr3DSceneReader SceneCacher(SceneCache.GetSceneCacher());
        //
        //lr3DSceneCacher SceneCacher(ResourceLoader,SceneCache);
        Scene->Draw(SceneCacher);
        //
		#ifdef L_DEBUG_PRINT_SCENE_CACHE
        	SceneCache.Print();
		#endif
        //
        SceneCache.DrawScene();
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
