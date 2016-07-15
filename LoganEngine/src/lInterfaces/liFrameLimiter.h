#ifndef L_FRAME_LIMITER
#define L_FRAME_LIMITER

/** \brief A frame limiter absztrakciós rétege. A frame limiter dolga, hogy a main loop végrehajtásának időtartamát egy
 * meghatározott fix ciklusidőre nyújtsa. Ezt úgy csinálja meg, hogy minden ciklus elején megnézi, hogy a program
 * indítása óta mennyi idő telt el, majd ugyanezt megteszi a ciklus végén is, és a kettőnek a különbsége lesz a ciklus
 * végrehajtásának az ideje. Ha ez kisebb egy küszöbértéknél, akkor beiktat egy késleltetést, ami a megadott
 * küszöbértékre nyújtja a ciklusidőt.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liFrameLimiter
{
public:

	virtual void SetFPS(unsigned int fps) = 0;

    /** \brief Elindítja a számlálót. A main loop elején kell lehívni.
     *
     * \return void
     *
     */
	virtual void StartFrameLimiter() = 0;

    /** \brief Megállítja a számlálót, megnézi, hogy a ciklus kezdete óta mennyi idő telt el, és ha az egy frame-re
     * megszabott idő alatt van, akkor beiktat egy késleltetést.
     *
     * \return void
     *
     */
	virtual void CheckFrameLimiter() = 0;

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
	liFrameLimiter(){}

	/** \brief Virtuális destruktor, nem csinál semmit.
     *
     */
    virtual ~liFrameLimiter(){}
};

#endif
