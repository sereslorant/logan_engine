#ifndef LFF_GL_RESOURCE_LOADER_H
#define LFF_GL_RESOURCE_LOADER_H

#include "../../lRenderer/lGLRenderer/lGLIncludes.h"

#include "../../lResourceManager/lResourceManager.h"

class lffGLStaticMesh
{
private:
	lrmStaticMesh &StaticMesh;
	std::vector<GLuint> DisplayLists;
	//
public:
	//
	static void DrawMesh(lrmStaticMesh &static_mesh,lrmStaticMesh::lrmMtlGroup &mtl_group)
	{
		glBegin(GL_TRIANGLES);
		//
		for(unsigned int i=0;i < mtl_group.IndexBuffer.size();i++)
		{
			unsigned int Index = mtl_group.IndexBuffer[i];
			glTexCoord2f(static_mesh.TexCoords[Index][0],static_mesh.TexCoords[Index][1]);
			glNormal3f(static_mesh.Normals[Index][0],static_mesh.Normals[Index][1],static_mesh.Normals[Index][2]);
			glVertex3f(static_mesh.Vertices[Index][0],static_mesh.Vertices[Index][1],static_mesh.Vertices[Index][2]);
		}
		//
		glEnd();
	}
	//
	class lffGLMaterialGroup
	{
	private:
		lrmStaticMesh *StaticMesh = nullptr;
		lrmStaticMesh::lrmMtlGroup *MtlGroup = nullptr;
		GLuint DisplayList = 0;
		//
	public:
		//
		const std::string &GetMaterial()
		{
			return MtlGroup->Material;
		}
		//
		void Draw()
		{
			if(StaticMesh == nullptr || MtlGroup == nullptr)
				{return;}
			//
			glCallList(DisplayList);
		}
		//
		lffGLMaterialGroup()
		{
			//
		}
		//
		lffGLMaterialGroup(lrmStaticMesh *static_mesh,lrmStaticMesh::lrmMtlGroup *mtl_group,GLuint display_list)
			:StaticMesh(static_mesh),MtlGroup(mtl_group),DisplayList(display_list)
		{
			//
		}
	};
	//
	unsigned int GetNumMtlGroups()
	{
		return StaticMesh.GetNumMtlGroups();
	}
	//
	const std::string &GetMaterialName(unsigned int mat_group_id)
	{
		return StaticMesh.GetMaterialGroup(mat_group_id).Material;
	}
	//
	lffGLMaterialGroup GetMtlGroup(unsigned int index)
	{
		return lffGLMaterialGroup(&StaticMesh,&StaticMesh.GetMaterialGroup(index),DisplayLists[index]);
	}
	//
	lffGLStaticMesh(lrmStaticMesh &static_mesh)
		:StaticMesh(static_mesh)
	{
		DisplayLists.resize(StaticMesh.GetNumMtlGroups());
		for(unsigned int i=0;i < DisplayLists.size();i++)
		{
			DisplayLists[i] = glGenLists(1);

			glNewList(DisplayLists[i],GL_COMPILE);
				DrawMesh(StaticMesh,StaticMesh.GetMaterialGroup(i));
			glEndList();
		}
	}
	//
	virtual ~lffGLStaticMesh()
	{
		for(auto Index : DisplayLists)
		{
			glDeleteLists(Index,1);
		}
	}
};

#include "../../lRenderer/lGLRenderer/lGLResources/lrGLTextureResource.h"

