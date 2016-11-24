#ifndef LI_IMG_H
#define LI_IMG_H

class liImg
{
public:

    /** \brief Függvény, amely megadja, hogy a kép sikeresen fel lett-e töltve a videókártyára.
     *
     * \return bool - ha történt hiba a kép feltöltésekor, akkor false, egyéb esetben true.
     *
     */
    virtual bool CheckError() = 0;

    /** \brief Getter függvény, elkéri a kép szélességét.
     *
     * \return unsigned int - a kép szélessége
     *
     */
    virtual unsigned int GetWidth() = 0;

    /** \brief Getter függvény, elkéri a kép magasságát.
     *
     * \return unsigned int - a kép magassága.
     *
     */
    virtual unsigned int GetHeight() = 0;

    /** \brief Getter függvény, megadja, hogy van-e alpha csatorna, vagy nincs.
     *
     * \return bool - az alpha blending értéke.
     *
     */
    virtual bool GetAlphaBlending() = 0;

    liImg()
    {

    }

    virtual ~liImg()
    {

    }
};

#endif // LI_IMG_H
