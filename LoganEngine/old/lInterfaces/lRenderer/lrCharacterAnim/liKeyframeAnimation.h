#ifndef LI_KEYFRAME_ANIMATION_H
#define LI_KEYFRAME_ANIMATION_H

#include "liSkeleton.h"

/** \brief Egy keyframe animationt megvalósító osztály.
 *
 * \author Seres Lóránt Gábor
 *
 */
class liKeyframeAnimation
{
public:

	virtual unsigned int Size() = 0;

    /** \brief Getter függvény, ami visszaadja a frame rate-et.
     *
     * \return int - a frame rate
     *
     */
    virtual int GetFrameRate() = 0;

    /** \brief Getter függvény, ami visszaadja a frame-ek időtartamát.
     *
     * \return float - a frame rate
     *
     */
    virtual double GetFrameDuration() = 0;

    /** \brief Getter függvény, ami visszaadja az animáció időtartamát.
     *
     * \return float - az animáció időtartama
     *
     */
    virtual double GetAnimDuration() = 0;

    /** \brief Elkéri a t-edik időpillanatbeli frame-et.
     *
     * \param t float:              Az iőpillanat, ami meghatározza, hogy melyik frame-et keressük.
     * \param Target lrSkeleton &:  A skeleton, amibe elmentjük a t időpillanatbeli frame-et.
     * \return void
     *
     */
    virtual void GetAnimation(double t,liSkeleton *Target) = 0;

    /** \brief Setter függvény, amivel be lehet állítani a frame rate-et.
     *
     * \param frameRate int: az új frame rate
     * \return void
     *
     */
    virtual void SetFrameRate(int frameRate) = 0;

    /** \brief Setter függvény, amivel át lehet méretezni a frame-eket tároló tömböt.
     *
     * \param i unsigned int: a frame-eket tartalmazó tömb új mérete.
     * \return void
     *
     */
    virtual void ResizeFrames(unsigned int i) = 0;

    /** \brief Operátor, amelyik elkéri az i-edik frame-et.
     *
     * \param i unsigned int: az elkérendő frame indexe
     * \return lrSkeleton & - az i-edik frame
     *
     */
    virtual liSkeleton *operator[](unsigned int i) = 0;

    /** \brief Operátor, amelyik elkéri az i-edik frame-et. Konstans tagfüggvény
     *
     * \param i unsigned int: az elkérendő frame indexe
     * \return const lrSkeleton & - az i-edik frame
     *
     */
    virtual const liSkeleton *operator[](unsigned int i) const = 0;

    liKeyframeAnimation(){}
    virtual ~liKeyframeAnimation()
    {}
};

#endif // LI_KEYFRAME_ANIMATION_H
