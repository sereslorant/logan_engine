#ifndef DUMMY_RM_H
#define DUMMY_RM_H

#include "../../lResourceManager/lResourceManager.h"

struct Color
{
	float R;
	float G;
	float B;
	//
	Color()
	{
		R = 0.0;
		G = 0.0;
		B = 0.0;
	}
	//
	Color(float r,float g,float b)
	{
		R = r;
		G = g;
		B = b;
	}
};

class DummyImage : public liBitmap2D
{
private:
    //
	constexpr static unsigned int TEXTURE_X = 2;
	constexpr static unsigned int TEXTURE_Y = 2;
    //
	constexpr static unsigned int NUM_TEXTURES = 6;
	//
	// Sky blue: Color(140.0/255.0,190.0/255.0,214.0/255.0)
	// Pine green: Color(1.0/255.0,121.0/255.0,111.0/255.0)
	//
	Color Texture[NUM_TEXTURES][TEXTURE_X][TEXTURE_Y] = {
												{{Color(1.0,1.0,0.8),Color(1.0,0.8,1.0)},{Color(1.0,0.8,0.8),Color(1.0,1.0,1.0)}},
												{{Color(0.0,0.2,1.0),Color(0.4,0.2,0.8)},{Color(0.0,0.2,0.8),Color(0.4,0.2,1.0)}},
												{{Color(1.0,1.0,1.0),Color(1.0,1.0,1.0)},{Color(1.0,1.0,1.0),Color(1.0,1.0,1.0)}},
												//
												{{Color(170.0/255.0,170.0/255.0,214.0/255.0),Color(170.0/255.0,170.0/255.0,214.0/255.0)},{Color(170.0/255.0,170.0/255.0,214.0/255.0),Color(170.0/255.0,170.0/255.0,214.0/255.0)}},
												{{Color(170.0/255.0,170.0/255.0,214.0/255.0),Color(170.0/255.0,170.0/255.0,214.0/255.0)},{Color(130.0/255.0,160.0/255.0,100.0/255.0),Color(130.0/255.0,160.0/255.0,100.0/255.0)}},
												{{Color(130.0/255.0,160.0/255.0,100.0/255.0),Color(130.0/255.0,160.0/255.0,100.0/255.0)},{Color(130.0/255.0,160.0/255.0,100.0/255.0),Color(130.0/255.0,160.0/255.0,100.0/255.0)}},
												};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.65,0.65,0.35),Color(0.5,0.65,0.35)},{Color(0.65,0.35,0.35),Color(0.5,0.5,0.5)}};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.695,0.695,0.35),Color(0.56,0.56,0.35)},{Color(0.695,0.56,0.35),Color(0.56,0.56,0.35)}};
			//
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.75,0.75,0.75),Color(0.75,0.75,0.75)},{Color(0.75,0.75,0.75),Color(0.75,0.75,0.75)}};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.75,0.75,0.75),Color(0.35,0.35,0.35)},{Color(0.65,0.65,0.65),Color(0.5,0.5,0.5)}};
			//
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.35,0.35,0.35),Color(0.35,0.35,0.35)},{Color(0.35,0.35,0.35),Color(0.35,0.35,0.35)}};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.75,0.75,0.0),Color(0.75,0.75,0.0)},{Color(0.75,0.75,0.0),Color(0.75,0.75,0.0)}};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.0,0.0,0.75),Color(0.0,0.0,0.75)},{Color(0.0,0.0,0.75),Color(0.0,0.0,0.75)}};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.0,0.75,0.0),Color(0.0,0.75,0.0)},{Color(0.0,0.75,0.0),Color(0.0,0.75,0.0)}};
			//Color Texture[CUBEMAP_SIZE][CUBEMAP_SIZE] = {{Color(0.75,0.0,0.0),Color(0.75,0.0,0.0)},{Color(0.75,0.0,0.0),Color(0.75,0.0,0.0)}};
	//
	unsigned int TexId = 0;
    //
public:
    //
    virtual int32_t GetWidth() const override
	{
    	return TEXTURE_X;
	}
    //
    virtual int32_t GetHeight() const override
	{
    	return TEXTURE_Y;
	}
    //
    virtual const char *GetPixelArray() const override
    {
    	return (char *)&Texture[TexId][0][0];
    }
    //
    /*
    virtual liSampler &GetSampler() override
    {

    }
	*/
	DummyImage(unsigned int tex_id)
		:TexId(tex_id % NUM_TEXTURES)
	{}
	//
	virtual ~DummyImage() override
	{}
};

class DummyCubemap : public liCubemap
{
private:
	static constexpr unsigned int CUBEMAP_SIZE = 2;
	DummyImage *CubemapSides[6] = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
	//
public:
	//
	virtual int32_t GetSize() const override
	{
		return CUBEMAP_SIZE;
	}
	//
	virtual const liBitmap2D &GetSide(LI_CUBEMAP_SIDE cubemap_side) const override
	{
		return *CubemapSides[cubemap_side];
	}
	//
	DummyCubemap(DummyImage *cubemap_sides[6])
	{
		for(int i=0;i < 6;i++)
		{
			CubemapSides[i] = cubemap_sides[i];
		}
	}
	//
	virtual ~DummyCubemap() override
	{}
	/*
	 * End of class
	 */
};