/*
class lffGLTexture
{
private:
	GLuint TextureId;
	//
public:
	//
	void Enable()
	{
		glBindTexture(GL_TEXTURE_2D,TextureId);
	}
	//
	void Disable()
	{
		glBindTexture(GL_TEXTURE_2D,0);
	}
	//
	lffGLTexture(GLuint texture_id)
		:TextureId(texture_id)
	{
		//
	}
	//
	lffGLTexture(liBitmap &bitmap)
	{
		glGenTextures(1,&TextureId);
		glBindTexture(GL_TEXTURE_2D,TextureId);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGB, GL_FLOAT, bitmap.GetPixelArray());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D,0);
	}
	//
	~lffGLTexture()
	{
		glDeleteTextures(1,&TextureId);
	}
};
*/
#include <string>
/*
class liResourcePath
{
public:
	//
	virtual void GetRest(std::string &str) = 0;
	//
	virtual void GetChild(std::string &str) = 0;
	//
	liResourcePath(){}
	virtual ~liResourcePath(){}
};
*//*
class luResourcePath //: public liResourcePath
{
private:
	std::istringstream StringStream;
	const char Delim;
	//
public:
	//
	virtual void GetRest(std::string &str) //override
	{
		std::getline(StringStream,str);
	}
	//
	virtual void GetChild(std::string &str) //override
	{
		std::getline(StringStream,str,Delim);
	}
	//
	luResourcePath(const std::string &str,const char delim)
		:StringStream(str),Delim(delim)
	{
		//
	}
	//
	virtual ~luResourcePath() //override
	{
		//
	}
};*/
/*
template<class Texture_T,class StaticMesh_T,class SkeletalMesh_T>
class lrResourceLoaderBase
{
public:
	//
	virtual Texture_T &GetTexture(const std::string &texture_path)
	{
		return Texture_T::NULL_OBJECT;
	}
	//
	virtual StaticMesh_T &GetStaticMesh(const std::string &static_mesh_path)
	{
		return StaticMesh_T::NULL_OBJECT;
	}
	//
	virtual SkeletalMesh_T &GetSkeletalMesh(const std::string &skeletal_mesh_path)
	{
		return SkeletalMesh_T::NULL_OBJECT;
	}
	//
	lrResourceLoaderBase()
	{}
	//
	virtual ~lrResourceLoaderBase()
	{}
};

template<class Texture_T,class StaticMesh_T,class SkeletalMesh_T>
class lrResourceLoader : lrResourceLoaderBase<Texture_T,StaticMesh_T,SkeletalMesh_T>
{
private:

public:
	//
	virtual Texture_T &GetTexture()
	{
		return Texture_T::NULL_OBJECT;
	}
	//
	virtual StaticMesh_T &GetStaticMesh()
	{
		return StaticMesh_T::NULL_OBJECT;
	}
	virtual SkeletalMesh_T &GetSkeletalMesh()
	{
		return SkeletalMesh_T::NULL_OBJECT;
	}
	//
	lrResourceLoader()
	{}
	//
	virtual ~lrResourceLoader() override
	{}
};
*/
class lffGLResourceLoader
{
private:
	liResourceManager &ResourceManager;
	//
	std::map<std::string,lffGLStaticMesh *> StaticMeshes;
	//std::map<std::string,lffGLTexture *> Textures;
	std::map<std::string,lrGLTextureResource> Textures;
	//
public:
	//
	lffGLStaticMesh *GetStaticMesh(const std::string &resource_id)
	{
		auto I = StaticMeshes.find(resource_id);
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *LoadedMesh = ResourceManager.GetStaticMesh(resource_id);
			//
			lffGLStaticMesh *NewStaticMesh = new lffGLStaticMesh(*LoadedMesh);
			//
			StaticMeshes[resource_id] = NewStaticMesh;
			//
			return StaticMeshes[resource_id];
		}
		//
		return I->second;
	}
	//
	lrGLTexture2DView GetTexture(const std::string &resource_id)
	{
		auto I = Textures.find(resource_id);
		if(I == Textures.end())
		{
			liBitmap *LoadedBitmap = ResourceManager.GetBitmap(resource_id);
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
	lffGLResourceLoader(liResourceManager &resource_manager)
		:ResourceManager(resource_manager)
	{
		//
	}
	//
	~lffGLResourceLoader()
	{
		for(auto I : StaticMeshes)
		{
			delete I.second;
		}
		//
		/*
		for(auto I : Textures)
		{
			delete I.second;
		}
		*/
	}
};

#endif // LFF_GL_RESOURCE_LOADER_H
