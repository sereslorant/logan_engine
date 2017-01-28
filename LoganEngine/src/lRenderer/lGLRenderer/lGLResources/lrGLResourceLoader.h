#ifndef LR_GL_RESOURCE_LOADER_H
#define LR_GL_RESOURCE_LOADER_H

#include "../lGLIncludes.h"
#include "../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!

/*
 * TODO: A bound checkeket újra kéne gondolni.
 */

#include "../../../lResourceManager/liResourceManager.h"

struct lrGLVertexData
{
	unsigned int BufferLength = 0;	/**< A buffer objectek hossza */
	//
	GLuint VertexBuffer		= 0; 	/**< A csúcspontokat tároló buffer object azonosítója */
	GLuint NormalBuffer		= 0;	/**< A normálvektorokat tároló buffer object azonosítója */
	GLuint TangentBuffer	= 0;	/**< A tangent vektorokat tároló buffer object azonosítója */
	GLuint BitangentBuffer	= 0;	/**< A bitangent vektorokat tároló buffer object azonosítója */
	GLuint TexCoordBuffer	= 0;	/**< A textúrakoordinátákat tároló buffer object azonosítója */
	//
	//
	void Initialize(unsigned int buffer_length)
	{
		BufferLength = buffer_length;
		//
		glGenBuffers(1,&VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),nullptr,GL_STATIC_DRAW);
		//
		glGenBuffers(1,&NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),nullptr,GL_STATIC_DRAW);
		//
		glGenBuffers(1,&TangentBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,TangentBuffer);
		glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),nullptr,GL_STATIC_DRAW);
		//
		glGenBuffers(1,&BitangentBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,BitangentBuffer);
		glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector3D),nullptr,GL_STATIC_DRAW);
		//
		glGenBuffers(1,&TexCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,TexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER,BufferLength * sizeof(lmVector2D),nullptr,GL_STATIC_DRAW);
		//
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
	//
	void Fill(lrmStaticMesh &static_mesh)
	{
		glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER,0,BufferLength*sizeof(lmVector3D),&static_mesh.Vertices[0]);
		//
		glBindBuffer(GL_ARRAY_BUFFER,NormalBuffer);
		glBufferSubData(GL_ARRAY_BUFFER,0,BufferLength*sizeof(lmVector3D),&static_mesh.Normals[0]);
		//
		glBindBuffer(GL_ARRAY_BUFFER,TangentBuffer);
		glBufferSubData(GL_ARRAY_BUFFER,0,BufferLength*sizeof(lmVector3D),&static_mesh.Tangents[0]);
		//
		glBindBuffer(GL_ARRAY_BUFFER,BitangentBuffer);
		glBufferSubData(GL_ARRAY_BUFFER,0,BufferLength*sizeof(lmVector3D),&static_mesh.Bitangents[0]);
		//
		glBindBuffer(GL_ARRAY_BUFFER,TexCoordBuffer);
		glBufferSubData(GL_ARRAY_BUFFER,0,BufferLength*sizeof(lmVector2D),&static_mesh.TexCoords[0]);
		//
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
	//
	lrGLVertexData(){}
	//
	~lrGLVertexData()
	{
		glDeleteBuffers(1,&VertexBuffer);
		glDeleteBuffers(1,&NormalBuffer);
		glDeleteBuffers(1,&TangentBuffer);
		glDeleteBuffers(1,&BitangentBuffer);
		glDeleteBuffers(1,&TexCoordBuffer);
	}
	/*
	 * End of class
	 */
};

struct lrGLMaterialGroup
{
public:
	unsigned int BufferLength = 0;  /**< Az index buffer hossza */
	GLuint IndexBuffer = 0;         /**< Az index buffer azonosítója */
	//
	std::string MaterialName;
	//
	void Initialize(unsigned int buffer_length)
	{
		BufferLength = buffer_length;
		//
		glGenBuffers(1,&IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
    	glBufferData(GL_ELEMENT_ARRAY_BUFFER,BufferLength * sizeof(GLuint),nullptr,GL_STATIC_DRAW);
    	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,BufferLength * sizeof(GLushort),nullptr,GL_STATIC_DRAW);
    	//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}
	//
	void Fill(lrmStaticMesh::lrmMtlGroup &mtl_group)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
		//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,0,BufferLength*sizeof(GLuint),&mtl_group.IndexBuffer[0]);
		void *IndexBufferVoidPtr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_WRITE_ONLY);
		GLuint *IndexBufferPtr = (GLuint *)IndexBufferVoidPtr;
		//
		for(unsigned int i=0;i < BufferLength;i++)
		{
			IndexBufferPtr[i] = mtl_group.IndexBuffer[i];
		}
		//
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
		//
		MaterialName = mtl_group.Material;
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}
	//
	lrGLMaterialGroup()
	{
		//Empty
	}
	//
	~lrGLMaterialGroup()
	{
		glDeleteBuffers(1,&IndexBuffer);
	}
	/*
	 * End of class
	 */
};

