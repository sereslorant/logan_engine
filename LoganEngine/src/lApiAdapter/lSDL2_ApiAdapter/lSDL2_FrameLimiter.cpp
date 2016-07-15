
#include "lSDL2_FrameLimiter.h"

lSDL2_FrameLimiter::lSDL2_FrameLimiter(): MaxFPS(60)
{
    Timer=0;
    //FPS_Counter=0;
}

lSDL2_FrameLimiter::~lSDL2_FrameLimiter()
{

}

void lSDL2_FrameLimiter::StartFrameLimiter()
{
    Timer = SDL_GetTicks();
}

void lSDL2_FrameLimiter::CheckFrameLimiter()
{
    int Timer_End = (SDL_GetTicks()-Timer);
    if (Timer_End < 1000/MaxFPS)
    {
        SDL_Delay((1000/MaxFPS) - Timer_End);
    }
}
