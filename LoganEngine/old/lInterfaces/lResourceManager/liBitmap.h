#ifndef LI_BITMAP_H
#define LI_BITMAP_H

#include <inttypes.h>

class liBitmap
{
public:
    enum LI_PIXEL_FORMAT
    {
        PF_R8_G8_B8 = -3,
        PF_B8_G8_R8 = -2,
        PF_LUT = -1,
        //16
        //5_6_5
        PF_B5_G6_R5 = 0,
        PF_R5_G6_B5,
        //4_4_4_4
        PF_A4_B4_G4_R4,
        PF_A4_R4_G4_B4,
        //1_5_5_5
        PF_B5_G5_R5_A1,
        PF_B5_G5_R5_X1,
        PF_R5_G5_B5_A1,
        PF_R5_G5_B5_X1,
        //32
        //2_10_10_10
        PF_A2_B10_G10_R10,
        PF_A2_R10_G10_B10,
        //8_8_8_8
        PF_A8_B8_G8_R8,
        PF_X8_B8_G8_R8,
        PF_A8_R8_G8_B8,
        PF_X8_R8_G8_B8,
    };

    class liSampler
    {
	public:

		virtual float GetRed(int i,int j) = 0;
		virtual float GetGreen(int i,int j) = 0;
		virtual float GetBlue(int i,int j) = 0;
		virtual float GetAlpha(int i,int j) = 0;

		liSampler()
		{}

		virtual ~liSampler()
		{}
    };

    virtual int GetError() = 0;
    virtual int32_t GetWidth() = 0;
    virtual int32_t GetHeight() = 0;
    virtual LI_PIXEL_FORMAT GetPixelFormat() = 0;

    virtual char *GetPixelArray() = 0;
    virtual liSampler *GetSampler() = 0;

	liBitmap(){}
	virtual ~liBitmap(){}
};

#endif // LI_BITMAP_H
