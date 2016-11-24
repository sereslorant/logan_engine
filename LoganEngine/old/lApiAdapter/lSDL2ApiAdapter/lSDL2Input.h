#ifndef L_SDL2_INPUT_H
#define L_SDL2_INPUT_H

#include "../lInput.h"

#include <SDL2/SDL.h>

class lSDL2Input final : public lInput
{
public:
    void CheckInput() final override;

    lSDL2Input(){}
    ~lSDL2Input(){}
};

#endif
