#ifndef LR_GL_LIGHTS_H
#define LR_GL_LIGHTS_H

#include "../../../lGLIncludes.h"
#include "../../../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!

#include "../../../liGLShaderInterfaces.h"

#include "lrGLData/lrGLLightData.h"

#include <vector>

struct lrGLLights
{
private:
	std::vector<lrGLLightData> Lights;
	//
public:
	//
	unsigned int NumLights()
	{
		return Lights.size();
	}
	//
	void Resize(unsigned int new_size)
	{
		Lights.resize(new_size);
	}
	//
	void SetLight(const li3DLight &light,unsigned int light_id)
	{
		if(light_id < Lights.size())
		{
			Lights[light_id].SetLight(light);
		}
	}
	//
	void ApplyLight(liGLPointLightShader &shader,unsigned int light_id)
	{
		if(light_id < Lights.size())
		{
			lrGLLightData &Light = Lights[light_id];
			//
			ApplyLight(shader,Light);
		}
	}
	//
	static void ApplyLight(liGLPointLightShader &shader,lrGLLightData &light)
	{
		glUniform3fv(shader.GetLightPositionLocation(),1,light.Position);
		glUniform3fv(shader.GetLightColorLocation(),1,light.Color);
		glUniform1f(shader.GetLightIntensityLocation(),light.Intensity);
	}
};

#endif // LR_GL_LIGHTS_H
