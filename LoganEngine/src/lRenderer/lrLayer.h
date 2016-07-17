#ifndef LR_LAYER_H
#define LR_LAYER_H

#include "../lInterfaces/lRenderer/li2DRenderer.h"

class lrLayer : public liLayer
{
private:
	bool Enabled = false;

	virtual void DrawScene() = 0;

public:

	virtual void Enable() override
	{
		Enabled = true;
	}

	virtual void Disable() override
	{
		Enabled = false;
	}

	void Draw()
	{
		if(Enabled)
		{
			DrawScene();
		}
	}

	lrLayer(){}
	virtual ~lrLayer() override{}
};

#endif // LR_LAYER_H
