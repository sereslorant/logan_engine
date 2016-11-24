
#include "lSDL2Input.h"

void lSDL2Input::CheckInput()
{
    SDL_Event event;
    //lMouseState &MouseState = Input.GetMouseState();

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            SetQuit(true);
        }

        if(event.type == SDL_MOUSEMOTION)
        {
            int X = event.motion.x;
            int Y = event.motion.y;

            SetMX(X);
            SetMY(Y);
        }

        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {SetLmb(true);}
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {SetMmb(true);}
            if(event.button.button == SDL_BUTTON_RIGHT)
            {SetRmb(true);}
        }
        if(event.type == SDL_MOUSEBUTTONUP)
        {
            if(event.button.button == SDL_BUTTON_LEFT)
            {SetLmb(false);}
            if(event.button.button == SDL_BUTTON_MIDDLE)
            {SetMmb(false);}
            if(event.button.button == SDL_BUTTON_RIGHT)
            {SetRmb(false);}
        }

        if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {SetEscape(true);}


            if(event.key.keysym.sym == SDLK_RETURN)
            {SetEnter(true);}

            if(event.key.keysym.sym == SDLK_LSHIFT)
            {SetLShift(true);}

            if(event.key.keysym.sym == SDLK_RSHIFT)
            {SetRShift(true);}

            if(event.key.keysym.sym == SDLK_SPACE)
            {SetSpace(true);}


            if(event.key.keysym.sym == SDLK_UP)
            {SetUp(true);}

            if(event.key.keysym.sym == SDLK_DOWN)
            {SetDown(true);}

            if(event.key.keysym.sym == SDLK_LEFT)
            {SetLeft(true);}

            if(event.key.keysym.sym == SDLK_RIGHT)
            {SetRight(true);}


            if(event.key.keysym.sym == SDLK_w)
            {SetW(true);}

            if(event.key.keysym.sym == SDLK_a)
            {SetA(true);}

            if(event.key.keysym.sym == SDLK_s)
            {SetS(true);}

            if(event.key.keysym.sym == SDLK_d)
            {SetD(true);}

            if(event.key.keysym.sym == SDLK_q)
            {SetQ(true);}

            if(event.key.keysym.sym == SDLK_e)
            {SetE(true);}
        }

        if(event.type == SDL_KEYUP)
        {
            if(event.key.keysym.sym == SDLK_ESCAPE)
            {SetEscape(false);}


            if(event.key.keysym.sym == SDLK_RETURN)
            {SetEnter(false);}

            if(event.key.keysym.sym == SDLK_LSHIFT)
            {SetLShift(false);}

            if(event.key.keysym.sym == SDLK_RSHIFT)
            {SetRShift(false);}

            if(event.key.keysym.sym == SDLK_SPACE)
            {SetSpace(false);}


            if(event.key.keysym.sym == SDLK_UP)
            {SetUp(false);}

            if(event.key.keysym.sym == SDLK_DOWN)
            {SetDown(false);}

            if(event.key.keysym.sym == SDLK_LEFT)
            {SetLeft(false);}

            if(event.key.keysym.sym == SDLK_RIGHT)
            {SetRight(false);}


            if(event.key.keysym.sym == SDLK_w)
            {SetW(false);}

            if(event.key.keysym.sym == SDLK_a)
            {SetA(false);}

            if(event.key.keysym.sym == SDLK_s)
            {SetS(false);}

            if(event.key.keysym.sym == SDLK_d)
            {SetD(false);}

            if(event.key.keysym.sym == SDLK_q)
            {SetQ(false);}

            if(event.key.keysym.sym == SDLK_e)
            {SetE(false);}
        }
    }
}
