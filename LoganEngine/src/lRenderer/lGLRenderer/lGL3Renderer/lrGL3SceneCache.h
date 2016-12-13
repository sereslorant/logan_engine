#ifndef LR_GL3_SCENE_CACHE_H
#define LR_GL3_SCENE_CACHE_H

#include "../lGLResources/lrGLResourceLoader.h"

#include "../../lrUtils.h"

class lrGLModelCacher : public liModelCacher
{
private:
	lrGLResourceLoader &ResourceLoader;
	//
public:
	//
	virtual void IncrementMaterialGroups(const liMaterialLibrary &material_library,const std::string &model_name,liStaticMeshStats &static_mesh_stats) override
	{
		lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(model_name);
		for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
		{
			lrGLMaterialGroupView MaterialGroup = StaticMesh.GetMaterialGroup(i);
			//
			std::string TextureGroupKey;
			std::string MaterialGroupKey;
			lrUtils::ComputeTextureGroupKey(material_library.GetMaterial(MaterialGroup.GetMaterial()),TextureGroupKey);
			lrUtils::ComputeMaterialGroupKey(model_name,MaterialGroup.GetMaterial(),MaterialGroupKey);
			//
			static_mesh_stats.Increment(TextureGroupKey,MaterialGroupKey);
		}
	}
	//
	lrGLModelCacher(lrGLResourceLoader &resource_loader)
		:ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGLModelCacher() override
	{}
};

#include "../lGLIncludes.h"

struct lrGLLightData
{
	GLfloat Position[4] = {0.0,0.0,0.0,1.0};
	//GLfloat Ambient[4] = {0.0,0.0,0.0,1.0};
	GLfloat Color[4] = {0.0,0.0,0.0,1.0};
	GLfloat Intensity = 0.0;
	//
	void SetLight(const li3DLight &light)
	{
		for(int i=0;i < 3;i++)
		{
			Position[i] = light.GetPosition()[i];
		}
		//
		Color[0] = light.GetColor().GetRed();
		Color[1] = light.GetColor().GetGreen();
		Color[2] = light.GetColor().GetBlue();
		//
		Intensity = light.GetIntensity();
	}
};

struct lrGLMaterialData
{
	/*
	 * Material[0]: albedo
	 * Material[1]: Fresnel,roughness,reflectiveness,metallic
	 */
	GLfloat Material[2][4] = {{0.0,0.0,0.0,1.0},{0.0,0.0,0.0,0.0}};
	//
	void SetMaterial(const liMaterial &material)
	{
		Material[0][0] = material.GetAlbedo().GetRed();
		Material[0][1] = material.GetAlbedo().GetGreen();
		Material[0][2] = material.GetAlbedo().GetBlue();
		//
		Material[1][0] = material.GetFresnel();
		Material[1][1] = material.GetRoughness();
		Material[1][2] = material.GetReflectiveness();
		Material[1][3] = material.GetMetallic();
	}
};

#include "../liGLShaderInterfaces.h"

