#ifndef L_UTILS_H
#define L_UTILS_H

#include <fstream>
#include <string>


/** \brief Statikus tömb
 * \tparam class T:             A tárolt objektum típusa
 * \tparam unsigned int SIZE:   A tömb hossza
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int SIZE>
class luStaticArray
{
private:
    T Data[SIZE];
public:

    /** \brief Tömbelem lekérdező operátor
     *
     * \param i unsigned int: az elem indexe
     * \return T: a tömb i-edik eleme
     *
     */
    T &operator[](unsigned int i)
    {
        return Data[i];
    }

    /** \brief Tömbelem lekérdező operátor - konstans tagfüggvény
     *
     * \param i unsigned int: az elem indexe
     * \return const T: a tömb i-edik eleme
     *
     */
    const T &operator[](unsigned int i) const
    {
        return Data[i];
    }

    /** \brief Default konstruktor
     *
     */
    luStaticArray()
    {

    }

    /** \brief Copy konstruktor
     *
     * \param Array const lmStaticArray<T,SIZE> &: az átmásolandó tömb.
     *
     */
    luStaticArray(const luStaticArray<T,SIZE> &Array)
    {
        for(int i=0;i < SIZE;i++)
        {
            Data[i] = Array.Data[i];
        }
    }
};

int GetHexDigit(char Digit);
void LoadQuotedString(std::istream &in,std::string &str,bool step_back = false);

#endif // L_UTILS_H
