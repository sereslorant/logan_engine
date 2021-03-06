
#include "lrGL3SceneCache.h"

class lAllocateStaticMeshInstances : public liMeshCountOperation
{
private:
	lrGL3TextureList &TextureList;
	unsigned int CurrentMeshId=0;
	//
public:
	//
	virtual void PerformOperation(const std::string &mesh_name,unsigned int mesh_count) override
	{
		TextureList.InitializeStaticMeshInstances(CurrentMeshId,mesh_name,mesh_count);
		//
		CurrentMeshId++;
	}
	//
	lAllocateStaticMeshInstances(lrGL3TextureList &texture_list)
		:TextureList(texture_list)
	{}
	//
	virtual ~lAllocateStaticMeshInstances() override
	{}
	//
	/*
	 * End of class
	 */
};

class lAllocateSkeletalMeshInstances : public liMeshCountOperation
{
private:
	lrGL3TextureList &TextureList;
	unsigned int CurrentMeshId=0;
	//
public:
	//
	virtual void PerformOperation(const std::string &mesh_name,unsigned int mesh_count) override
	{
		TextureList.InitializeSkeletalMeshInstances(CurrentMeshId,mesh_name,mesh_count);
		//
		CurrentMeshId++;
	}
	//
	lAllocateSkeletalMeshInstances(lrGL3TextureList &texture_list)
		:TextureList(texture_list)
	{}
	//
	virtual ~lAllocateSkeletalMeshInstances() override
	{}
	//
	/*
	 * End of class
	 */
};
//
class lAllocateTextureList : public liTextureGroupOperation
{
private:
	lrGLResourceLoader &ResourceLoader;
	//
	std::map<std::string,lrGL3TextureList *> &TextureDictionary;
	std::vector<lrGL3TextureList> &TextureLists;
	//
	unsigned int TextureListId = 0;
	//
public:
	virtual void PerformOperation(const liTextureGroup &texture_group) override
	{
		lrGL3TextureList &TextureList = TextureLists[TextureListId];
		//
		unsigned int NumStaticMeshes = texture_group.NumStaticMeshes();
		unsigned int NumSkeletalMeshes = texture_group.NumSkeletalMeshes();
		TextureList.Construct(	texture_group.GetAlbedoMap(),
					ResourceLoader.GetTexture(texture_group.GetAlbedoMap()),
					"DUMMY",
					ResourceLoader.GetCubemap("DUMMY"),
					texture_group.NumStaticMeshes(),
					texture_group.NumSkeletalMeshes()
					);
		//
		lAllocateStaticMeshInstances AllocateStaticMeshInstances(TextureList);
		lAllocateSkeletalMeshInstances AllocateSkeletalMeshInstances(TextureList);
		texture_group.ForEachStaticMeshCount(AllocateStaticMeshInstances);
		texture_group.ForEachSkeletalMeshCount(AllocateSkeletalMeshInstances);
		//
		TextureDictionary[texture_group.GetAlbedoMap()] = &TextureList;
		TextureListId++;
	}
	//
	lAllocateTextureList(lrGLResourceLoader &resource_loader,std::map<std::string,lrGL3TextureList *> &texture_dictionary,std::vector<lrGL3TextureList> &texture_lists)
		:ResourceLoader(resource_loader),TextureDictionary(texture_dictionary),TextureLists(texture_lists)
	{}
	//
	virtual ~lAllocateTextureList() override
	{}
	//
	/*
	 * End of class
	 */
};

lrGL3SceneCache::lrGL3SceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,/*lrGL3StaticMeshShader &static_mesh_point_light_shader,*//*lrGL3StaticMeshShader &static_mesh_environment_shader,*/lrGLResourceLoader &resource_loader)
	:/*SceneCacher(resource_loader,static_mesh_point_light_shader,*this),*/ProjectionMatrix(lmMatrix4x4::IDENTITY),ViewMatrix(lmMatrix4x4::IDENTITY)//,StaticMeshPointLightShader(static_mesh_point_light_shader),StaticMeshEnvironmentShader(static_mesh_environment_shader)
{
	lrUtils::GetProjectionMatrix(frustum,ProjectionMatrix);
	lrUtils::GetViewMatrix(camera,ViewMatrix);
	//
	CameraPosition = camera.GetPosition();
	//
	Lights.Resize(scene_cache_stats.GetNumLights());
	//
	TextureLists.resize(scene_cache_stats.GetNumTextureGroups());
	//
	lAllocateTextureList AllocateTextureList(resource_loader,TextureDictionary,TextureLists);
	scene_cache_stats.ForEachTextureGroup(AllocateTextureList);
}
