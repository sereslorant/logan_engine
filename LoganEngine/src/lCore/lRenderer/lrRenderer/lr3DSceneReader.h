#ifndef LR3D_SCENE_READER_H
#define LR3D_SCENE_READER_H

#include <string>
#include <map>

#include <lScene/l3DScene/li3DScene.h>

class li3DSceneCache
{
public:
	virtual void CacheStaticMesh(const li3DMesh &mesh,const li3DStaticMesh &static_mesh) = 0;
	virtual void CacheSkeletalMesh(const li3DMesh &mesh,const li3DSkeletalMesh &skeletal_mesh) = 0;
	virtual void CacheLight(const li3DLight &light) = 0;
	//
	li3DSceneCache()
	{}
	//
	virtual ~li3DSceneCache()
	{}
};

class lr3DSceneReader : public li3DSceneDrawer, public li3DElementVisitor
{
private:
	//
	class lr3DMeshReader : public li3DMeshVisitor
	{
	private:
		const li3DMesh &Mesh;
		li3DSceneCache &SceneCache;
		//
	public:
		//
		virtual void VisitStaticMesh(const li3DStaticMesh &static_mesh) override
		{
			SceneCache.CacheStaticMesh(Mesh,static_mesh);
		}
		//
		virtual void VisitSkeletalMesh(const li3DSkeletalMesh &skeletal_mesh) override
		{
			SceneCache.CacheSkeletalMesh(Mesh,skeletal_mesh);
		}
		//
		lr3DMeshReader(const li3DMesh &mesh,li3DSceneCache &scene_cache)
			:Mesh(mesh),SceneCache(scene_cache)
		{}
		//
		virtual ~lr3DMeshReader() override
		{}
	};
	//
	li3DSceneCache &SceneCache;
	//
public:
	//
	virtual void VisitMesh(const li3DMesh &mesh) override
	{
		lr3DMeshReader MeshReader(mesh,SceneCache);
		//
		mesh.Accept(MeshReader);
	}
	//
	virtual void VisitLight(const li3DLight &light) override
	{
		SceneCache.CacheLight(light);
	}
	//
	virtual void Draw(const li3DElement &element) override
	{
		element.Accept(*this);
	}
	//
	lr3DSceneReader(li3DSceneCache &scene_cache)
		:SceneCache(scene_cache)
	{}
	//
	virtual ~lr3DSceneReader() override
	{}
	//
	/*
	 * End of class
	 */
};

class liMeshStats
{
public:
	virtual void Increment(const std::string &texture_group_key,const std::string &static_mesh_key) = 0;
	//
	liMeshStats(){}
	virtual ~liMeshStats(){}
};

class liModelCacher
{
public:
	//
	virtual void IncrementStaticMeshes(const liMaterialLibrary &material_library,const std::string &model_name,liMeshStats &mesh_stats) = 0;
	virtual void IncrementSkeletalMeshes(const liMaterialLibrary &material_library,const std::string &character_name,liMeshStats &mesh_stats) = 0;
	//
	liModelCacher()
	{}
	//
	virtual ~liModelCacher()
	{}
};

class liMeshCountOperation
{
public:
	virtual void PerformOperation(const std::string &mesh_name,unsigned int mesh_count) = 0;
	//
	liMeshCountOperation(){}
	virtual ~liMeshCountOperation(){}
};

class liTextureGroup
{
public:
	virtual const std::string &GetAlbedoMap() const = 0;
	virtual unsigned int NumStaticMeshes() const = 0;
	virtual unsigned int NumSkeletalMeshes() const = 0;
	virtual void ForEachStaticMeshCount(liMeshCountOperation &operation) const = 0;
	virtual void ForEachSkeletalMeshCount(liMeshCountOperation &operation) const = 0;
	//
	liTextureGroup()
	{}
	//
	virtual ~liTextureGroup()
	{}
};

class liTextureGroupOperation
{
public:
	virtual void PerformOperation(const liTextureGroup &texture_group) = 0;
	//
	liTextureGroupOperation(){}
	virtual ~liTextureGroupOperation(){}
};

class liSceneCacheStats
{
public:
	virtual unsigned int GetNumLights() const = 0;
	virtual unsigned int GetNumTextureGroups() const = 0;
	virtual void ForEachTextureGroup(liTextureGroupOperation &operation) const = 0;
	//
	liSceneCacheStats()
	{}
	//
	virtual ~liSceneCacheStats()
	{}
};

