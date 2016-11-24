#ifndef LR_GL_STM_SHADER
#define LR_GL_STM_SHADER

#include "lrGLRendererShader.h"

/** \brief Egy static mesh OpenGL shaderének általános funkcióit biztosító osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLStmShader : public lrGLRendererShader
{
public:

    /** \brief Elkéri a position attribute locationjét
     *
     * \return virtual GLint - a position attribute locationje
     *
     */
    virtual GLint GetPositionLocation() = 0;

    /** \brief Elkéri a normal attribute locationjét
     *
     * \return virtual GLint - a normal attribute locationje
     *
     */
    virtual GLint GetNormalLocation() = 0;

    /** \brief Elkéri a tangent attribute locationjét
     *
     * \return virtual GLint - a tangent attribute locationje
     *
     */
    virtual GLint GetTangentLocation() = 0;

    /** \brief Elkéri a bitangent attribute locationjét
     *
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

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
	lrGLStmShader()
	{

	}

    /** \brief Virtuális destruktor, nem csinál semmit.
     *
     */
	virtual ~lrGLStmShader() override
	{

	}
};

#endif
