#ifndef LR_GL_LIGHTS_H
#define LR_GL_LIGHTS_H

#include <lScene/l3DScene/li3DElement.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <vector>

struct lrGLLightArray
{
	std::vector<float> Positions;
	std::vector<float> Colors;
	std::vector<float> Intensities;
	
	float *GetPositionBegin(unsigned int begin_light_id)
	{
		return &Positions[begin_light_id * 3];
	}
	
	float *GetColorBegin(unsigned int begin_light_id)
	{
		return &Colors[begin_light_id * 3];
	}
	
	float *GetIntensityBegin(unsigned int begin_light_id)
	{
		return &Intensities[begin_light_id];
	}
	
	unsigned int NumLights()
	{
		return Intensities.size();
	}
	
	void Resize(unsigned int new_size)
	{
		Positions.resize(new_size * 3);
		Colors.resize(new_size * 3);
		Intensities.resize(new_size);
	}
	
	void SetLight(const li3DLight &light,unsigned int light_id)
	{
		if(light_id < NumLights())
		{
			for(int i=0;i < 3;i++)
			{
				Positions[light_id*3 + i] = light.GetPosition()[i];
			}
			
			Colors[light_id*3 + 0] = light.GetColor().GetRed();
			Colors[light_id*3 + 1] = light.GetColor().GetGreen();
			Colors[light_id*3 + 2] = light.GetColor().GetBlue();
			
			Intensities[light_id] = light.GetIntensity();
		}
	}
};

#endif // LR_GL_LIGHTS_H