class lr3DSceneCacheStats : public li3DSceneCache, public liSceneCacheStats
{
private:
	//
	struct lr3DTextureGroup : public liTextureGroup
	{
	private:
		bool Initialized = false;
		std::string AlbedoMap;
		//
		std::map<std::string,unsigned int> StaticMeshCount;
		std::map<std::string,unsigned int> SkeletalMeshCount;
		//
	public:
		//
		bool IsInitialized()
		{
			return Initialized;
		}
		//
		void Initialize(const std::string &albedo_map)
		{
			if(!Initialized)
			{
				AlbedoMap = albedo_map;
				Initialized = true;
			}
		}
		//
		virtual const std::string &GetAlbedoMap() const override
		{
			return AlbedoMap;
		}
		//
		virtual unsigned int NumStaticMeshes() const override
		{
			return StaticMeshCount.size();
		}
		//
		virtual unsigned int NumSkeletalMeshes() const override
		{
			return SkeletalMeshCount.size();
		}
		//
		virtual void ForEachStaticMeshCount(liMeshCountOperation &operation) const override
		{
			for(auto &I : StaticMeshCount)
			{
				operation.PerformOperation(I.first,I.second);
			}
		}
		//
		virtual void ForEachSkeletalMeshCount(liMeshCountOperation &operation) const override
		{
			for(auto &I : SkeletalMeshCount)
			{
				operation.PerformOperation(I.first,I.second);
			}
		}
		//
		void IncrementStaticMeshCount(const std::string &mesh_key)
		{
			StaticMeshCount[mesh_key]++;
		}
		//
		void IncrementSkeletalMeshCount(const std::string &mesh_key)
		{
			SkeletalMeshCount[mesh_key]++;
		}
		//
		lr3DTextureGroup()
		{}
		//
		virtual ~lr3DTextureGroup() override
		{}
	};
	//
	std::map<std::string,lr3DTextureGroup> TextureGroups;
	unsigned int NumLights = 0;
	//
	lr3DTextureGroup &GetTextureGroup(const std::string &key)
	{
		lr3DTextureGroup &TextureGroup = TextureGroups[key];
		if(!TextureGroup.IsInitialized())
		{
			TextureGroup.Initialize(key);
		}
		return TextureGroup;
	}
	//
	class lrStaticMeshStats : public liMeshStats
	{
	private:
		lr3DSceneCacheStats &SceneCacheStats;
	public:
		//
		virtual void Increment(const std::string &texture_group_key,const std::string &static_mesh_key) override
		{
			SceneCacheStats.GetTextureGroup(texture_group_key).IncrementStaticMeshCount(static_mesh_key);
		}
		//
		lrStaticMeshStats(lr3DSceneCacheStats &scene_cache_stats)
			:SceneCacheStats(scene_cache_stats)
		{}
		//
		virtual ~lrStaticMeshStats()
		{}
	};
	//
	class lrSkeletalMeshStats : public liMeshStats
	{
	private:
		lr3DSceneCacheStats &SceneCacheStats;
	public:
		//
		virtual void Increment(const std::string &texture_group_key,const std::string &static_mesh_key) override
		{
			SceneCacheStats.GetTextureGroup(texture_group_key).IncrementSkeletalMeshCount(static_mesh_key);
		}
		//
		lrSkeletalMeshStats(lr3DSceneCacheStats &scene_cache_stats)
			:SceneCacheStats(scene_cache_stats)
		{}
		//
		virtual ~lrSkeletalMeshStats()
		{}
	};
	//
	liModelCacher &ModelCacher;
	//
public:
	//
	virtual void CacheStaticMesh(const li3DMesh &mesh,const li3DStaticMesh &static_mesh) override
	{
		lrStaticMeshStats StaticMeshStats(*this);
		//
		ModelCacher.IncrementStaticMeshes(mesh.GetMaterialLibrary(),static_mesh.GetModelName(),StaticMeshStats);
	}
	//
	virtual void CacheSkeletalMesh(const li3DMesh &mesh,const li3DSkeletalMesh &skeletal_mesh) override
	{
		lrSkeletalMeshStats SkeletalMeshStats(*this);
		//
		ModelCacher.IncrementSkeletalMeshes(mesh.GetMaterialLibrary(),skeletal_mesh.GetCharacterName(),SkeletalMeshStats);
	}
	//
	virtual void CacheLight(const li3DLight &light) override
	{
		NumLights++;
	}
	//
	virtual unsigned int GetNumLights() const override
	{
		return NumLights;
	}
	//
	virtual unsigned int GetNumTextureGroups() const override
	{
		return TextureGroups.size();
	}
	//
	virtual void ForEachTextureGroup(liTextureGroupOperation &operation) const override
	{
		for(auto &I : TextureGroups)
		{
			operation.PerformOperation(I.second);
		}
	}
	//
	lr3DSceneCacheStats(liModelCacher &model_cacher)
		:ModelCacher(model_cacher)
	{}
	//
	virtual ~lr3DSceneCacheStats() override
	{}
};

#endif // LR3D_SCENE_READER_H