#include <vector>


#include "../../../lResourceManager/liResourceManager.h"

struct lrGLStaticMeshResource
{
	lrGLVertexData VertexData;
	std::vector<lrGLMaterialGroup> MaterialGroups;
	//
	void Initialize(lrmStaticMesh &static_mesh)
	{
		VertexData.Initialize(static_mesh.Vertices.size());
		VertexData.Fill(static_mesh);
		MaterialGroups.resize(static_mesh.GetNumMtlGroups());
		for(unsigned int i=0;i < MaterialGroups.size();i++)
		{
			MaterialGroups[i].Initialize(static_mesh.GetMaterialGroup(i).IndexBuffer.size());
			MaterialGroups[i].Fill(static_mesh.GetMaterialGroup(i));
		}
	}
	//
	lrGLStaticMeshResource()
	{
		//Empty
	}
	//
	~lrGLStaticMeshResource()
	{
		//Empty
	}
	/*
	 * End of class
	 */
};

#include "../liGLShaderInterfaces.h"

class lrGLMaterialGroupView
{
private:
	lrGLVertexData		*VertexData = nullptr;
	lrGLMaterialGroup	*MaterialGroup = nullptr;
	//
public:
	//
	const std::string &GetMaterial()
	{
		return MaterialGroup->MaterialName;
	}
	//
	void BindAttributes(liGLStaticMeshShader &static_mesh_shader)
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			GLint PositionLocation  = static_mesh_shader.GetVertexLocation();
			GLint NormalLocation    = static_mesh_shader.GetNormalLocation();
			GLint TangentLocation   = static_mesh_shader.GetTangentLocation();
			GLint BitangentLocation = static_mesh_shader.GetBitangentLocation();
			GLint TexCoordLocation  = static_mesh_shader.GetTexCoordLocation();
			//
			glBindBuffer(GL_ARRAY_BUFFER,VertexData->VertexBuffer);
			glEnableVertexAttribArray(PositionLocation);
			glVertexAttribPointer(PositionLocation,3,GL_FLOAT,GL_FALSE,0,0);
			//
			glBindBuffer(GL_ARRAY_BUFFER,VertexData->NormalBuffer);
			glEnableVertexAttribArray(NormalLocation);
			glVertexAttribPointer(NormalLocation,3,GL_FLOAT,GL_FALSE,0,0);
			//
			glBindBuffer(GL_ARRAY_BUFFER,VertexData->TangentBuffer);
			glEnableVertexAttribArray(TangentLocation);
			glVertexAttribPointer(TangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
			//
			glBindBuffer(GL_ARRAY_BUFFER,VertexData->BitangentBuffer);
			glEnableVertexAttribArray(BitangentLocation);
			glVertexAttribPointer(BitangentLocation,3,GL_FLOAT,GL_FALSE,0,0);
			//
			glBindBuffer(GL_ARRAY_BUFFER,VertexData->TexCoordBuffer);
			glEnableVertexAttribArray(TexCoordLocation);
			glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);
		}
	}
	//
	void Draw()
	{
		if(VertexData != nullptr && MaterialGroup != nullptr)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,MaterialGroup->IndexBuffer);
			glDrawElements(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_INT,nullptr);
			//glDrawElements(GL_TRIANGLES,MaterialGroup->BufferLength,GL_UNSIGNED_SHORT,nullptr);
		}
	}
	//
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
	//
	lrGLMaterialGroupView()
	{
		//Empty
	}
	//
	lrGLMaterialGroupView(lrGLVertexData *vertex_data,lrGLMaterialGroup *material_group)
		:VertexData(vertex_data),MaterialGroup(material_group)
	{
		//Empty
	}
	//
	~lrGLMaterialGroupView()
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
	//
