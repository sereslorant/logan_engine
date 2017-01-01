
#include "lrGL3DCachedLayer.h"

#include "lrGL3SceneCache/lrGL3SceneCache.h"
#include "../../../lr3DSceneReader.h"

void lrGL3DCachedLayer::DrawScene()
{
	if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
	{return;}
	//
	lrGLModelCacher ModelCacher(ResourceLoader);
	lr3DSceneCacheStats SceneCacheStats(ModelCacher);
	lr3DSceneReader SceneReader(SceneCacheStats);
	Scene->Draw(SceneReader);
	//
	lrGL3SceneCache SceneCache(*Frustum,*Camera,SceneCacheStats,StaticMeshPointLightShader,StaticMeshEnvironmentShader,ResourceLoader);
	//
	lr3DSceneReader SceneCacher(SceneCache.GetSceneCacher());
	//
	Scene->Draw(SceneCacher);
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
		SceneCache.Print();
	#endif
	//
	SceneCache.DrawScene();
}
