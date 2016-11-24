#ifndef LR_GL_SKELETAL_MESH
#define LR_GL_SKELETAL_MESH

#include "lrGLShader/lrGLSkmShader.h"

#include "../lrSkeletalMesh.h"

#include "../../lInterfaces/lResourceManager/liGPUFactory.h"

#include <vector>

/** \brief OpenGL-t használó skeletal mesh
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLSkeletalMesh : public lrSkeletalMesh
{

protected:
    unsigned int BufferLength;  /**< A buffer objectek hossza */

    GLuint VertexBuffer;    /**< A csúcspontokat tároló buffer object azonosítója */
    GLuint NormalBuffer;    /**< A normálvektorokat tároló buffer object azonosítója */
    GLuint TangentBuffer;   /**< A tangent vektorokat tároló buffer object azonosítója */
    GLuint BitangentBuffer; /**< A bitangent vektorokat tároló buffer object azonosítója */
    GLuint TexCoordBuffer;  /**< A textúrakoordinátákat tároló buffer object azonosítója */
    GLuint BoneIdBuffer;    /**< A csontok indexét tároló buffer object azonosítója */
    GLuint WeightBuffer;    /**< A csontok súlyát tároló buffer object azonosítója */

    unsigned int IndexBufferLength; /**< Az index buffer hossza */
    GLuint IndexBuffer;             /**< Az index buffer azonosítója */

    GLuint VertexArrayObject;       /**< A vertex array object, amihez a vertex data bindolva van. */

public:

    /** \brief Getter függvény, amely visszaadja a buffer objectek hosszát.
     *
     * \return unsigned int - a buffer objectek hossza
     *
     */
    unsigned int GetBufferLength(){return BufferLength;}

    /** \brief Getter függvény, visszaadja az index buffer hosszát.
     *
     * \return unsigned int - az index buffer hossza
     *
     */
    GLuint GetIndexBufferLength(){return IndexBufferLength;}

    /** \brief Kirajzolás előtt ezzel a függvénnyel kell bindolni a vertex array objectet
     *
     * \return void
     *
     */
    virtual void Enable() override;

    /** \brief Kirajzolás után ezzel deaktiváljuk a vertex array objectet.
     *
     * \return virtual void
     *
     */
    virtual void Disable() override;

    /** \brief Konstruktor
     *
     * \param Shader lrGLSkmShader*:    Az OpenGL renderer static mesh shaderje, amitől az attribute locationöket elkérjük.
     * \param Src lrmSkeletalMesh*:     A resource manager által betöltött adattömbök
     *
     */
    lrGLSkeletalMesh(lrGLSkmShader *Shader,liSkeletalMeshResource *Src);

    /** \brief Virtuális destruktor
     *
     * Felszabadítja a buffer objecteket, az index buffert és a vertex array objecteket.
     *
     */
    virtual ~lrGLSkeletalMesh();
};

#endif
