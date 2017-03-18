#ifndef LSDL2_FRAME_LIMITER
#define LSDL2_FRAME_LIMITER

#include <SDL2/SDL.h>

#include <lApiAdapter/liFrameLimiter.h>

class lSDL2_FrameLimiter : public liFrameLimiter
{
private:
	int MaxFPS;
	int Timer;
	//int FPS_Counter;
public:
	//
	virtual void SetFPS(unsigned int fps) override
	{
		MaxFPS = fps;
	}
	//
	void StartFrameLimiter() override;
	void CheckFrameLimiter() override;
	//
	lSDL2_FrameLimiter();
    virtual ~lSDL2_FrameLimiter() override;
};

#endif
