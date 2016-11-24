#ifndef LR_GL_3_3_SKM_SHADER
#define LR_GL_3_3_SKM_SHADER

#include "../lrGLShader/lrGLSkmShader.h"

#include "../../../lApiAdapter/lDLLFunctions.h"

/** \brief Egy OpenGL 3.3-at használó skeletal mesh shader
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGL3_3SkmShader : public lrGLSkmShader
{
private:
	GLuint VsId;    /**< A vertex shader azonosítója */
	GLuint FsId;    /**< A fragment shader azonosítója */

	GLuint ProgramId;   /**< A shader program azonosítója */

	lrGLTextureLocation *DiffuseMapLocation;
	lrGLTextureLocation *NormalMapLocation;
	lrGLTextureLocation *SpecularMapLocation;
	lrGLTextureLocation *EmissiveMapLocation;

public:

    /** \brief Az attribute változók layout locationje
     * Az OpenGL 3.3-at használó shaderekben manuálisan adtam meg az attribute változók layout locationjét.
     * Ez az enmueráció tartalmazza a locationöket megadó konstansokat.
     *
     */
    enum lGL3_3LayoutLocation
    {
        LR_GL3_POSITION_LOCATION  = 0,
        LR_GL3_NORMAL_LOCATION    = 1,
        LR_GL3_TANGENT_LOCATION   = 2,
        LR_GL3_BITANGENT_LOCATION = 3,
        LR_GL3_TEXCOORD_LOCATION  = 4,
        LR_GL3_BONE_ID_LOCATION   = 5,
        LR_GL3_WEIGHT_LOCATION    = 6,
    };

    /** \brief Visszaadja a model mátrix uniform locationjét
     *
     * \return virtual GLint - a model matrix locationje.
     *
     */
    virtual GLint GetModelMatrixLocation() override;

    /** \brief Visszaadja a view mátrix uniform locationjét
     *
     * \return virtual GLint - a view matrix locationje.
     *
     */
    virtual GLint GetViewMatrixLocation() override;

    /** \brief Visszaadja a projection mátrix uniform locationjét
     *
     * \return virtual GLint - a projection matrix locationje.
     *
     */
    virtual GLint GetProjectionMatrixLocation() override;

    /** \brief Visszaadja a model view mátrix uniform locationjét
     *
     * \return virtual GLint - a model view matrix locationje.
     *
     */
    virtual GLint GetModelViewMatrixLocation() override;

    /** \brief Visszaadja a model view projection mátrix uniform locationjét
     *
     * \return virtual GLint - a model view projection matrix locationje.
     *
     */
    virtual GLint GetModelViewProjectionMatrixLocation() override;

    /** \brief Visszaadja a normal mátrix uniform locationjét
     *
     * \return virtual GLint - a normal matrix locationje.
     *
     */
    virtual GLint GetNormalMatrixLocation() override;

    /** \brief Elkéri a csontok traszformációinak a uniform locationjét
     *
     * \return virtual GLint - a csontok traszformációinak a uniform locationje
     *
     */
    virtual GLint GetBoneTransformLocation() override;

    /** \brief Visszaadja a diffuse map uniform locationjét
     *
     * \return const lrGLTextureLocation & - a diffuse map locationje.
     *
     */
    virtual const lrGLTextureLocation &GetDiffuseMapLocation() override;

    /** \brief Visszaadja a normal map uniform locationjét
     *
     * \return const lrGLTextureLocation & - a normal map locationje.
     *
     */
    virtual const lrGLTextureLocation &GetNormalMapLocation() override;

    virtual const lrGLTextureLocation &GetSpecularMapLocation() override;
    virtual const lrGLTextureLocation &GetEmissiveMapLocation() override;

    /** \brief Visszaadja a material diffuse propertyjének uniform locationjét
     *
     * \return virtual GLint
     *
     */
    virtual GLint GetMaterialDiffuseLocation() override;

    /** \brief Visszaadja a material specular propertyjének uniform locationjét
     *
     * \return virtual GLint
     *
     */
    virtual GLint GetMaterialSpecularLocation() override;

    /** \brief Visszaadja a material shininessének uniform locationjét
     *
     * \return virtual GLint
     *
     */
    virtual GLint GetMaterialShininessLocation() override;

    /** \brief Elkéri a position attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a position attribute locationje
     *
     */
    virtual GLint GetPositionLocation() override;

    /** \brief Elkéri a normal attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a normal attribute locationje
     *
     */
    virtual GLint GetNormalLocation() override;

    /** \brief Elkéri a tangent attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a tangent attribute locationje
     *
     */
    virtual GLint GetTangentLocation() override;

    /** \brief Elkéri a bitangent attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a bitangent attribute locationje
     *
     */
    virtual GLint GetBitangentLocation() override;

    /** \brief Elkéri a textúra koordináta attribute locationjét
     *
     * \return virtual GLint - a textúra koordináta attribute locationje
     *
     */
    virtual GLint GetTexCoordLocation() override;

    /** \brief Elkéri a bone id attribute locationjét
     *
     * \return virtual GLint - a bone id attribute locationje
     *
     */
    virtual GLint GetBoneIdLocation() override;

    /** \brief Elkéri a bone súlyának attribute locationjét
     *
     * \return virtual GLint - a bone súlyának attribute locationje
     *
     */
    virtual GLint GetWeightLocation() override;

    /** \brief Getter függvény, visszaadja a program ID-t.
     *
     * \return virtual GLuint - a shader program ID-je.
     *
     */
	GLuint GetProgramId() override;

    /** \brief Függvény, ami aktiválja a shadert
     *
     * \return virtual GLint
     *
     */
    void UseProgram() override;

    /** \brief Függvény, ami deaktiválja a shadert
     *
     * \return virtual GLint
     *
     */
    void DisableProgram() override;

    /** \brief Konstruktor
     *
     * \param VsName const std::string&: a vertex shader fájlneve
     * \param FsName const std::string&: a fragment shader fájlneve
     *
     */
    lrGL3_3SkmShader(const std::string &VsName,const std::string &FsName);

    /** \brief Destruktor
     *
     * Felszabadítja a shadereket és a shader programot.
     *
     */
    virtual ~lrGL3_3SkmShader() override;

};

#endif
