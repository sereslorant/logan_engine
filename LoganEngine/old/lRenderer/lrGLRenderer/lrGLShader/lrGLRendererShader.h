#ifndef LR_GL_RENDERER_SHADER_H
#define LR_GL_RENDERER_SHADER_H

#include "lrGLShader.h"

/** \brief Egy OpenGL shader általános funkcióit biztosító osztály
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLRendererShader : public lrGLShader
{
public:

	class lrGLTextureLocation
	{
	private:
		GLint TextureLocation;
		GLint HasTextureLocation;

	public:

		GLint GetTextureLocation() const
		{
			return TextureLocation;
		}

		GLint GetHasTextureLocation() const
		{
			return HasTextureLocation;
		}

		lrGLTextureLocation(GLint texture_location,GLint has_texture_location)
			:TextureLocation(texture_location),HasTextureLocation(has_texture_location)
		{
			//Üres függvénytörzs
		}

		~lrGLTextureLocation()
		{}
	};

    /** \brief Visszaadja a model mátrix uniform locationjét
     *
     * \return virtual GLint - a model matrix locationje.
     *
     */
    virtual GLint GetModelMatrixLocation() = 0;

    /** \brief Visszaadja a view mátrix uniform locationjét
     *
     * \return virtual GLint - a view matrix locationje.
     *
     */
    virtual GLint GetViewMatrixLocation() = 0;

    /** \brief Visszaadja a projection mátrix uniform locationjét
     *
     * \return virtual GLint - a projection matrix locationje.
     *
     */
    virtual GLint GetProjectionMatrixLocation() = 0;

    /** \brief Visszaadja a model view mátrix uniform locationjét
     *
     * \return virtual GLint - a model view matrix locationje.
     *
     */
    virtual GLint GetModelViewMatrixLocation() = 0;

    /** \brief Visszaadja a model view projection mátrix uniform locationjét
     *
     * \return virtual GLint - a model view projection matrix locationje.
     *
     */
    virtual GLint GetModelViewProjectionMatrixLocation() = 0;

    /** \brief Visszaadja a normal mátrix uniform locationjét
     *
     * \return virtual GLint - a normal matrix locationje.
     *
     */
    virtual GLint GetNormalMatrixLocation() = 0;

    /** \brief Visszaadja a diffuse map uniform locationjét
     *
     * \return virtual const lrGLTextureLocation & - a diffuse map locationje.
     *
     */
    virtual const lrGLTextureLocation &GetDiffuseMapLocation() = 0;

    /** \brief Visszaadja a normal map uniform locationjét
     *
     * \return virtual const lrGLTextureLocation & - a normal map locationje.
     *
     */
    virtual const lrGLTextureLocation &GetNormalMapLocation() = 0;

    /** \brief Visszaadja a normal map uniform locationjét
     *
     * \return virtual const lrGLTextureLocation & - a normal map locationje.
     *
     */
    virtual const lrGLTextureLocation &GetSpecularMapLocation() = 0;

    /** \brief Visszaadja a normal map uniform locationjét
     *
     * \return virtual const lrGLTextureLocation & - a normal map locationje.
     *
     */
    virtual const lrGLTextureLocation &GetEmissiveMapLocation() = 0;

    /** \brief Visszaadja a material diffuse propertyjének uniform locationjét
     *
     * \return virtual GLint
     *
     */
    virtual GLint GetMaterialDiffuseLocation() = 0;

    /** \brief Visszaadja a material specular propertyjének uniform locationjét
     *
     * \return virtual GLint
     *
     */
    virtual GLint GetMaterialSpecularLocation() = 0;

    /** \brief Visszaadja a material shininessének uniform locationjét
     *
     * \return virtual GLint
     *
     */
    virtual GLint GetMaterialShininessLocation() = 0;

    /** \brief Konstruktor, nem csinál semmit
     *
     *
     */
	lrGLRendererShader()
	{

	}

    /** \brief Virtuális destruktor, nem csinál semmit
     *
     *
     */
	virtual ~lrGLRendererShader() override
	{

	}
};

#endif // LR_GL_RENDERER_SHADER_H
