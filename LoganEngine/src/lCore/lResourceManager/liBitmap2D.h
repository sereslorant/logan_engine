#ifndef LI_BITMAP_2D_H
#define LI_BITMAP_2D_H

/*
 * Bitképekhez kötkető interfész
 */

#include <inttypes.h>

class liBitmap2D
{
public:
	//
    class liSampler
    {
	public:
    	//
		virtual float GetRed(int i,int j) = 0;
		virtual float GetGreen(int i,int j) = 0;
		virtual float GetBlue(int i,int j) = 0;
		virtual float GetAlpha(int i,int j) = 0;
		//
		liSampler(){}
		virtual ~liSampler(){}
		/*
		 * End of class
		 */
    };
    //
    virtual int32_t GetWidth() const = 0;
    virtual int32_t GetHeight() const = 0;
    //
    virtual const char *GetPixelArray() const = 0;
    //virtual liSampler &GetSampler() = 0;
    //
	liBitmap2D(){}
	virtual ~liBitmap2D(){}
	/*
	 * End of class
	 */
};

#endif // LI_BITMAP_2D_H