#include "../lGLIncludes.h"
#include "../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class lrGL3MeshInstances
{
private:
	//
	bool MeshInitialized = false;
	lrGLMaterialGroupView MaterialGroup;
	GLuint VertexArrayObject = 0;
	/*
	 * Instance-ökhöz tartozó adatok.
	 */
	unsigned int Next = 0;
	std::vector<lrGLMaterialData> Materials;
	std::vector<lmMatrix4x4> ModelMatrices;
	std::vector<lmMatrix4x4> MvMatrices;
	std::vector<lmMatrix4x4> MvpMatrices;
	std::vector<lmMatrix3x3> NormalMatrices;
	//
	void ApplyMatrix(liGLShader &shader,unsigned int instance_id)
	{
		glUniformMatrix4fv(shader.GetModelMatrixLocation(),1,GL_FALSE,ModelMatrices[instance_id][0]);
		glUniformMatrix3fv(shader.GetNormalMatrixLocation(),1,GL_FALSE,NormalMatrices[instance_id][0]);
	}
	//
	void ApplyMaterial(liGLShader &shader,unsigned int instance_id)
	{
		lrGLMaterialData &Material = Materials[instance_id];
		//
		glUniform3fv(shader.GetMaterial0Location(),1,Material.Material[0]);
		glUniform4fv(shader.GetMaterial1Location(),1,Material.Material[1]);
	}
	//
public:
	//
	void Construct(liGLStaticMeshShader &static_mesh_shader,lrGLMaterialGroupView &material_group)
	{
		if(!MeshInitialized)
		{
			glGenVertexArrays(1,&VertexArrayObject);
			glBindVertexArray(VertexArrayObject);
			//
			MaterialGroup = material_group;
			MaterialGroup.BindAttributes(static_mesh_shader);
			//
			glBindVertexArray(0);
			//
			MeshInitialized = true;
		}
	}
	//
	void DrawInstances(liGLShader &shader)
	{
		for(unsigned int i=0;i < NumInstances();i++)
		{
			ApplyMatrix(shader,i);
			ApplyMaterial(shader,i);
			//
			glBindVertexArray(VertexArrayObject);
			MaterialGroup.Draw();
			glBindVertexArray(0);
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
			std::cout << "\t\t" << Materials[i].Material[0][0] << ";" << Materials[i].Material[0][1] << ";" << Materials[i].Material[0][2] << "\n";
			std::cout << "\t\t" << Materials[i].Material[1][0] << ";" << Materials[i].Material[1][1] << ";" << Materials[i].Material[1][2] << ";" << Materials[i].Material[1][3] << "\n";
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
	void Resize(unsigned int size)
	{
		Materials.resize(size);
		ModelMatrices.resize(size);
		MvMatrices.resize(size);
		MvpMatrices.resize(size);
		NormalMatrices.resize(size);
		Next = 0;
	}
	//
	void SetNextInstance(lmMatrix4x4 &projection_matrix,lmMatrix4x4 &view_matrix,const liMaterial &material,const li3DMesh &mesh)
	{
		if(Next < NumInstances())
		{
			SetInstance(projection_matrix,view_matrix,material,mesh,Next);
			Next++;
		}
	}
	//
	void SetInstance(lmMatrix4x4 &projection_matrix,lmMatrix4x4 &view_matrix,const liMaterial &material,const li3DMesh &mesh,unsigned int instance_id)
	{
		if(instance_id < NumInstances())
		{
			Materials[instance_id].SetMaterial(material);
			ModelMatrices[instance_id] = lmMatrix4x4(lmMatrix4x4::IDENTITY);
			lrUtils::GetModelMatrix(mesh,ModelMatrices[instance_id]);
			//
			MvMatrices[instance_id] = view_matrix * ModelMatrices[instance_id];
			MvpMatrices[instance_id] = projection_matrix * MvMatrices[instance_id];
			ModelMatrices[instance_id].GetSubMatrix(3,3).Invert().Transpose(NormalMatrices[instance_id]);
		}
	}
	//
	lrGL3MeshInstances()
	{}
	//
	~lrGL3MeshInstances()
	{
		glDeleteVertexArrays(1,&VertexArrayObject);
	}
};


#include <vector>
#include <map>

class lrGL3TextureList
{
private:
	std::string AlbedoMapName;
	lrGLTexture2DView AlbedoMap;
	//
	std::vector<lrGL3MeshInstances> MeshInstances;
	std::map<std::string,lrGL3MeshInstances *> ModelDictionary;
	//
public:
	//
	void DrawTextureList(liGLShader &shader)
	{
		for(lrGL3MeshInstances &MeshInstances_Puszcsy : MeshInstances)
		{
			MeshInstances_Puszcsy.DrawInstances(shader);
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
			lrGL3MeshInstances &MeshInstances = *J.second;
			MeshInstances.Print();
		}
	}
	//
	#endif
	//
	const std::string &GetAlbedoMapName()
	{
		return AlbedoMapName;
	}
	//
	lrGLTexture2DView &GetAlbedoMap()
	{
		return AlbedoMap;
	}
	//
	void Construct(const std::string &albedo_map_name,lrGLTexture2DView albedo_map,unsigned int num_material_groups)
	{
		AlbedoMapName = albedo_map_name;
		AlbedoMap = albedo_map;
		MeshInstances.resize(num_material_groups);
	}
	//
	lrGL3MeshInstances &GetMeshInstances(const std::string &material_group_key)
	{
		return *ModelDictionary[material_group_key];
	}
	//
	void InitializeMeshInstances(unsigned int mat_group_id,const std::string &material_group_name,unsigned int num_instances)
	{
		lrGL3MeshInstances &MatGroupInstances = MeshInstances[mat_group_id];
		//
		MatGroupInstances.Resize(num_instances);
		ModelDictionary[material_group_name] = &MatGroupInstances;
	}
};

#include <vector>

struct lrGLLights
{
private:
	unsigned int NextLight = 0;
	std::vector<lrGLLightData> Lights;
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
	void ApplyLight(liGLShader &shader,unsigned int light_id)
	{
		if(light_id < Lights.size())
		{
			lrGLLightData &Light = Lights[light_id];
			//
			//glUniform3fv(shader.GetLightPositionLocation(),1,Light.Position);
			//glUniform3fv(shader.GetLightColorLocation(),1,Light.Color);
			//glUniform1f(shader.GetLightIntensityLocation(),Light.Intensity);
			//
			ApplyLight(shader,Light);
		}
	}
	//
	static void ApplyLight(liGLShader &shader,lrGLLightData &light)
	{
		glUniform3fv(shader.GetLightPositionLocation(),1,light.Position);
		glUniform3fv(shader.GetLightColorLocation(),1,light.Color);
		glUniform1f(shader.GetLightIntensityLocation(),light.Intensity);
	}
};

#include "lrGL3StaticMeshShader.h"

#include <vector>
#include <map>

class lrGL3SceneCache
{
private:
	//
	class lAllocateMeshInstances : public liMeshCountOperation
	{
	private:
		lrGL3TextureList &TextureList;
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
		lAllocateMeshInstances(lrGL3TextureList &texture_list)
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
		lrGLResourceLoader &ResourceLoader;
		lrGL3SceneCache &SceneCache;
		unsigned int TextureListId = 0;
		//
	public:
		virtual void PerformOperation(const liTextureGroup &texture_group) override
		{
			lrGL3TextureList &TextureList = SceneCache.TextureLists[TextureListId];
			//
			TextureList.Construct(	texture_group.GetDiffuseMap(),
									ResourceLoader.GetTexture(texture_group.GetDiffuseMap()),
									texture_group.NumStaticMeshes());
			//
			lAllocateMeshInstances AllocateMeshInstances(TextureList);
			texture_group.ForEachMeshCount(AllocateMeshInstances);
			//
			SceneCache.TextureDictionary[texture_group.GetDiffuseMap()] = &TextureList;
			TextureListId++;
		}
		//
		lAllocateTextureList(lrGLResourceLoader &resource_loader,lrGL3SceneCache &scene_cache)
			:ResourceLoader(resource_loader),SceneCache(scene_cache)
		{}
		//
		virtual ~lAllocateTextureList() override
		{}
	};
	//
	class lrGL3SceneCacher : public li3DSceneCache
	{
	private:
		lrGLResourceLoader &ResourceLoader;
		lrGL3SceneCache &SceneCache;
		//
	public:
		//
		virtual void CacheStaticMesh(const li3DMesh &mesh,const li3DStaticMesh &static_mesh) override
		{
			lrGLStaticMeshView StaticMesh = ResourceLoader.GetStaticMesh(static_mesh.GetModelName());
			const liMaterialLibrary &MaterialLibrary = mesh.GetMaterialLibrary();
			//
			for(unsigned int i=0;i < StaticMesh.NumMtlGroups();i++)
			{
				lrGLMaterialGroupView MtlGroup = StaticMesh.GetMaterialGroup(i);
				//
				const liMaterial &Material = MaterialLibrary.GetMaterial(MtlGroup.GetMaterial());
				//
				std::string TextureGroupName;
				std::string MaterialGroupName;
				lrUtils::ComputeTextureGroupKey(Material,TextureGroupName);
				lrUtils::ComputeMaterialGroupKey(static_mesh.GetModelName(),MtlGroup.GetMaterial(),MaterialGroupName);
				//
				lrGL3MeshInstances &MeshInstances = SceneCache.GetMeshInstances(TextureGroupName,MaterialGroupName);
				//
				MeshInstances.Construct(SceneCache.StaticMeshShader,MtlGroup);
				MeshInstances.SetNextInstance(SceneCache.ProjectionMatrix,SceneCache.ViewMatrix,Material,mesh);
			}
		}
		//
		virtual void CacheLight(const li3DLight &light) override
		{
			SceneCache.Lights.SetNextLight(light);
		}
		//
		lrGL3SceneCacher(lrGLResourceLoader &resource_loader,lrGL3SceneCache &scene_cache)
			:ResourceLoader(resource_loader),SceneCache(scene_cache)
		{
			//
		}
		//
		virtual ~lrGL3SceneCacher() override
		{
			//
		}
		//
		/*
		 * End of class
		 */
	};
	//
	lrGL3SceneCacher SceneCacher;
	//
	lmMatrix4x4 ProjectionMatrix;
	lmMatrix4x4 ViewMatrix;
	//
	lmVector3D CameraPosition;
	//
	lrGLLights Lights;
	//
	lrGL3StaticMeshShader &StaticMeshShader;
	//
	std::map<std::string,lrGL3TextureList *> TextureDictionary;
	std::vector<lrGL3TextureList> TextureLists;
	//
	lrGL3TextureList &GetTextureList(const std::string &texture_group_name)
	{
		return *TextureDictionary[texture_group_name];
	}
	//
	lrGL3MeshInstances &GetMeshInstances(const std::string &texture_group_name,const std::string &material_group_name)
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
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
	}
	//
	void SetProjectionMatrix()
	{
		glUniformMatrix4fv(StaticMeshShader.GetProjectionMatrixLocation(),1,GL_FALSE,ProjectionMatrix[0]);
	}
	//
	void SetViewMatrix()
	{
		glUniformMatrix4fv(StaticMeshShader.GetViewMatrixLocation(),1,GL_FALSE,ViewMatrix[0]);
		glUniform3fv(StaticMeshShader.GetCameraPositionLocation(),1,&CameraPosition[0]);
	}
	//
	void DrawMeshes()
	{
		for(lrGL3TextureList &TextureList : TextureLists)
		{
			//lrGLTexture2DView AlbedoMap = TextureList.GetAlbedoMap();
			//TODO
			//AlbedoMap.Bind();
			//
			TextureList.DrawTextureList(StaticMeshShader);
			//
			//TODO
			//AlbedoMap.Unbind();
		}
	}
	//
	void PrePass()
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//
		glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
		//
		DrawMeshes();
	}
	//
	void Draw()
	{
		glDepthFunc(GL_LEQUAL);
		//
		glDepthMask(GL_FALSE);
		glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
		//
		glDisable(GL_BLEND);
		bool First = true;
		//
		for(unsigned int i=0;i < Lights.NumLights();i++)
		{
			Lights.ApplyLight(StaticMeshShader,i);
			//
			DrawMeshes();
			//
			if(First)
			{
				First = false;
				//
				glEnable(GL_BLEND);
				glBlendEquation(GL_FUNC_ADD);
				glBlendFunc(GL_ONE,GL_ONE);
			}
		}
		//
		/*
		 * Ez kell, hogy a clear hatással legyen a depth bufferre.
		 */
		glDepthMask(GL_TRUE);
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
			lrGL3TextureList &TextureList = *I.second;
			TextureList.Print();
		}
		std::cout << std::endl;
	}
	//
	#endif
	//
	void DrawScene()
	{
		StaticMeshShader.UseProgram();
		//
		Prepare();
		//
		SetProjectionMatrix();
		//
		SetViewMatrix();
		//
		/*
		 * Pre pass rendering
		 */
		PrePass();
		//
		/*
		 * Lighting
		 */
		Draw();
		//
		StaticMeshShader.DisableProgram();
	}
	//
	lrGL3SceneCache(const liFrustum &frustum,const li3DCamera &camera,const liSceneCacheStats &scene_cache_stats,lrGL3StaticMeshShader &static_mesh_shader,lrGLResourceLoader &resource_loader)
		:SceneCacher(resource_loader,*this),ProjectionMatrix(lmMatrix4x4::IDENTITY),ViewMatrix(lmMatrix4x4::IDENTITY),StaticMeshShader(static_mesh_shader)
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
		lAllocateTextureList AllocateTextureList(resource_loader,*this);
		scene_cache_stats.ForEachTextureGroup(AllocateTextureList);
	}
};

#endif // LR_GL3_SCENE_CACHE_H
