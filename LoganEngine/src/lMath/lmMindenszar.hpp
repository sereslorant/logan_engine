#ifndef LM_MINDENSZAR_H
#define LM_MINDENSZAR_H


template <class T>
void lmSwap(T &var1,T &var2)
{
	T Tmp = var1;
	var1 = var2;
	var2 = Tmp;
}

/** \brief Signum függvény
 *
 * \param val T: az érték, aminek az előjelére kíváncsiak vagyunk.
 * \return -1, ha negtív, 1, ha pozitív, 0, ha 0.
 *
 * \author Random ember stackoverflow-n.
 *
 */
template <class T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


/** \brief A paraméterként kapott x értéket beleclampeli a Min - Max által körülhatárolt tartományba.
 *
 * \param Min lmScalar: Az intervallum minimuma.
 * \param Max lmScalar: Az intervallum maximuma.
 * \param x lmScalar:   A clampelendő érték.
 * \return lmScalar - a clampelt érték.
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T>
T lmClamp(T Min,T Max,T x)
{
    if(x < Min)
    {
        return Min;
    }
    if(x > Max)
    {
        return Max;
    }

    return x;
}

#endif // LM_MINDENSZAR_H
