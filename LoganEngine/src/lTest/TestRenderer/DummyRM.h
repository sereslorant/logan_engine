#ifndef DUMMY_RM_H
#define DUMMY_RM_H

#include "../../lResourceManager/lResourceManager.h"

class DummyImage : public liBitmap
{
private:
	//
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
    //
	constexpr static unsigned int TEXTURE_X = 2;
	constexpr static unsigned int TEXTURE_Y = 2;
    //
	Color Texture[2][TEXTURE_X][TEXTURE_Y] = {
												{{Color(0.0,1.0,0.0),Color(0.0,0.8,0.2)},{Color(0.0,0.8,0.0),Color(0.0,1.0,0.2)}},
												{{Color(0.0,0.2,1.0),Color(0.4,0.2,0.8)},{Color(0.0,0.2,0.8),Color(0.4,0.2,1.0)}}
												};
	//
	unsigned int TexId = 0;
    //
public:
    //
    virtual int32_t GetWidth() override
	{
    	return TEXTURE_X;
	}
    //
    virtual int32_t GetHeight() override
	{
    	return TEXTURE_Y;
	}
    //
    virtual char *GetPixelArray() override
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
		:TexId(tex_id % 2)
	{}
	//
	virtual ~DummyImage() override
	{}
};

class DummyRM : public liResourceManager
{
private:
	std::map<std::string,lrmStaticMesh *> StaticMeshes;

	DummyImage *Image = new DummyImage(0);
	DummyImage *Image2 = new DummyImage(1);

public:
	//
	virtual lrmStaticMesh *GetStaticMesh(const std::string &mesh_name) override
	{
		auto I = StaticMeshes.find(mesh_name);
		//
		if(I == StaticMeshes.end())
		{
			lrmStaticMesh *StaticMesh = new lrmStaticMesh;
			if(mesh_name == "Sphere")
			{
				lrmResourceManager::GenerateSphere(*StaticMesh,16,8);
			}
			else if(mesh_name == "Cone")
			{
				//std::cout << "Cone loaded" << std::endl;
				lrmResourceManager::GenerateSphere(*StaticMesh,3,3);
			}
			else if(mesh_name == "Box")
			{
				lrmResourceManager::GenerateCube(*StaticMesh,false);
			}
			else if(mesh_name == "Box_InsideOut")
			{
				lrmResourceManager::GenerateCube(*StaticMesh,true);
			}
			else
			{
				lrmResourceManager::GenerateSphere(*StaticMesh,16,8);
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
	virtual liBitmap *GetBitmap(const std::string &resource_identifier) override
	{
		if(resource_identifier == "Puszcsy")
		{
			return Image2;
		}
		return Image;
	}
	//
	DummyRM()
	{}
	//
	virtual ~DummyRM() override
	{
		delete Image;

		for(auto I : StaticMeshes)
		{
			delete I.second;
		}
	}
};

#endif
