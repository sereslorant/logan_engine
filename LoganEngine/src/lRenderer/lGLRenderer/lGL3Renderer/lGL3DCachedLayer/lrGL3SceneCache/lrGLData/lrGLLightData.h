#ifndef LR_GL_LIGHT_DATA_H
#define LR_GL_LIGHT_DATA_H

#include "../../../../lGLIncludes.h"

#include "../../../../../../lInterfaces/lScene/l3DScene/li3DElement.h"

struct lrGLLightData
{
	GLfloat Position[4] = {0.0,0.0,0.0,1.0};
	GLfloat Color[4] = {0.0,0.0,0.0,1.0};
	GLfloat Intensity = 0.0;
	//
	void SetLight(const li3DLight &light)
	{
		for(int i=0;i < 3;i++)
		{
			Position[i] = light.GetPosition()[i];
		}
		//
		Color[0] = light.GetColor().GetRed();
		Color[1] = light.GetColor().GetGreen();
		Color[2] = light.GetColor().GetBlue();
		//
		Intensity = light.GetIntensity();
	}
};

#endif // LR_GL_LIGHT_DATA_H
