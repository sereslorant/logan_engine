#ifndef L_INTERFACE
#define L_INTERFACE

#include "lInput.h"
#include "liFrameLimiter.h"
#include "../lInterfaces/lRenderer/liRenderer.h"
#include "../lInterfaces/lResourceManager/liResourceManager.h"
#include "../lGooey/lGooRenderer.h"

/** \brief Ez a class felelős az összes platformspecifikus rendszerhívás (ablakkészítés, input, output, stb.) összefogásáért.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liApiAdapter
{
public:

    /** \brief Getter függvény, visszaadja az inputkezelőre mutató pointert.
     *
     * \return lInput *: Az inputkezelőre mutató pointer
     *
     */
    virtual lInput *GetInput() = 0;

    /** \brief Getter függvény, visszaadja a frame limiterre mutató pointert.
     *
     * \return lFrameLimiter *: A frame limiterre mutató pointer
     *
     */
    virtual liFrameLimiter *GetFrameLimiter() = 0;

    /** \brief Getter függvény, visszaadja a rendererre mutató pointert.
     *
     * \return lRenderer *: A rendererre mutató pointer
     *
     */
    virtual liRenderer *GetRenderer() = 0;

    virtual liGuiRenderer *GetGuiRenderer() = 0;

    /** \brief Getter függvény, visszaadja a resource managerre mutató pointert.
     *
     * \return lrm_RM *: A resource managerre mutató pointer
     *
     */
    virtual liResourceManager *GetResourceManager() = 0;

    /** \brief Doublebuffer esetén swapolja a buffert.
     *
     * \return void
     *
     */
    virtual void SwapBuffers() = 0;

    /** \brief Konstruktor, minden pointert nullptr-re állít.
     *
     */
    liApiAdapter(){}

    /** \brief Destruktor, minden dinamikusan allokált objektumot felszabadít.
     *
     */
    virtual ~liApiAdapter(){}
};

#endif
