#ifndef LSDL2_FRAME_LIMITER
#define LSDL2_FRAME_LIMITER

#include "../liFrameLimiter.h"

#include <SDL2/SDL.h>

class lSDL2FrameLimiter : public liFrameLimiter
{
private:
	const int MaxFPS;
	int Timer;
	//int FPS_Counter;
public:
	lSDL2FrameLimiter(int maxFPS);
    virtual ~lSDL2FrameLimiter() override;

	void StartFrameLimiter() override;
	void CheckFrameLimiter() override;
};

#endif
