
#include "lrGL3DCachedLayer.h"

#include "lrGLModelCacher.h"

#include "../../../lr3DSceneReader.h"

void lrGL3DCachedLayer::DrawScene()
{
	if(Frustum == nullptr || Camera == nullptr || Scene == nullptr)
	{return;}
	//
	lrGLModelCacher ModelCacher(ResourceLoader);
	lr3DSceneCacheStats SceneCacheStats(ModelCacher);
	lr3DSceneReader SceneReader(SceneCacheStats);
	//
	auto Before = std::chrono::steady_clock::now();
	Scene->Draw(SceneReader);
	auto After = std::chrono::steady_clock::now();
	//
	std::chrono::nanoseconds Difference = After - Before;
	std::cout << "Stat time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
	//
	lrGL3SceneCache SceneCache(*Frustum,*Camera,SceneCacheStats,ResourceLoader);
	//
	lrGL3SceneCacher SceneCacher(ResourceLoader,RenderState3D.GetStaticMeshPointLightShader(),SceneCache);
	//
	Before = std::chrono::steady_clock::now();
	lr3DSceneReader SceneCacherWrapper(SceneCacher);
	Scene->Draw(SceneCacherWrapper);
	After = std::chrono::steady_clock::now();
	//
	Difference = After - Before;
	std::cout << "Copy time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
	//
	Before = std::chrono::steady_clock::now();
	RenderState3D.DrawScene(SceneCache);
	After = std::chrono::steady_clock::now();
	//
	Difference = After - Before;
	std::cout << "Draw time = " << Difference.count() / 1000000.0 << "ms" << std::endl;
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
		SceneCache.Print();
	#endif
}
