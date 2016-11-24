#ifndef L_GOO_OBJECT_FACTORY_H
#define L_GOO_OBJECT_FACTORY_H

#include "liGooObjectFactory.h"

#include "lGooCircle.h"
#include "lGooRect.h"
#include "lGooLine.h"
#include "lGooEllipse.h"

class lGooObjectFactory :public liGooObjectFactory
{
private:
	liGuiRenderer *GuiRenderer;

public:

	virtual void SetRenderer(liGuiRenderer *gui_renderer)
	{
		GuiRenderer = gui_renderer;
	}

	virtual liGuiRenderer *GetRenderer()
	{
		return GuiRenderer;
	}

    virtual liGooObject *NewCircle() override
    {
    	return new lGooCircle(GuiRenderer->GetCircle());
    }

    virtual liGooObject *NewRect() override
    {
    	return new lGooRect(GuiRenderer->GetRect());
    }

	virtual liGooObject *NewLine() override
	{
		return new lGooLine(GuiRenderer->GetLine());
	}

	virtual liGooObject *NewEllipse() override
	{
		return new lGooEllipse(GuiRenderer->GetEllipse());
	}

	virtual liGooObject *NewContainer() override
	{
		return new lGooContainer;
	}

	virtual liGooObject *NewGUI() override
	{
		return new lGooGUI(GuiRenderer->CreateDisplayList());
	}

    lGooObjectFactory()
    {}

    virtual ~lGooObjectFactory() override
    {}
};

#endif // L_GOO_OBJECT_FACTORY_H
