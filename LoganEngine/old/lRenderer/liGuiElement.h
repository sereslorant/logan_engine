#ifndef LI_GUI_SPRITE
#define LI_GUI_SPRITE

class liGuiElement
{
public:

    virtual void Draw(bool Outline) const = 0;

    liGuiElement()
    {}
    virtual ~liGuiElement()
    {}
};


#endif // LI_GUI_SPRITE
