#ifndef LR_IMG_H
#define LR_IMG_H

#include "../lInterfaces/lRenderer/liImg.h"

/** \brief Egy a videókártyára feltöltött bittérkép adatait összefogó platformfüggetlen absztrakt class. Arra való, hogy a platformspecifikus részleteket elfedje.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrImg : public liImg
{
protected:
    unsigned int Width;     /**< A bittérkép szélessége */
    unsigned int Height;    /**< A bittérkép hosszúsága */
    bool AlphaBlending;     /**< Alfa csatorna - true, ha a képnek van alfa csatornája, false egyébként. */
public:

    /** \brief Getter függvény, elkéri a kép szélességét.
     *
     * \return unsigned int - a kép szélessége
     *
     */
    virtual unsigned int GetWidth() override
    {
        return Width;
    }

    /** \brief Getter függvény, elkéri a kép magasságát.
     *
     * \return unsigned int - a kép magassága.
     *
     */
    virtual unsigned int GetHeight() override
    {
        return Height;
    }

    /** \brief Getter függvény, megadja, hogy van-e alpha csatorna, vagy nincs.
     *
     * \return bool - az alpha blending értéke.
     *
     */
    virtual bool GetAlphaBlending() override
    {
        return AlphaBlending;
    }

    /** \brief Konstruktor, amely inicializálja a class minden változóját.
     *
     * \param width unsigned int    a kép szélessége.
     * \param height unsigned int   a kép magassága.
     * \param alpha_blending bool    az alpha blending értéke.
     *
     */
    lrImg(unsigned int width,unsigned int height,bool alpha_blending)
        :Width(width),Height(height),AlphaBlending(alpha_blending)
    {

    }

    /** \brief Virtuális destruktor, nem csinál semmit.
     *
     */
    virtual ~lrImg() override
    {

    }
};

#endif // LR_IMG_H
