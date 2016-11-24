#ifndef LR_GL_SKM_SHADER
#define LR_GL_SKM_SHADER

#include "lrGLRendererShader.h"

/** \brief Egy skeletal mesh OpenGL shaderének általános funkcióit biztosító osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLSkmShader : public lrGLRendererShader
{
public:

    /** \brief Elkéri a position attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a position attribute locationje
     *
     */
    virtual GLint GetPositionLocation() = 0;

    /** \brief Elkéri a normal attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a normal attribute locationje
     *
     */
    virtual GLint GetNormalLocation() = 0;

    /** \brief Elkéri a tangent attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a tangent attribute locationje
     *
     */
    virtual GLint GetTangentLocation() = 0;

    /** \brief Elkéri a bitangent attribute locationjét
     *
     * \param i unsigned int: deprecated
     * \return virtual GLint - a bitangent attribute locationje
     *
     */
    virtual GLint GetBitangentLocation() = 0;

    /** \brief Elkéri a textúra koordináta attribute locationjét
     *
     * \return virtual GLint - a textúra koordináta attribute locationje
     *
     */
    virtual GLint GetTexCoordLocation() = 0;

    /** \brief Elkéri a bone id attribute locationjét
     *
     * \return virtual GLint - a bone id attribute locationje
     *
     */
    virtual GLint GetBoneIdLocation() = 0;

    /** \brief Elkéri a bone súlyának attribute locationjét
     *
     * \return virtual GLint - a bone súlyának attribute locationje
     *
     */
    virtual GLint GetWeightLocation() = 0;

    /** \brief Elkéri a csontok traszformációinak a uniform locationjét
     *
     * \return virtual GLint - a csontok traszformációinak a uniform locationje
     *
     */
    virtual GLint GetBoneTransformLocation() = 0;

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
	lrGLSkmShader()
	{

	}

    /** \brief Virtuális destruktor, nem csinál semmit.
     *
     */
	virtual ~lrGLSkmShader() override
	{

	}
};

#endif
