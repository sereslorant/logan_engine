#ifndef LR_GL_STATIC_MESH_H
#define LR_GL_STATIC_MESH_H

#include "lrGLShader/lrGLStmShader.h"

#include "../lrStaticMesh.h"

#include "../../lInterfaces/lResourceManager/liGPUFactory.h"

/** \brief OpenGL-t használó static mesh
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLStaticMesh : public lrStaticMesh
{
public:
    /** \brief Ez az adatstruktúra tartalmazza az összes buffer object indexét, ami a static mesh kirajzolásához kell.
     */
    struct GLVertexData
    {
        unsigned int BufferLength; /**< A buffer objectek hossza */

        GLuint VertexBuffer;    /**< A csúcspontokat tároló buffer object azonosítója */
        GLuint NormalBuffer;    /**< A normálvektorokat tároló buffer object azonosítója */
        GLuint TangentBuffer;   /**< A tangent vektorokat tároló buffer object azonosítója */
        GLuint BitangentBuffer; /**< A bitangent vektorokat tároló buffer object azonosítója */
        GLuint TexCoordBuffer;  /**< A textúrakoordinátákat tároló buffer object azonosítója */
    };

protected:
    GLVertexData VertexData; /**< A mesh kirajzolásához szükséges buffer objectek */

public:

    /** \brief OpenGL-t használó material group.
     *
     * Tartalmazza az index buffert, ami megadja a háromszögeket, amik hozzá tartoznak. Az index buffer
     * egész számhármasokat tartalmaz, amelyek a buffer objectekben a háromszög csúcsaira mutató indexek.
     * Az őt tartalmazó static mesh tartalmazza a vertex adatokat, amik a kirajzoláshoz kellenek, de a
     * benne lévő vertex array objecthez hozzá vannak bindolva.
     *
     */
    class lrGLMaterialGroup : public lrMaterialGroup
    {
    protected:
        unsigned int BufferLength;  /**< Az index buffer hossza */
        GLuint IndexBuffer;         /**< Az index buffer azonosítója */

        GLuint VertexArrayObject;   /**< A vertex array object, amihez a vertex data bindolva van. */

    public:

        /** \brief Getter függvény, visszaadja az index buffer hosszát.
         *
         * \return unsigned int - az index buffer hossza
         *
         */
        unsigned int GetBufferLength() const;

        /** \brief Getter függvény, visszaadja az index buffer azonosítóját.
         *
         * \return GLuint - az index buffer azonosítója
         *
         */
        GLuint GetIndexBuffer() const;

        /** \brief Getter függvény, visszaadja a vertex array object azonosítóját.
         *
         * \return GLuint - a vertex array object azonosítója
         *
         */
        GLuint GetVertexArrayObject() const;

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
         * \param Shader lrGLStmShader*:            Az OpenGL renderer static mesh shaderje, amitől az attribute locationöket elkérjük.
         * \param VertexData GLVertexData*:         A material groupot tartalmazó static mesh buffer objectjei
         * \param Src lrmStaticMesh::lrmMtlGroup*:  A resource manager által betöltött adattömbök
         *
         */
        lrGLMaterialGroup(lrGLStmShader *Shader,GLVertexData *VertexData,const liStaticMeshResource::liMtlGroup *Src);

        /** \brief Destruktor
         *
         * Törli az index buffert és a vertex array objectet
         *
         */
        ~lrGLMaterialGroup();
    };

    /** \brief Getter függvény, amely visszaadja a buffer objectek hosszát.
     *
     * \return unsigned int - a buffer objectek hossza
     *
     */
    unsigned int GetBufferLength() const;

    /** \brief Getter függvény, visszaadja a vertex buffer azonosítóját.
     *
     * \return GLuint - a vertex buffer azonosítója.
     *
     */
    GLuint GetVertexBuffer() const;

    /** \brief Getter függvény, visszaadja a normal buffer azonosítóját.
     *
     * \return GLuint - a normal buffer azonosítója.
     *
     */
    GLuint GetNormalBuffer() const;

    /** \brief Getter függvény, visszaadja a tangent buffer azonosítóját.
     *
     * \return GLuint - a tangent buffer azonosítója.
     *
     */
    GLuint GetTangentBuffer() const;

    /** \brief Getter függvény, visszaadja a bitangent buffer azonosítóját.
     *
     * \return GLuint - a bitangent buffer azonosítója.
     *
     */
    GLuint GetBitangentBuffer() const;

    /** \brief Getter függvény, visszaadja a textúrakoordináta buffer azonosítóját.
     *
     * \return GLuint - a textúrakoordináta buffer azonosítója.
     *
     */
    GLuint GetTexCoordBuffer() const;

    /** \brief Konstruktor
     *
     * \param Shader lrGLStmShader*:    Az OpenGL renderer static mesh shaderje, amitől az attribute locationöket elkérjük.
     * \param Src lrmStaticMesh*:       A resource manager által betöltött adattömb
     *
     */
    lrGLStaticMesh(lrGLStmShader *Shader,const liStaticMeshResource *Src);

    /** \brief Destruktor
     *
     * Felszabadítja az összes buffer objectet.
     *
     */
    ~lrGLStaticMesh();
};

#endif
