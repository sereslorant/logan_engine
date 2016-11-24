#ifndef LR_KEYFRAME_ANIMATION_H
#define LR_KEYFRAME_ANIMATION_H

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liKeyframeAnimation.h"

#include "lrSkeleton.h"
#include <vector>

/** \brief Egy keyframe animationt megvalósító osztály.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrKeyframeAnimation : public liKeyframeAnimation
{
private:
    int FrameRate;			/**< Időegység alatt végbemenő frame-ek száma. */
    double FrameDuration;	/**< Egy frame időtartama (= 1.0/FrameRate) */
    //double AnimDuration;	/**< Az animáció időtartama  (= Frames.size()*FrameDuration) */

    std::vector<lrSkeleton> Frames; /**< Az egyes frame-eket tartalmazó tömb. */

public:

	virtual unsigned int Size() override
	{
		return Frames.size();
	}

    /** \brief Getter függvény, ami visszaadja a frame rate-et.
     *
     * \return int - a frame rate
     *
     */
    virtual int GetFrameRate() override;

    /** \brief Getter függvény, ami visszaadja a frame-ek időtartamát.
     *
     * \return float - a frame rate
     *
     */
    virtual double GetFrameDuration() override;

    /** \brief Getter függvény, ami visszaadja az animáció időtartamát.
     *
     * \return float - az animáció időtartama
     *
     */
    virtual double GetAnimDuration() override;

    /** \brief Elkéri a t-edik időpillanatbeli frame-et.
     *
     * \param t float:              Az iőpillanat, ami meghatározza, hogy melyik frame-et keressük.
     * \param Target lrSkeleton &:  A skeleton, amibe elmentjük a t időpillanatbeli frame-et.
     * \return void
     *
     */
    virtual void GetAnimation(double t,liSkeleton *Target) override;

    /** \brief Setter függvény, amivel be lehet állítani a frame rate-et.
     *
     * \param frameRate int: az új frame rate
     * \return void
     *
     */
    virtual void SetFrameRate(int frameRate) override;

    /** \brief Setter függvény, amivel át lehet méretezni a frame-eket tároló tömböt.
     *
     * \param i unsigned int: a frame-eket tartalmazó tömb új mérete.
     * \return void
     *
     */
    virtual void ResizeFrames(unsigned int i) override;

    /** \brief Operátor, amelyik elkéri az i-edik frame-et.
     *
     * \param i unsigned int: az elkérendő frame indexe
     * \return lrSkeleton & - az i-edik frame
     *
     */
    virtual liSkeleton *operator[](unsigned int i) override
    {
        return &Frames[i];
    }

    /** \brief Operátor, amelyik elkéri az i-edik frame-et. Konstans tagfüggvény
     *
     * \param i unsigned int: az elkérendő frame indexe
     * \return const lrSkeleton & - az i-edik frame
     *
     */
    virtual const liSkeleton *operator[](unsigned int i) const override
    {
        return &Frames[i];
    }

    lrKeyframeAnimation(unsigned int frame_rate,unsigned int num_frames,unsigned int num_bones)
		:FrameRate(frame_rate),FrameDuration(1.0/frame_rate),Frames(num_frames)
	{
		for(lrSkeleton &I : Frames)
		{
			I.Resize(num_bones);
		}
	}
    virtual ~lrKeyframeAnimation() override
    {}
};

#endif // LR_KEYFRAME_ANIMATION_H
