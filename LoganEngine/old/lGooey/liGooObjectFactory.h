#ifndef LI_GOO_OBJECT_FACTORY_H
#define LI_GOO_OBJECT_FACTORY_H

#include "../lRenderer/liGuiRenderer.h"

#include "liGooObject.h"

class liGooObjectFactory
{
public:

	virtual void SetRenderer(liGuiRenderer *renderer) = 0;
	virtual liGuiRenderer *GetRenderer() = 0;

    virtual liGooObject *NewCircle() = 0;
    virtual liGooObject *NewRect() = 0;
	virtual liGooObject *NewLine() = 0;
	virtual liGooObject *NewEllipse() = 0;

	virtual liGooObject *NewContainer() = 0;

	virtual liGooObject *NewGUI() = 0;

    liGooObjectFactory(){};
    virtual ~liGooObjectFactory(){};
};

#endif // LI_GOO_OBJECT_FACTORY_H
