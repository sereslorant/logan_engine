#ifndef LFF_GL_3D_SCENE_CACHE_H
#define LFF_GL_3D_SCENE_CACHE_H

#include "../../../lRenderer/lr3DSceneReader.h"

#include "../lffGLData.h"

#include "../lffGLResourceLoader.h"
#include "../../../lRenderer/lrUtils.h"

#include <vector>

class lffGLModelCacher : public liModelCacher
{
private:
	lffGLResourceLoader &ResourceLoader;
	//
public:
	//
	virtual void IncrementMaterialGroups(const liMaterialLibrary &material_library,const std::string &model_name,liStaticMeshStats &static_mesh_stats) override
	{
		lffGLStaticMesh *StaticMesh = ResourceLoader.GetStaticMesh(model_name);
		for(unsigned int i=0;i < StaticMesh->GetNumMtlGroups();i++)
		{
			lffGLStaticMesh::lffGLMaterialGroup MtlGroup = StaticMesh->GetMtlGroup(i);
			//
			std::string TextureGroupKey;
			std::string MaterialGroupKey;
			lrUtils::ComputeTextureGroupKey(material_library.GetMaterial(MtlGroup.GetMaterial()),TextureGroupKey);
			lrUtils::ComputeMaterialGroupKey(model_name,MtlGroup.GetMaterial(),MaterialGroupKey);
			//
			static_mesh_stats.Increment(TextureGroupKey,MaterialGroupKey);
		}
	}
	//
	lffGLModelCacher(lffGLResourceLoader &resource_loader)
		:ResourceLoader(resource_loader)
	{}
	//
	virtual ~lffGLModelCacher() override
	{}
};

class lffGL3DMeshInstances
{
private:
	//
	bool MeshInitialized = false;
	lffGLStaticMesh::lffGLMaterialGroup MtlGroup;
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	unsigned int Next = 0;
	std::vector<lffGLMaterialData> Materials;
	std::vector<lmMatrix4x4> ModelMatrices;
	//
	/*
	 * Fixed function OpenGL specifikus dolog!
	 */
	//
	void ApplyMaterial(unsigned int material_id)
	{
		lffGLMaterialData &Material = Materials[material_id];
		//
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,Material.Diffuse);
		glMaterialfv(GL_FRONT,GL_SPECULAR,Material.Specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,&Material.Shininess);
	}
	//
public:
	//
	/*
	 * Fixed function OpenGL specifikus dolog!
	 */
	//
	void Construct(lffGLStaticMesh::lffGLMaterialGroup &mtl_group)
	{
		if(!MeshInitialized)
		{
			MtlGroup = mtl_group;
			MeshInitialized = true;
		}
	}
	//
	void DrawInstanced()
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			glPushMatrix();
				//
				glMultMatrixf(ModelMatrices[i][0]);
				ApplyMaterial(i);
				//
				MtlGroup.Draw();
			glPopMatrix();
		}
	}
	//
	/*
	 * Minden más!
	 */
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	//
	void Print()
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			std::cout << "\t\tMaterial:\n";
			std::cout << "\t\t" << Materials[i].Diffuse[0] << ";" << Materials[i].Diffuse[1] << ";" << Materials[i].Diffuse[2] << "\n";
			std::cout << "\t\t" << Materials[i].Specular[0] << ";" << Materials[i].Specular[1] << ";" << Materials[i].Specular[2] << "\n";
			std::cout << "\t\t" << Materials[i].Shininess << "\n";
			std::cout << "\t\tModel matrix:\n";
			for(int j=0;j < 4;j++)
			{
				std::cout << "\t\t";
				for(int k=0;k < 4;k++)
				{
					std::cout << ModelMatrices[i][j][k] << ";";
				}
				std::cout << "\n";
			}
			std::cout << "\n";
		}
	}
	//
	#endif
	//
	unsigned int NumInstances()
	{
		return (unsigned int)ModelMatrices.size();
	}
	//
	/*
	void AssignMtlGroup(lffGLStaticMesh &static_mesh,unsigned int index)
	{
		if(!MeshInitialized)
		{
			MtlGroup = static_mesh.GetMtlGroup(index);
			MeshInitialized = true;
		}
	}
	*/
	//
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		ModelMatrices.resize(size);
		Next = 0;
	}
	//
	void SetNextInstance(const liMaterial &material,const li3DMesh &mesh)
	{
		if(Next < NumInstances())
		{
			SetInstance(material,mesh,Next);
			Next++;
		}
	}
	//
	void SetInstance(const liMaterial &material,const li3DMesh &mesh,unsigned int instance_id)
	{
		if(instance_id < NumInstances())
		{
			Materials[instance_id].SetMaterial(material);
			ModelMatrices[instance_id] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			lrUtils::GetModelMatrix(mesh,ModelMatrices[instance_id]);
		}
	}
};

#include <vector>
#include <map>

