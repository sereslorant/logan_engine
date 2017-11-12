#ifndef LR_GL_RESOURCE_LOADER_H
#define LR_GL_RESOURCE_LOADER_H

#include "lrGLMeshResource/lrGLVertexData.h"
#include "lrGLMeshResource/lrGLBoneData.h"
#include "lrGLMeshResource/lrGLPrimitiveGroup.h"

#include <vector>

#include <lResourceManager/liResourceManager.h>

struct lrGLStaticMeshResource
{
	lrGLVertexData VertexData;
	std::vector<lrGLPrimitiveGroup> MaterialGroups;
	
	void Initialize(lrmStaticMesh &static_mesh)
	{
		VertexData.UploadData(static_mesh);
		
		MaterialGroups.resize(static_mesh.GetNumMtlGroups());
		for(unsigned int i=0;i < MaterialGroups.size();i++)
		{
			//MaterialGroups[i].Initialize(static_mesh.GetMaterialGroup(i).IndexBuffer.size());
			MaterialGroups[i].UploadData(static_mesh.GetMaterialGroup(i));
		}
	}
	
	lrGLStaticMeshResource()
	{
		// Empty
	}
	
	~lrGLStaticMeshResource()
	{
		// Empty
	}
	/*
	 * End of class
	 */
};

struct lrGLSkeletalMeshResource
{
	lrGLVertexData	VertexData;
	lrGLBoneData	BoneData;
	std::vector<lrGLPrimitiveGroup> MaterialGroups;
	
	void Initialize(lrmSkeletalMesh &skeletal_mesh)
	{
		VertexData.UploadData(skeletal_mesh.BindPoseMesh);
		BoneData.UploadData(skeletal_mesh);
		
		MaterialGroups.resize(skeletal_mesh.BindPoseMesh.GetNumMtlGroups());
		for(unsigned int i=0;i < MaterialGroups.size();i++)
		{
			MaterialGroups[i].UploadData(skeletal_mesh.BindPoseMesh.GetMaterialGroup(i));
		}
	}
	
	lrGLSkeletalMeshResource()
	{
		// Empty
	}
	
	~lrGLSkeletalMeshResource()
	{
		// Empty
	}
	/*
	 * End of class
	 */
};

#include "../liGLShaderInterfaces.h"

class lrGLStaticMaterialGroupView
{
private:
	lrGLVertexData		*VertexData = nullptr;
	lrGLPrimitiveGroup	*MaterialGroup = nullptr;
	
public:
	
	const std::string &GetMaterial()
	{
		return MaterialGroup->MaterialName;
	}
	
	void BindAttributes(liGLStaticMeshShader &static_mesh_shader)
	{
		if(MaterialGroup == nullptr)
			{return;}
		
		if(VertexData != nullptr)
		{
			GLint PositionLocation  = static_mesh_shader.GetVertexLocation();
			GLint NormalLocation    = static_mesh_shader.GetNormalLocation();
			GLint TangentLocation   = static_mesh_shader.GetTangentLocation();
			GLint BitangentLocation = static_mesh_shader.GetBitangentLocation();
			GLint TexCoordLocation  = static_mesh_shader.GetTexCoordLocation();
			
			VertexData->VertexBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(PositionLocation);
			glVertexAttribPointer(PositionLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->NormalBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(NormalLocation);
			glVertexAttribPointer(NormalLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->TangentBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(TangentLocation);
			glVertexAttribPointer(TangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->BitangentBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(BitangentLocation);
			glVertexAttribPointer(BitangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->TexCoordBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(TexCoordLocation);
			glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);
		}
	}
	
	void BindIndexBuffer()
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			MaterialGroup->IndexBuffer.BindBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
	}
	
	void Draw()
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			glDrawElements(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr);
		}
	}
	
	/*
	void DrawInstanced(unsigned int num_instances)
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,MaterialGroup->IndexBuffer);
			glDrawElementsInstanced(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr,num_instances);
		}
	}
	*/
	
	lrGLStaticMaterialGroupView()
	{
		//Empty
	}
	
	lrGLStaticMaterialGroupView(lrGLVertexData *vertex_data,lrGLPrimitiveGroup *material_group)
		:VertexData(vertex_data),MaterialGroup(material_group)
	{
		//Empty
	}
	
	~lrGLStaticMaterialGroupView()
	{
		//Empty
	}
	/*
	 * End of class
	 */
};

class lrGLStaticMeshView
{
private:
	lrGLStaticMeshResource *StaticMeshResource;
	
public:
	
	unsigned int NumMtlGroups()
	{
		if(StaticMeshResource != nullptr)
		{
			return StaticMeshResource->MaterialGroups.size();
		}
		else
		{
			return 0;
		}
	}
	
	lrGLStaticMaterialGroupView GetMaterialGroup(unsigned int id)
	{
		if(StaticMeshResource != nullptr && id < StaticMeshResource->MaterialGroups.size())
		{
			return lrGLStaticMaterialGroupView(&StaticMeshResource->VertexData,&StaticMeshResource->MaterialGroups[id]);
		}
		else
		{
			return lrGLStaticMaterialGroupView();
		}
	}
	
