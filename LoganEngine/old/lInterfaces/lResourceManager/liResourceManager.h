#ifndef LI_RESOURCE_MANAGER_H
#define LI_RESOURCE_MANAGER_H

#include "liGPUFactory.h"
#include "liCharAnimFactory.h"

#include "../lRenderer/liFramebuffer.h"
#include "../lRenderer/liImg.h"
#include "../lRenderer/liStaticMesh.h"
#include "../lRenderer/lrCharacterAnim/liCharacter.h"
#include "../lRenderer/liMultiMaterial.h"

#include "../lPhys/liPhMesh.h"

class liResourceManager
{
public:

	virtual void SetGPUFactory(liGPUFactory *gpu_factory) = 0;
	virtual void SetCharAnimFactory(liCharAnimFactory *char_anim_factory) = 0;

    /** \brief Ez a tagfüggvény minden beolvasott assetet felszabadít.
     *
     */
	virtual void Clean() = 0;

    /** \brief Képbetöltő függvény, a paraméterben kapott fájlból betölt egy bitképet, feltölteti a rendererrel a videoramba, és visszaad egy lrImg-re mutató pointert, amivel el lehet érni.
     *
     * \param filename const std::string & - a bitképet tartalmazó fájl neve
     * \return lrImg * - objektum, amelyen keresztül el lehet érni a videoramba feltöltött bitképet.
     *
     */
    virtual void AddFramebuffer(const std::string &name,liFramebuffer *framebuffer) = 0;

    /** \brief
     *
     * \param filename const std::string & - a bitképet tartalmazó fájl neve
     * \return lrImg * - objektum, amelyen keresztül el lehet érni a videoramba feltöltött bitképet.
     *
     */
    virtual liImg *LoadImg(const std::string &filename) = 0;

    /** \brief
     *
     * \param filename const std::string & - a bitképet tartalmazó fájl neve
     * \return lrImg * - objektum, amelyen keresztül el lehet érni a videoramba feltöltött bitképet.
     *
     */
    virtual liImg *LoadCubemap(const std::string &filename) = 0;

    /** \brief
     *
     * \param filename const std::string & - a static mesht tartalmazó fájl neve
     * \return lrStaticMesh * - objektum, amellyel ki lehet rajzolni a mesht.
     *
     */
    virtual liStaticMesh *LoadStaticMesh(const std::string &filename) = 0;

    /** \brief
     *
     * \param filename const std::string &
     * \return lrStaticMesh*
     *
     */
    virtual liStaticMesh *LoadHeightmap(const std::string &filename) = 0;

    virtual liSkeletalMesh *LoadSkeletalMesh(const std::string &filename) = 0;

    virtual liPhMesh *LoadPhysMesh(const std::string &filename,bool flip_normals) = 0;
    virtual liPhMesh *LoadPhysHeightmap(const std::string &filename,float tile_x,float tile_y,float tile_z) = 0;


    /** \brief
     *
     * \param filename const std::string & - a karaktert tartalmazó fájl neve
     * \return lrCharacter * - objektum, amellyel ki lehet rajzolni a karaktert.
     *
     */
    virtual liCharacter *LoadCharacter(const std::string &filename) = 0;

    /** \brief Skeletal animációbetöltő függvény, a paraméterben kapott fájlból betölt egy skeletal animationt, és visszaad egy rá mutató pointert.
     *
     * \param filename const std::string &
     * \return lrKeyframeAnimation * -
     *
     */
    virtual liKeyframeAnimation *LoadSkeletalAnimation(const std::string &filename) = 0;

    /** \brief Material betöltő függvény, a paraméterben kapott fájlból betölt egy multi materialt, és visszaad egy rá mutató pointert.
     *
     * \param filename const std::string & - a materialt tartalmazó fájl neve
     * \return lrMultiMaterial * - objektum, amely tartalmazza a material propertyket.
     *
     */
    virtual liMultiMaterial *LoadMtl(const std::string &filename) = 0;

	liResourceManager()
	{
		//Üres függvénytörzs
	}

	virtual ~liResourceManager()
	{
		//Üres függvénytörzs
	}
};

#endif // LI_RESOURCE_MANAGER_H