class lffGL3DTextureList
{
private:
	std::string DiffuseMapName;
	lrGLTexture2DView DiffuseMap;
	//
	std::vector<lffGL3DMeshInstances> MeshInstances;
	std::map<std::string,lffGL3DMeshInstances *> ModelDictionary;
	//
public:
	//
	/*
	 * FIXED FUNCTION OPENGL SPECIFIKUS DOLOG!!!
	 */
	//
	void DrawTextureList()
	{
		for(lffGL3DMeshInstances &MeshInstances_Puszcsy : MeshInstances)
		{
			MeshInstances_Puszcsy.DrawInstanced();
		}
	}
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	//
	void Print()
	{
		for(auto &J : ModelDictionary)
		{
			std::cout << "\tMesh: " << J.first << "\n";
			lffGL3DMeshInstances &MeshInstances = *J.second;
			MeshInstances.Print();
		}
	}
	//
	#endif
	//
	const std::string &GetDiffuseMapName()
	{
		return DiffuseMapName;
	}
	//
	lrGLTexture2DView &GetDiffuseMap()
	{
		return DiffuseMap;
	}
	//
	void Construct(const std::string &diffuse_map_name,lrGLTexture2DView diffuse_map,unsigned int num_material_groups)
	{
		DiffuseMapName = diffuse_map_name;
		DiffuseMap = diffuse_map;
		MeshInstances.resize(num_material_groups);
	}
	//
	lffGL3DMeshInstances &GetMeshInstances(const std::string &material_group_key)
	{
		return *ModelDictionary[material_group_key];
	}
	//
	void InitializeMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lffGL3DMeshInstances &MatGroupInstances = MeshInstances[mat_group_id];
		//
		MatGroupInstances.Resize(num_instances);
		ModelDictionary[material_group_name] = &MatGroupInstances;
	}
};

#include <vector>

struct lffGLLights
{
private:
	unsigned int NextLight = 0;
	std::vector<lffGLLightData> Lights;
	//
public:
	//
	unsigned int NumLights()
	{
		return Lights.size();
	}
	//
	void Resize(unsigned int new_size)
	{
		Lights.resize(new_size);
	}
	//
	void SetLight(const li3DLight &light,unsigned int light_id)
	{
		if(light_id < Lights.size())
		{
			Lights[light_id].SetLight(light);
		}
	}
	//
	void SetNextLight(const li3DLight &light)
	{
		if(NextLight < Lights.size())
		{
			SetLight(light,NextLight);
			NextLight++;
		}
	}
	//
	void ApplyLight(unsigned int light_id)
	{
		if(light_id < Lights.size())
		{
			lffGLLightData &Light = Lights[light_id];
			//
			glLightfv(GL_LIGHT0,GL_POSITION,Light.Position);
			//
			glLightfv(GL_LIGHT0,GL_AMBIENT,Light.Ambient);
			glLightfv(GL_LIGHT0,GL_DIFFUSE,Light.Diffuse);
			glLightfv(GL_LIGHT0,GL_SPECULAR,Light.Specular);
		}
	}
};

#include <vector>
#include <map>

