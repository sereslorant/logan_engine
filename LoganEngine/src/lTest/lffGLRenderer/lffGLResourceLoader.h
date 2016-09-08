#ifndef LFF_GL_RESOURCE_LOADER_H
#define LFF_GL_RESOURCE_LOADER_H

#include "lGLIncludes.h"

#include "../../lResourceManager/lResourceManager.h"

struct lffGLMaterial
{
	GLfloat Diffuse[4] = {0.0,0.0,0.0,1.0};
	GLfloat Specular[4] = {0.0,0.0,0.0,1.0};
	GLfloat Shininess = 0.0;

	void Apply()
	{
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,Diffuse);
		glMaterialfv(GL_FRONT,GL_SPECULAR,Specular);
		glMaterialfv(GL_FRONT,GL_SHININESS,&Shininess);
	}

	void SetMaterial(const liMaterial &material)
	{
		Diffuse[0] = material.GetDiffuse(L_RED_INDEX);
		Specular[0] = material.GetSpecular(L_RED_INDEX);
		//
		Diffuse[1] = material.GetDiffuse(L_GREEN_INDEX);
		Specular[1] = material.GetSpecular(L_GREEN_INDEX);
		//
		Diffuse[2] = material.GetDiffuse(L_BLUE_INDEX);
		Specular[2] = material.GetSpecular(L_BLUE_INDEX);
		//
		Shininess = material.GetShininess();
	}
};

class lffGLStaticMesh
{
private:
	lrmStaticMesh &StaticMesh;
	//
public:
	//
	class lffGLMaterialGroup
	{
	private:
		lrmStaticMesh *StaticMesh = nullptr;
		lrmStaticMesh::lrmMtlGroup *MtlGroup = nullptr;
		//
	public:
		//
		const std::string &GetMaterial()
		{
			if(MtlGroup == nullptr) {return "Dummy";}
			return MtlGroup->Material;
		}
		//
		void Draw()
		{
			if(StaticMesh == nullptr || MtlGroup == nullptr)
				{return;}
			//
			glBegin(GL_TRIANGLES);
			//
			for(unsigned int i=0;i < MtlGroup->IndexBuffer.size();i++)
			{
				unsigned int Index = MtlGroup->IndexBuffer[i];
				glTexCoord2f(StaticMesh->TexCoords[Index][0],StaticMesh->TexCoords[Index][1]);
				glNormal3f(StaticMesh->Normals[Index][0],StaticMesh->Normals[Index][1],StaticMesh->Normals[Index][2]);
				glVertex3f(StaticMesh->Vertices[Index][0],StaticMesh->Vertices[Index][1],StaticMesh->Vertices[Index][2]);
			}
			//
			glEnd();
		}
		//
		void DrawInstanced(lmMatrix4x4 mv_matrices[],lffGLMaterial materials[],unsigned int num_instances)
		{
			for(unsigned int i=0;i < num_instances;i++)
			{
				glPushMatrix();
					//
					Draw();
				glPopMatrix();
			}
		}
		//
		lffGLMaterialGroup()
		{
			//
		}
		//
		lffGLMaterialGroup(lrmStaticMesh *static_mesh,lrmStaticMesh::lrmMtlGroup *mtl_group)
			:StaticMesh(static_mesh),MtlGroup(mtl_group)
		{
			//
		}
	};
	//
	unsigned int GetNumMtlGroups()
	{
		return StaticMesh.MtlGroups.size();
	}
	//
	lffGLMaterialGroup GetMtlGroup(unsigned int index)
	{
		return lffGLMaterialGroup(&StaticMesh,StaticMesh.MtlGroups[index]);
	}
	//
	/*
	void Draw(lrmStaticMesh::lrmMtlGroup &mtl_group)
	{
		glBegin(GL_TRIANGLES);
		//
		/*for(lrmStaticMesh::lrmMtlGroup *MtlGroup : StaticMesh.MtlGroups)
		{* /
			for(unsigned int i=0;i < mtl_group.IndexBuffer.size();i++)
			{
				unsigned int Index = mtl_group.IndexBuffer[i];
				glTexCoord2f(StaticMesh.TexCoords[Index][0],StaticMesh.TexCoords[Index][1]);
				glNormal3f(StaticMesh.Normals[Index][0],StaticMesh.Normals[Index][1],StaticMesh.Normals[Index][2]);
				glVertex3f(StaticMesh.Vertices[Index][0],StaticMesh.Vertices[Index][1],StaticMesh.Vertices[Index][2]);
			}
		//}
		//
		glEnd();
	}
	*/
	//
	lffGLStaticMesh(lrmStaticMesh &static_mesh)
		:StaticMesh(static_mesh)
	{}
	//
	virtual ~lffGLStaticMesh()
	{}
};

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
	lffGLTexture(GLuint texture_id)
		:TextureId(texture_id)
	{
		//
	}
	//
	~lffGLTexture()
	{
		glDeleteTextures(1,&TextureId);
	}
};

class lffGLResourceLoader
{
private:
	liResourceManager &ResourceManager;
	//
	std::map<std::string,lffGLStaticMesh *> StaticMeshes;
	std::map<std::string,lffGLTexture *> Textures;
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
			std::cout << LoadedMesh << std::endl;
			//
			std::cout << LoadedMesh->Vertices[0][0] << std::endl;
			//
			lffGLStaticMesh *NewStaticMesh = new lffGLStaticMesh(*LoadedMesh);
			//
			std::cout << "Assigning" << std::endl;
			StaticMeshes[resource_id] = NewStaticMesh;
			//
			std::cout << "Returning" << std::endl;
			return StaticMeshes[resource_id];
		}
		//
		return I->second;
	}
	//
	lffGLTexture *GetTexture(const std::string &resource_id)
	{
		auto I = Textures.find(resource_id);
		if(I == Textures.end())
		{
			liBitmap *LoadedBitmap = ResourceManager.GetBitmap(resource_id);
			//
			if(LoadedBitmap != nullptr)
			{
				Textures[resource_id] = new lffGLTexture(*LoadedBitmap);
			}
			else
			{
				Textures[resource_id] = new lffGLTexture(0);
			}
			//
			return Textures[resource_id];
		}
		//
		return I->second;
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
		for(auto I : Textures)
		{
			delete I.second;
		}
	}
};

#endif // LFF_GL_RESOURCE_LOADER_H
