#include <iostream>

#include "lrKeyframeAnimation.h"

using namespace std;

int lrKeyframeAnimation::GetFrameRate()
{
    return FrameRate;
}

double lrKeyframeAnimation::GetFrameDuration()
{
    return FrameDuration;
}

double lrKeyframeAnimation::GetAnimDuration()
{
    return Frames.size() * FrameDuration;
}

void lrKeyframeAnimation::GetAnimation(double t,liSkeleton *Target)
{
    unsigned int cFrameIndex = floor(t/(double)FrameDuration);
    if(cFrameIndex >= Frames.size())
    {
        cFrameIndex = cFrameIndex % Frames.size();
    }

    unsigned int nFrameIndex = ceil(t/(double)FrameDuration);
    if(nFrameIndex >= Frames.size())
    {
        nFrameIndex = nFrameIndex % Frames.size();
    }

    double Interp = (t - (cFrameIndex*FrameDuration))/FrameDuration;

    //std::cout << Interp << std::endl;
    //std::cout << cFrameIndex << ";" << nFrameIndex << std::endl;

    lrSkeleton *CurrentFrame = &Frames[cFrameIndex];
    lrSkeleton *NextFrame    = &Frames[nFrameIndex];

    lrLinearInterpolation(CurrentFrame,NextFrame,Interp,Target);
}

void lrKeyframeAnimation::SetFrameRate(int frameRate)
{
    FrameRate = frameRate;

    FrameDuration = 1.0 / (double)FrameRate;
    //AnimDuration = Frames.size() * FrameDuration;
}

void lrKeyframeAnimation::ResizeFrames(unsigned int i)
{
    Frames.resize(i);
    //AnimDuration = Frames.size() * FrameDuration;
}