class lffGL3DSceneCache
{
private:
	//
	class lAllocateMeshInstances : public liMeshCountOperation
	{
	private:
		lffGL3DTextureList &TextureList;
		unsigned int CurrentMeshId=0;
		//
	public:
		//
		virtual void PerformOperation(const std::string &mesh_name,unsigned int mesh_count) override
		{
			TextureList.InitializeMeshInstances(CurrentMeshId,mesh_name,mesh_count);
			//
			CurrentMeshId++;
		}
		//
		lAllocateMeshInstances(lffGL3DTextureList &texture_list)
			:TextureList(texture_list)
		{}
		//
		virtual ~lAllocateMeshInstances() override
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
		lffGLResourceLoader &ResourceLoader;
		lffGL3DSceneCache &SceneCache;
		unsigned int TextureListId = 0;
		//
	public:
		virtual void PerformOperation(const liTextureGroup &texture_group) override
		{
			lffGL3DTextureList &TextureList = SceneCache.TextureLists[TextureListId];
			//
			TextureList.Construct(	texture_group.GetAlbedoMap(),
									ResourceLoader.GetTexture(texture_group.GetAlbedoMap()),
									texture_group.NumStaticMeshes());
			//
			lAllocateMeshInstances AllocateMeshInstances(TextureList);
			texture_group.ForEachMeshCount(AllocateMeshInstances);
			//
			SceneCache.TextureDictionary[texture_group.GetAlbedoMap()] = &TextureList;
			TextureListId++;
		}
		//
		lAllocateTextureList(lffGLResourceLoader &resource_loader,lffGL3DSceneCache &scene_cache)
			:ResourceLoader(resource_loader),SceneCache(scene_cache)
		{}
		//
		virtual ~lAllocateTextureList() override
		{}
	};
	//
	class lffGL3DSceneCacher : public li3DSceneCache
	{
	private:
		lffGLResourceLoader &ResourceLoader;
		lffGL3DSceneCache &SceneCache;
		//
	public:
		//
		virtual void CacheStaticMesh(const li3DMesh &mesh,const li3DStaticMesh &static_mesh) override
		{
			lffGLStaticMesh *StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
			const liMaterialLibrary &MaterialLibrary = mesh.GetMaterialLibrary();
			//
			for(unsigned int i=0;i < StaticMesh->GetNumMtlGroups();i++)
			{
				lffGLStaticMesh::lffGLMaterialGroup MtlGroup = StaticMesh->GetMtlGroup(i);
				//
				const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
				//
				std::string TextureGroupName;
				std::string MaterialGroupName;
				lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
				lrUtils::ComputeMaterialGroupKey(static_mesh.GetModelName(),MtlGroup.GetMaterial(),MaterialGroupName);
				//
				lffGL3DMeshInstances &MeshInstances = SceneCache.GetMeshInstances(TextureGroupName,MaterialGroupName);
				//
				MeshInstances.Construct(MtlGroup);
				MeshInstances.SetNextInstance(Material,mesh);
			}
		}
		//
		virtual void CacheLight(const li3DLight &light) override
		{
			SceneCache.Lights.SetNextLight(light);
		}
		//
		lffGL3DSceneCacher(lffGLResourceLoader &resource_loader,lffGL3DSceneCache &scene_cache)
			:ResourceLoader(resource_loader),SceneCache(scene_cache)
		{
			//
		}
		//
		virtual ~lffGL3DSceneCacher() override
		{
			//
		}
		//
		/*
		 * End of class
		 */
	};
	//
	lffGL3DSceneCacher SceneCacher;
	//
	lmMatrix4x4 ProjectionMatrix;
	lmMatrix4x4 ViewMatrix;
	//
	lffGLLights Lights;
	//
	std::map<std::string,lffGL3DTextureList *> TextureDictionary;
	std::vector<lffGL3DTextureList> TextureLists;
	//
	lffGL3DTextureList &GetTextureList(const std::string &texture_group_name)
	{
		return *TextureDictionary[texture_group_name];
	}
	//
	lffGL3DMeshInstances &GetMeshInstances(const std::string &texture_group_name,const std::string &material_group_name)
	{
		return GetTextureList(texture_group_name).GetMeshInstances(material_group_name);
	}
	//
	/*
	 * Minden, ami a rendereléshez köthető
	 */
	void Prepare()
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
		glEnable(GL_TEXTURE_2D);
		//
		glEnable(GL_DEPTH_TEST);
	}
	//
	void SetProjectionMatrix()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultMatrixf(ProjectionMatrix[0]);
	}
	//
	void SetViewMatrix()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultMatrixf(ViewMatrix[0]);
	}
	//
	void DrawMeshes()
	{
		for(lffGL3DTextureList &TextureList : TextureLists)
		{
			lrGLTexture2DView Texture = TextureList.GetDiffuseMap();
			Texture.Bind();
			//
			TextureList.DrawTextureList();
			/*
			for(lffGL3DMeshInstances &MeshInstances : TextureList.MeshInstances)
			{
				MeshInstances.DrawInstanced();
			}
			*/
			Texture.Unbind();
		}
	}
	//
public:
	//
	li3DSceneCache &GetSceneCacher()
	{
		return SceneCacher;
	}
	//
	#ifdef L_DEBUG_PRINT_SCENE_CACHE
	//
	void Print()
	{
		std::cout << "StaticMeshes:" << std::endl;
		for(auto &I : TextureDictionary)
		{
			std::cout << "Texture: " << I.first << "\n";
			lffGL3DTextureList &TextureList = *I.second;
			TextureList.Print();
		}
		std::cout << std::endl;
	}
	//
	#endif
	//
	void DrawScene()
	{
		Prepare();
		//
		SetProjectionMatrix();
		//
		SetViewMatrix();
		//
		/*
		 * Depth test beállítása
		 */
		bool First = true;
		//
		glDepthFunc(GL_LESS);
		//
		for(unsigned int i=0;i < Lights.NumLights();i++)
		{
			Lights.ApplyLight(i);
			//
			DrawMeshes();
			//
			if(First)
			{
				glDepthFunc(GL_LEQUAL);
				First = false;
			}
		}
	}
	//
	lffGL3DSceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,lffGLResourceLoader &resource_loader)
		:SceneCacher(resource_loader,*this),ProjectionMatrix(lmMatrix4x4::IDENTITY),ViewMatrix(lmMatrix4x4::IDENTITY)
	{
		lrUtils::GetProjectionMatrix(frustum,ProjectionMatrix);
		lrUtils::GetViewMatrix(camera,ViewMatrix);
		//
		Lights.Resize(scene_cache_stats.GetNumLights());
		//
		TextureLists.resize(scene_cache_stats.GetNumTextureGroups());
		//
		lAllocateTextureList AllocateTextureList(resource_loader,*this);
		scene_cache_stats.ForEachTextureGroup(AllocateTextureList);
	}
};

#endif // LFF_GL_3D_SCENE_CACHE_H