public:
	//
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
	//
	lrGLMaterialGroupView GetMaterialGroup(unsigned int id)
	{
		if(StaticMeshResource != nullptr && id < StaticMeshResource->MaterialGroups.size())
		{
			return lrGLMaterialGroupView(&StaticMeshResource->VertexData,&StaticMeshResource->MaterialGroups[id]);
		}
		else
		{
			return lrGLMaterialGroupView();
		}
	}
	//
	lrGLStaticMeshView(lrGLStaticMeshResource *static_mesh_resource)
		:StaticMeshResource(static_mesh_resource)
	{}
	//
	~lrGLStaticMeshView()
	{}
	/*
	 * End of class
	 */
};

#include "../../../lResourceManager/liResourceManager.h"

#include "lGLTextureResource/lrGLTextureResource.h"
#include "lGLTextureResource/lrGLTexture2DView.h"
#include "lGLTextureResource/lrGLTextureCubemapView.h"

#include <map>

class lrGLResourceLoader
{
private:
	liResourceManager *ResourceManager = nullptr;
	//
	std::map<std::string,lrGLStaticMeshResource> StaticMeshes;
	std::map<std::string,lrGLTextureResource> Textures;
	std::map<std::string,lrGLTextureResource> Cubemaps;
	//
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
	//
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
	//
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
	//
public:
	//
	lrGLStaticMeshView GetStaticMesh(const std::string &resource_id)
	{
		auto I = StaticMeshes.find(resource_id);
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *LoadedMesh = LoadStaticMesh(resource_id);
			//
			lrGLStaticMeshResource &StaticMeshResource = StaticMeshes[resource_id];
			if(LoadedMesh != nullptr)
			{
				StaticMeshResource.Initialize(*LoadedMesh);
			}
			//
			return lrGLStaticMeshView(&StaticMeshes[resource_id]);
		}
		//
		return lrGLStaticMeshView(&I->second);
	}
	//
	lrGLTexture2DView GetTexture(const std::string &resource_id)
	{
		auto I = Textures.find(resource_id);
		if(I == Textures.end())
		{
			liBitmap2D *LoadedBitmap = LoadBitmap(resource_id);
			//
			lrGLTextureResource	*TextureResource = &Textures[resource_id];
			lrGLTexture2DView	Texture2DView(TextureResource);
			//
			if(LoadedBitmap != nullptr)
			{
				TextureResource->Initialize();
				Texture2DView.Fill(*LoadedBitmap);
			}
			//
			return Texture2DView;
		}
		//
		return lrGLTexture2DView(&I->second);
	}
	//
	lrGLTextureCubemapView GetCubemap(const std::string &resource_id)
	{
		auto I = Cubemaps.find(resource_id);
		if(I == Cubemaps.end())
		{
			liCubemap *LoadedCubemap = LoadCubemap(resource_id);
			//
			lrGLTextureResource	*TextureResource = &Cubemaps[resource_id];
			lrGLTextureCubemapView	CubemapView(TextureResource);
			//
			if(LoadedCubemap != nullptr)
			{
				const void *CubemapData[6] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
				//
				CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_UP).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_Y - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_DOWN).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_LEFT).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_X - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_RIGHT).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_NEGATIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_FRONT).GetPixelArray();
				CubemapData[GL_TEXTURE_CUBE_MAP_POSITIVE_Z - GL_TEXTURE_CUBE_MAP_POSITIVE_X] = (void *)LoadedCubemap->GetSide(liCubemap::LI_CUBEMAP_BACK).GetPixelArray();
				//
				TextureResource->Initialize();
				CubemapView.Fill(LoadedCubemap->GetSize(),CubemapData);
			}
			//
			return CubemapView;
		}
		//
		return lrGLTextureCubemapView(&I->second);
	}
	//
	void SetResourceManager(liResourceManager *resource_manager)
	{
		ResourceManager = resource_manager;
	}
	//
	lrGLResourceLoader()
	{
		//
	}
	//
	~lrGLResourceLoader()
	{
		/*
		for(auto I : StaticMeshes)
		{
			delete I.second;
		}*/
		//
		/*
		for(auto I : Textures)
		{
			delete I.second;
		}
		*/
	}
};

#endif // LR_GL_RESOURCE_LOADER_H
