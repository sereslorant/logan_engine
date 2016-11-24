#ifndef LRM_RESOURCE_MANAGER
#define LRM_RESOURCE_MANAGER

#include "../lInterfaces/lResourceManager/liResourceManager.h"

#include "lrmPhWavefrontMesh.h"
#include "lrmPhHeightmapMesh.h"



#include "BMP.h"

#include <string>
#include <map>

/** \brief A resource managger felelős minden assetért, amit fájlból olvasunk be. Tőle lehet elkérni a materialokat, a textúrákat, a modelleket és az animációkat.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrmResourceManager : public liResourceManager
{
protected:
    liGPUFactory *GPUFactory; /**< A resource managerhez bindolt renderer. Ő töti fel a modelleket és textúrákat a videoramba. */
    liCharAnimFactory *CharAnimFactory;

    std::string ImagePath; /**< A textúrákat tartalmazó mappa útvonala */
    std::map<std::string,liImg *> LoadedImages; /**< A betöltött képekre mutató pointereket tartalmazó asszociatív tömb. A kulcs a fájlnév. */

    std::string CubemapPath; /**< A textúrákat tartalmazó mappa útvonala */
    std::map<std::string,liImg *> LoadedCubemaps; /**< A betöltött képekre mutató pointereket tartalmazó asszociatív tömb. A kulcs a fájlnév. */

    std::map<std::string,liFramebuffer *> AddedFramebuffers;

    std::string StaticMeshPath; /**< A statikus mesheket tartalmazó mappa útvonala */
    std::string HeightmapPath; /**< A heightmapeket tartalmazó mappa útvonala */
    std::map<std::string,liStaticMesh *> LoadedStaticMeshes; /**< A betöltött statikus meshekre mutató pointereket tartalmazó asszociatív tömb. A kulcs a fájlnév. */

    std::string SkeletalMeshPath; /**< A skeletal mesheket tartalmazó mappa útvonala */
    std::map<std::string,liSkeletalMesh *> LoadedSkeletalMeshes; /**< A betöltött skeletal meshekre mutató pointereket tartalmazó asszociatív tömb. A kulcsról egyelőre inkább ne beszéljünk. xPPP */

    std::string SkeletalAnimPath; /**< A skeletal meshek animációit tartalmazó mappa útvonala */
    std::map<std::string,liKeyframeAnimation *> LoadedSkeletalAnimations; /**< A betöltött animációkra mutató pointereket tartalmazó asszociatív tömb. A kulcs a fájlnév. */

    std::string CharacterPath; /**< A karaktereket tartalmazó mappa útvonala */
    std::map<std::string,liCharacter *> LoadedCharacters; /**< A betöltött karakterekre mutató pointereket tartalmazó asszociatív tömb. A kulcs a fájlnév. */

    std::string MaterialPath; /**< A materialokat tartalmazó mappa útvonala */
    std::map<std::string,liMultiMaterial *> LoadedMaterials; /**< A betöltött materialokra mutató pointereket tartalmazó asszociatív tömb. A kulcs a fájlnév. */

