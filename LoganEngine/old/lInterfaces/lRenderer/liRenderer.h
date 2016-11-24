#ifndef LR_RENDERER_H
#define LR_RENDERER_H

#include <iostream>
#include <string>

#include "../../lMath/lMath.h"

#include "liImg.h"
#include "liStaticMesh.h"
#include "lrCharacterAnim/liCharacter.h"
#include "liMultiMaterial.h"
#include "liFramebuffer.h"

#include "liPointLight.h"

#include "liFrustum.h"
#include "liCamera.h"

/*TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#include "../../lRenderer/lrColor.h"

#include "../../lGooey/lGooRenderer.h"
/*TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/


//#include "../../lRenderer/lrSceneGraph/lrsStaticMeshNode.h"
//#include "../../lRenderer/lrSceneGraph/lrsSkeletalMeshNode.h"
#include "lrSceneGraph/liSceneNode.h"

#include "../lResourceManager/liResourceManager.h"


class liPostProcessCommand
{
public:

	virtual void Apply() = 0;

	liPostProcessCommand(){}
	virtual ~liPostProcessCommand(){}
};

class liPostProcessCommandFactory
{
public:
	virtual liPostProcessCommand *CreateBlurCommand(unsigned int blur_count) = 0;
	virtual liPostProcessCommand *CreateBloomCommand(const lmVector3D &min_intensity,unsigned int blur_count) = 0;

	liPostProcessCommandFactory(){}
	virtual ~liPostProcessCommandFactory(){}
};

class liPostProcessor
{
public:

	virtual liPostProcessCommandFactory *GetCommandFactory() = 0;
	virtual void AddPostProcessCommand(liPostProcessCommand *command) = 0;
	virtual void ClearPostProcessCommands() = 0;

	virtual void ApplyPostProcess() = 0;

	liPostProcessor(){}
	virtual ~liPostProcessor(){}

};

/** \brief Ez az interfész fogja össze a grafikus API platformspecifikus függvényhívásait.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liRenderer
{
public:
    /** \brief Ez a függvény összepárosítja a paraméterként kapott resource managert ezzel a rendererrel.
     *
     * \param rm lrm_RM *: az összepárosítandó resource manager
     * \return void
     *
     */
    virtual void SetRM(liResourceManager *rm) = 0;

    virtual liFramebuffer *GetScreen() = 0;

    virtual liFramebuffer *CreateFramebuffer(unsigned int width,unsigned int height) = 0;

    virtual liCamera *CreateCamera(const lmVector3D &position,float yaw,float pitch) = 0;
    virtual liFrustum *CreateFrustum(float top,float bottom,float left,float right,float far,float near) = 0;

    virtual liPointLight *CreatePointLight(float ambient[],float diffuse[],float specular[],float attenuation[],const lmVector3D &position) = 0;
	virtual void FreePointLights() = 0;

    virtual bool CreateGuiRenderer(liGuiRenderer **gui_renderer) = 0;

    virtual liPostProcessor *GetPostProcessor() = 0;


    /** \brief Ez a függvény a pillanatnyilag használt framebuffert letisztítja, és beállítja a projection és view mátrixokat a paraméterben kapott kamera és frustum alapján.
     *
     * \param Frustum const lrFrustum &: A látható térrészt leíró csonkagúla
     * \param Camera const lrCamera &:   A kamera
     * \param BgColor const lrColor &:   A háttér új színe
     * \return void
     *
     */
    virtual void SetScene(const liFrustum *Frustum,const liCamera *Camera,const lrColor &BgColor) = 0;

    /** \brief Ez a függvény letakarítja a display listet.
     *
     * \return void
     *
     */
    virtual void ClearDL() = 0;

    /** \brief Ez a függvény hozzáad egy új statikus mesht a display listhez.
     *
     * \param Mesh lrStaticMesh&:               A display listhez hozzáadandó static mesh
     * \param MultiMaterial lrMultiMaterial&    A static mesh által használt multi material
     * \param SceneNode lrsStaticMeshNode&      A static mesht tartalmazó scene node
     * \return void
     *
     */
    virtual void AddStaticMesh(liStaticMesh &Mesh,liMultiMaterial &MultiMaterial,liSceneNode &SceneNode,liStaticMeshNode &StaticMeshNode) = 0;

    /** \brief Ez a függvény hozzáad egy új skeletal mesht a színtér gráfhoz.
     *
     * \param Mesh lrCharacter&                 A skeletal mesheket tartalmazó karakter
     * \param Skeleton lrSkeleton&              A skeletal mesh testtartását megadó skeleton
     * \param MultiMaterial lrMultiMaterial&    A skeletal mesh által használt multi material
     * \param SceneNode lrsSkeletalMeshNode&    A skeletal mesht tartalmazó scene node
     * \return void
     *
     */
    virtual void AddSkeletalMesh(liCharacter &Mesh,liSkeleton &Skeleton,liMultiMaterial &MultiMaterial,liSceneNode &SceneNode,liSkeletalMeshNode &SkeletalMeshNode) = 0;

    /** \brief Ez a függvény kirajzolja a jelenlegi színteret a jelenleg beállított framebufferbe.
     *
     * \return void
     *
     */
    virtual void Draw(liFramebuffer *framebuffer) = 0;

    /** \brief Konstruktor - nem csinál semmit.
     *
     */
    liRenderer(){};

    /** \brief Virtuális destruktor, nem csinál semmit.
     *
     */
    virtual ~liRenderer(){};
};

#endif