	lrGLStaticMeshView(lrGLStaticMeshResource *static_mesh_resource)
		:StaticMeshResource(static_mesh_resource)
	{}
	
	~lrGLStaticMeshView()
	{}
	/*
	 * End of class
	 */
};

class lrGLSkeletalMaterialGroupView
{
private:
	lrGLVertexData		*VertexData		= nullptr;
	lrGLBoneData		*BoneData		= nullptr;
	lrGLPrimitiveGroup	*MaterialGroup	= nullptr;
	
public:
	
	const std::string &GetMaterial()
	{
		return MaterialGroup->MaterialName;
	}
	
	void BindAttributes(liGLStaticMeshShader &static_mesh_shader)
	{
		if(MaterialGroup == nullptr)
			{return;}
		
		if(VertexData != nullptr)
		{
			GLint PositionLocation  = static_mesh_shader.GetVertexLocation();
			GLint NormalLocation    = static_mesh_shader.GetNormalLocation();
			GLint TangentLocation   = static_mesh_shader.GetTangentLocation();
			GLint BitangentLocation = static_mesh_shader.GetBitangentLocation();
			GLint TexCoordLocation  = static_mesh_shader.GetTexCoordLocation();
			
			VertexData->VertexBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(PositionLocation);
			glVertexAttribPointer(PositionLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->NormalBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(NormalLocation);
			glVertexAttribPointer(NormalLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->TangentBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(TangentLocation);
			glVertexAttribPointer(TangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->BitangentBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(BitangentLocation);
			glVertexAttribPointer(BitangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
			
			VertexData->TexCoordBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(TexCoordLocation);
			glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);
		}
	}
	
	void BindBoneData(liGLSkeletalMeshShader &skeletal_mesh_shader)
	{
		if(MaterialGroup == nullptr)
			{return;}
		
		if(BoneData != nullptr)
		{
			GLint BoneIdLocation	= skeletal_mesh_shader.GetBoneIdLocation();
			GLint WeightLocation	= skeletal_mesh_shader.GetWeightLocation();
			
			//glBindBuffer(GL_ARRAY_BUFFER,BoneData->BoneIdBuffer);
			BoneData->BoneIdBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(BoneIdLocation);
			glVertexAttribIPointer(BoneIdLocation,lrmSkeletalMesh::MAX_WEIGHT_COUNT,GL_INT,0,0);
			
			//glBindBuffer(GL_ARRAY_BUFFER,BoneData->WeightBuffer);
			BoneData->WeightBuffer.BindBuffer(GL_ARRAY_BUFFER);
			glEnableVertexAttribArray(WeightLocation);
			glVertexAttribPointer(WeightLocation,lrmSkeletalMesh::MAX_WEIGHT_COUNT,GL_FLOAT,GL_FALSE,0,0);
		}
	}
	
	void BindIndexBuffer()
	{
		if(VertexData != nullptr && BoneData != nullptr && MaterialGroup != nullptr)
		{
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,MaterialGroup->IndexBuffer);
			MaterialGroup->IndexBuffer.BindBuffer(GL_ELEMENT_ARRAY_BUFFER);
		}
	}
	
	void Draw()
	{
		if(VertexData != nullptr && BoneData != nullptr && MaterialGroup != nullptr)
		{
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,MaterialGroup->IndexBuffer);
			glDrawElements(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr);
			//glDrawElements(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_SHORT,nullptr);
		}
	}
	
	/*
	void DrawInstanced(unsigned int num_instances)
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,MaterialGroup->IndexBuffer);
			glDrawElementsInstanced(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr,num_instances);
		}
	}
	*/
	
	lrGLSkeletalMaterialGroupView()
	{
		//Empty
	}
	
	lrGLSkeletalMaterialGroupView(lrGLVertexData *vertex_data,lrGLBoneData *bone_data,lrGLPrimitiveGroup *material_group)
		:VertexData(vertex_data),BoneData(bone_data),MaterialGroup(material_group)
	{
		//Empty
	}
	
	~lrGLSkeletalMaterialGroupView()
	{
		//Empty
	}
	/*
	 * End of class
	 */
};

class lrGLSkeletalMeshView
{
private:
	lrGLSkeletalMeshResource *SkeletalMeshResource;
	
public:
	
	unsigned int NumMtlGroups()
	{
		if(SkeletalMeshResource != nullptr)
		{
			return SkeletalMeshResource->MaterialGroups.size();
		}
		else
		{
			return 0;
		}
	}
	
	lrGLSkeletalMaterialGroupView GetMaterialGroup(unsigned int id)
	{
		if(SkeletalMeshResource != nullptr && id < SkeletalMeshResource->MaterialGroups.size())
		{
			return lrGLSkeletalMaterialGroupView(&SkeletalMeshResource->VertexData,&SkeletalMeshResource->BoneData,&SkeletalMeshResource->MaterialGroups[id]);
		}
		else
		{
			return lrGLSkeletalMaterialGroupView();
		}
	}
	
