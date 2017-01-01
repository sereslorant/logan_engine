#ifndef LR_GL_DATA_H
#define LR_GL_DATA_H

#include "../../../lGLIncludes.h"

struct lrGLLightData
{
	GLfloat Position[4] = {0.0,0.0,0.0,1.0};
	//GLfloat Ambient[4] = {0.0,0.0,0.0,1.0};
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

struct lrGLMaterialData
{
	/*
	 * Material[0]: albedo
	 * Material[1]: Fresnel,roughness,reflectiveness,metallic
	 */
	GLfloat Material[2][4] = {{0.0,0.0,0.0,1.0},{0.0,0.0,0.0,0.0}};
	//
	void SetMaterial(const liMaterial &material)
	{
		Material[0][0] = material.GetAlbedo().GetRed();
		Material[0][1] = material.GetAlbedo().GetGreen();
		Material[0][2] = material.GetAlbedo().GetBlue();
		//
		Material[1][0] = material.GetFresnel();
		Material[1][1] = material.GetRoughness();
		Material[1][2] = material.GetReflectiveness();
		Material[1][3] = material.GetMetallic();
	}
};

#endif // LR_GL_DATA_H