public:

    virtual void SetGPUFactory(liGPUFactory *gpu_factory) override
    {
        GPUFactory = gpu_factory;
        //Renderer->BindRM(this);
    }

	virtual void SetCharAnimFactory(liCharAnimFactory *char_anim_factory) override
	{
		CharAnimFactory = char_anim_factory;
	}

    virtual void Clean() override
    {
        for(auto I : LoadedImages)
        {
            delete I.second;
        }
        LoadedImages.clear();

        for(auto I : LoadedCubemaps)
        {
            delete I.second;
        }
        LoadedCubemaps.clear();

        for(auto I : AddedFramebuffers)
        {
            delete I.second;
        }
        AddedFramebuffers.clear();

        for(auto I : LoadedStaticMeshes)
        {
            delete I.second;
        }
        LoadedStaticMeshes.clear();

        for(auto I : LoadedSkeletalMeshes)
        {
            delete I.second;
        }
        LoadedSkeletalMeshes.clear();

        for(auto I : LoadedSkeletalAnimations)
        {
            delete I.second;
        }
        LoadedSkeletalAnimations.clear();

        for(auto I : LoadedCharacters)
        {
            delete I.second;
        }
        LoadedCharacters.clear();

        for(auto I : LoadedMaterials)
        {
            delete I.second;
        }
        LoadedMaterials.clear();
    }

    virtual void AddFramebuffer(const std::string &name,liFramebuffer *framebuffer) override
    {
    	auto I = AddedFramebuffers.find(name);

    	if(I == AddedFramebuffers.end())
		{
			AddedFramebuffers[name] = framebuffer;
		}
    }

    virtual liImg *LoadImg(const std::string &filename) override;
    virtual liImg *LoadCubemap(const std::string &filename) override
    {
    	std::map<std::string,liImg *>::iterator I = LoadedCubemaps.find(filename);

		if(I == LoadedCubemaps.end())
		{
			BMP *Bitmaps[6];
			Bitmaps[0] = new BMP(CubemapPath + filename + "/Right.bmp");
			Bitmaps[1] = new BMP(CubemapPath + filename + "/Left.bmp");
			Bitmaps[2] = new BMP(CubemapPath + filename + "/Top.bmp");
			Bitmaps[3] = new BMP(CubemapPath + filename + "/Bottom.bmp");
			Bitmaps[4] = new BMP(CubemapPath + filename + "/Front.bmp");
			Bitmaps[5] = new BMP(CubemapPath + filename + "/Back.bmp");

			liBitmap *IBitmap[6];
			for(int i=0;i < 6;i++)
			{
				IBitmap[i] = Bitmaps[i];
			}

			liImg *Img;
			GPUFactory->UploadCubemap(Img,IBitmap);

			LoadedCubemaps[filename] = Img;

			for(int i=0;i < 6;i++)
			{
				delete Bitmaps[i];
			}
		}

		return LoadedCubemaps[filename];
    }

    virtual liStaticMesh *LoadStaticMesh(const std::string &filename) override;
    virtual liStaticMesh *LoadHeightmap(const std::string &filename) override;
    virtual liSkeletalMesh *LoadSkeletalMesh(const std::string &filename) override
    {
    	auto I = LoadedSkeletalMeshes.find(filename);

    	if(I == LoadedSkeletalMeshes.end())
		{
			return nullptr;
		}
		else
		{
			return I->second;
		}
    }


    virtual liPhMesh *LoadPhysMesh(const std::string &filename,bool flip_normals) override
    {
    	return new lrmPhWavefrontMesh(filename,flip_normals);
    }

    virtual liPhMesh *LoadPhysHeightmap(const std::string &filename,float tile_x,float tile_y,float tile_z) override
    {
    	return new lrmPhHeightmapMesh(filename,tile_x,tile_y,tile_z);
    }

    virtual liCharacter *LoadCharacter(const std::string &filename) override;
    virtual liKeyframeAnimation *LoadSkeletalAnimation(const std::string &filename) override;
    virtual liMultiMaterial *LoadMtl(const std::string &filename) override;

    /** \brief Konstruktor, amely beállítja az alapértelmezett materialt.
     *
     * \param defDiffuse float*             a default material diffuse intenzitása
     * \param defSpecular float*            a default material specular intenzitása
     * \param defShininess float            a default material shininess intenzitása
     * \param defDiffMap const std::string& a default material diffuse mapje
     * \param defNrmMap const std::string&  a default material normal mapje
     *
     */
    lrmResourceManager(float *defDiffuse,float *defSpecular,float defShininess);

    /** \brief Destruktor, lehívja a clean függvényt.
     *
     */
    virtual ~lrmResourceManager() override;
};

#endif
