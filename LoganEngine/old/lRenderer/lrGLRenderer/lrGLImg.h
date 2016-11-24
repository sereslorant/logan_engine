#ifndef LRM_GL_IMG
#define LRM_GL_IMG

#include "lGLExt.h"

using namespace glExt;

#include "../lrImg.h"

#include <string>

/** \brief OpenGL renderer által feltöltött kép elérésére használt osztály.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrGLImg : public lrImg
{
protected:
    GLuint Id; /**< Az OpenGL által kiosztott azonosító */
public:

    /** \brief Függvény, amely megadja, hogy a kép sikeresen fel lett-e töltve a videókártyára.
     *
     * Ha a kép létrehozásakor hiba lép fel, akkor az OpenGL a 0 azonosítót adja neki, ezzel végezzük a hibaellenőrzést.
     *
     * \return bool - ha történt hiba a kép feltöltésekor, akkor false, egyéb esetben true.
     *
     */
    bool CheckError();

    /** \brief Getter függvény, amivel el lehet kérni az OpenGL által kiosztott azonosítót
     *
     * \return GLuint - az OpenGL által kiosztott azonosító
     *
     */
    GLuint GetId();

    void SetWidth(unsigned int width)
    {
    	Width = width;
    }

    void SetHeight(unsigned int height)
    {
    	Height = height;
    }

    /** \brief Konstruktor, amely inicializálja a class minden változóját.
     *
     * \param width unsigned int    a kép szélessége.
     * \param height unsigned int   a kép magassága.
     * \param alphaBlending bool    az alpha blending értéke.
     * \param id GLuint             az OpenGL által kiosztott index
     *
     */
    lrGLImg(unsigned int width,unsigned int height,bool alphaBlending,GLuint id);

    /** \brief Destruktor
     *
     * Felszabadítja az Id-hez tartozó textúrát a glDeleteTextures()-szel.
     *
     */
    virtual ~lrGLImg() override;
};

#endif