	lrGLSkeletalMeshView(lrGLSkeletalMeshResource *skeletal_mesh_resource)
		:SkeletalMeshResource(skeletal_mesh_resource)
	{}
	
	~lrGLSkeletalMeshView()
	{}
	/*
	 * End of class
	 */
};

#include <lResourceManager/liResourceManager.h>

#include "lGLTextureResource/lrGLTextureResource.h"
#include "lGLTextureResource/lrGLTexture2DView.h"
#include "lGLTextureResource/lrGLTextureCubemapView.h"

#include <map>

class lrGLResourceLoader
{
private:
	liResourceManager *ResourceManager = nullptr;
	
	std::map<std::string,lrGLStaticMeshResource> StaticMeshes;
	std::map<std::string,lrGLStaticMeshResource> SkeletalMeshes;
	std::map<std::string,lrGLTextureResource> Textures;
	std::map<std::string,lrGLTextureResource> Cubemaps;
	
	lrmStaticMesh *LoadStaticMesh(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			return ResourceManager->GetStaticMesh(resource_id);
		}
		else
		{
			return nullptr;
		}
	}
	
	lrmStaticMesh *LoadSkeletalMesh(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			lrmSkeletalMesh *SkeletalMesh = ResourceManager->GetSkeletalMesh(resource_id);
			if(SkeletalMesh != nullptr)
			{
				return &SkeletalMesh->BindPoseMesh;
			}
			else
			{
				return nullptr;
			}
		}
		else
		{
			return nullptr;
		}
	}
	
	liBitmap2D *LoadBitmap(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			return ResourceManager->GetBitmap(resource_id);
		}
		else
		{
			return nullptr;
		}
	}
	
	liCubemap *LoadCubemap(const std::string &resource_id)
	{
		if(ResourceManager != nullptr)
		{
			return ResourceManager->GetCubemap(resource_id);
		}
		else
		{
			return nullptr;
		}
	}
	
public:
	
	lrGLStaticMeshView GetStaticMesh(const std::string &resource_id)
	{
		auto I = StaticMeshes.find(resource_id);
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *LoadedMesh = LoadStaticMesh(resource_id);
			
			lrGLStaticMeshResource &StaticMeshResource = StaticMeshes[resource_id];
			if(LoadedMesh != nullptr)
			{
				StaticMeshResource.Initialize(*LoadedMesh);
			}
			
			return lrGLStaticMeshView(&StaticMeshes[resource_id]);
		}
		
		return lrGLStaticMeshView(&I->second);
	}
	
	lrGLStaticMeshView GetSkeletalMesh(const std::string &resource_id)
	{
		auto I = SkeletalMeshes.find(resource_id);
		if(I == SkeletalMeshes.end())
		{
			lrmStaticMesh *LoadedMesh = LoadSkeletalMesh(resource_id);
			
			lrGLStaticMeshResource &StaticMeshResource = SkeletalMeshes[resource_id];
			if(LoadedMesh != nullptr)
			{
				StaticMeshResource.Initialize(*LoadedMesh);
			}
			
			return lrGLStaticMeshView(&SkeletalMeshes[resource_id]);
		}
		
		return lrGLStaticMeshView(&I->second);
	}
	
	lrGLTexture2DView GetTexture(const std::string &resource_id)
	{
		auto I = Textures.find(resource_id);
		if(I == Textures.end())
		{
			liBitmap2D *LoadedBitmap = LoadBitmap(resource_id);
			
			lrGLTextureResource	*TextureResource = &Textures[resource_id];
			lrGLTexture2DView	Texture2DView(TextureResource);
			
			if(LoadedBitmap != nullptr)
			{
				TextureResource->Initialize();
				Texture2DView.Fill(*LoadedBitmap);
			}
			
			return Texture2DView;
		}
		
		return lrGLTexture2DView(&I->second);
	}
	
	lrGLTextureCubemapView GetCubemap(const std::string &resource_id)
	{
		auto I = Cubemaps.find(resource_id);
		if(I == Cubemaps.end())
		{
			liCubemap *LoadedCubemap = LoadCubemap(resource_id);
			
			lrGLTextureResource	*TextureResource = &Cubemaps[resource_id];
			lrGLTextureCubemapView	CubemapView(TextureResource);
			
			if(LoadedCubemap != nullptr)
			{
				const void *CubemapData[6] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
				
				CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_UP).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_DOWN).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_LEFT).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_RIGHT).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_FRONT).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_BACK).GetPixelArray();
				
				TextureResource->Initialize();
				CubemapView.Fill(LoadedCubemap->GetSize(),CubemapData);
			}
			
			return CubemapView;
		}
		
		return lrGLTextureCubemapView(&I->second);
	}
	
	void SetResourceManager(liResourceManager *resource_manager)
	{
		ResourceManager = resource_manager;
	}
	
	lrGLResourceLoader()
	{
		
	}
	
	~lrGLResourceLoader()
	{
		
	}
};

#endif // LR_GL_RESOURCE_LOADER_H
