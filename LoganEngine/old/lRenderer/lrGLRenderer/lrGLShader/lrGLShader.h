#ifndef LR_GL_SHADER
#define LR_GL_SHADER

#include <fstream>
#include <string>

#include "../lGLExt.h"

using namespace glExt;

bool lLoadText(const std::string &filename,std::string &destination);

/** \brief Egy OpenGL shader általános funkcióit biztosító osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLShader
{
public:

    /** \brief Függvény, amely a paraméterben kapott shader kódot lefordítja, és létrehoz egy shadert.
     *
     * \param Src const std::string&    A shader forráskódja
     * \param ShaderType GLenum         A shader típusa.
     * \return GLuint - a shader azonosítója
     *
     */
    static GLuint LoadShader(const std::string &Src,GLenum ShaderType);

    /** \brief Getter függvény, visszaadja a program ID-t.
     *
     * \return virtual GLuint - a shader program ID-je.
     *
     */
	virtual GLuint GetProgramId() = 0;

    /** \brief Függvény, ami aktiválja a shadert
     *
     * \return virtual GLint
     *
     */
	virtual void UseProgram() = 0;

    /** \brief Függvény, ami deaktiválja a shadert
     *
     * \return virtual GLint
     *
     */
	virtual void DisableProgram() = 0;

    /** \brief Konstruktor, nem csinál semmit
     *
     *
     */
	lrGLShader();

    /** \brief Virtuális destruktor, nem csinál semmit
     *
     *
     */
	virtual ~lrGLShader();
};

#endif