class DummyRM : public liResourceManager
{
private:
	lrmWfObjLoaderModule	ObjLoaderModule;
	lrmMd5LoaderModule		Md5LoaderModule;
	std::map<std::string,lrmStaticMesh *> StaticMeshes;
	std::map<std::string,lrmSkeletalMesh *> SkeletalMeshes;
	//
	DummyImage *Image = new DummyImage(0);
	DummyImage *Image2 = new DummyImage(1);
	DummyImage *Image3 = new DummyImage(2);
	//
	DummyImage *Top = new DummyImage(3);
	DummyImage *Middle = new DummyImage(4);
	DummyImage *Bottom = new DummyImage(5);
	//
	DummyCubemap *Cubemap;
	DummyCubemap *Cubemap2;
	DummyCubemap *Cubemap3;
	DummyCubemap *EnvMap;
	//
public:
	//
	virtual lrmStaticMesh *GetStaticMesh(const std::string &mesh_name) override
	{
		auto I = StaticMeshes.find(mesh_name);
		//
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *StaticMesh = nullptr;
			if(mesh_name == "Sphere")
			{
				//lrmResourceManager::GenerateSphere(*StaticMesh,16,8);
				//lrmResourceManager::GenerateSphere(*StaticMesh,32,16);
				StaticMesh = new lrmStaticMesh;
				lrmResourceManager::GenerateSphere(*StaticMesh,64,32);
			}
			else if(mesh_name == "Cone")
			{
				//std::cout << "Cone loaded" << std::endl;
				StaticMesh = new lrmStaticMesh;
				lrmResourceManager::GenerateSphere(*StaticMesh,3,3);
			}
			else if(mesh_name == "Box")
			{
				StaticMesh = new lrmStaticMesh;
				lrmResourceManager::GenerateCube(*StaticMesh,false);
			}
			else if(mesh_name == "Box_InsideOut")
			{
				StaticMesh = new lrmStaticMesh;
				lrmResourceManager::GenerateCube(*StaticMesh,true);
			}
			else if(mesh_name == "LoadedMesh")
			{
				if(!ObjLoaderModule.LoadStaticMesh("IdiotaAlakzat.txt",StaticMesh))
				{
					StaticMesh = new lrmStaticMesh;
					lrmResourceManager::GenerateCube(*StaticMesh,false);
				}
			}
			else if(mesh_name == "Anything")
			{
				//lrmResourceManager::GenerateSphere(*StaticMesh,32,16);
				StaticMesh = new lrmStaticMesh;
				lrmResourceManager::GenerateSphere(*StaticMesh,64,32);
			}
			else
			{
				//lrmResourceManager::GenerateSphere(*StaticMesh,16,8);
				StaticMesh = new lrmStaticMesh;
				lrmResourceManager::GenerateSphere(*StaticMesh,64,32);
			}
			//
			StaticMeshes[mesh_name] = StaticMesh;
			//
			return StaticMesh;
		}
		else
		{
			return I->second;
		}
	}
	//
	virtual lrmSkeletalMesh *GetSkeletalMesh(const std::string &resource_identifier) override
	{
		auto I = SkeletalMeshes.find(resource_identifier);
		//
		if(I == SkeletalMeshes.end())
		{
			lrmSkeletalMesh *SkeletalMesh;
			if(!Md5LoaderModule.LoadSkeletalMesh("bob_lamp_update_export.md5mesh",SkeletalMesh))
			{
				return nullptr;
			}
			return SkeletalMesh;
		}
		else
		{
			return I->second;
		}
	}
	//
	virtual liBitmap2D *GetBitmap(const std::string &resource_identifier) override
	{
		if(resource_identifier == "Puszcsy")
		{
			return Image2;
		}
		else if(resource_identifier == "Empty")
		{
			return Image3;
		}
		return Image;
	}
	//
	virtual liCubemap *GetCubemap(const std::string &resource_identifier) override
	{
		if(resource_identifier == "DUMMY")
		{
			return EnvMap;
		}
		else if(resource_identifier == "Puszcsy")
		{
			return Cubemap2;
		}
		else if(resource_identifier == "Empty")
		{
			return Cubemap3;
		}
		return Cubemap;
	}
	//
	DummyRM()
	{
		DummyImage *CubemapSides[6] = {Image,Image,Image,Image,Image,Image};
		Cubemap = new DummyCubemap(CubemapSides);
		DummyImage *CubemapSides2[6] = {Image2,Image2,Image2,Image2,Image2,Image2};
		Cubemap2 = new DummyCubemap(CubemapSides2);
		DummyImage *CubemapSides3[6] = {Image3,Image3,Image3,Image3,Image3,Image3};
		Cubemap3 = new DummyCubemap(CubemapSides3);
		//
		DummyImage *EnvMapSides[6] = {Top,Bottom,Middle,Middle,Middle,Middle};
		EnvMap = new DummyCubemap(EnvMapSides);
	}
	//
	virtual ~DummyRM() override
	{
		delete Image;
		delete Image2;
		delete Image3;
		//
		delete Cubemap;
		delete Cubemap2;
		delete Cubemap3;
		//
		delete EnvMap;
		//
		for(auto I : StaticMeshes)
		{
			delete I.second;
		}
	}
};

#endif
