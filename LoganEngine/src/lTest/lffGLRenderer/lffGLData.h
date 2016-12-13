#ifndef L_GL_DATA_H
#define L_GL_DATA_H

#include "../../lRenderer/lGLRenderer/lGLIncludes.h"

#include <cmath>

struct lffGLLightData
{
	GLfloat Position[4] = {0.0,0.0,0.0,1.0};
	GLfloat Ambient[4] = {0.0,0.0,0.0,1.0};
	GLfloat Diffuse[4] = {0.0,0.0,0.0,1.0};
	GLfloat Specular[4] = {0.0,0.0,0.0,1.0};
	//
	void SetLight(const li3DLight &light)
	{
		for(int i=0;i < 3;i++)
		{
			Position[i] = light.GetPosition()[i];
		}
		//
		//Ambient[0] = light.GetAmbient(L_RED_INDEX);
		Diffuse[0] = light.GetColor().GetRed();// * light.GetIntensity();
		Specular[0] = light.GetColor().GetRed();// * light.GetIntensity();
		//
		//Ambient[1] = light.GetAmbient(L_GREEN_INDEX);
		Diffuse[1] = light.GetColor().GetGreen();// * light.GetIntensity();
		Specular[1] = light.GetColor().GetGreen();// * light.GetIntensity();
		//
		//Ambient[2] = light.GetAmbient(L_BLUE_INDEX);
		Diffuse[2] = light.GetColor().GetBlue();// * light.GetIntensity();
		Specular[2] = light.GetColor().GetBlue();// * light.GetIntensity();
	}
};

struct lffGLMaterialData
{
	GLfloat Diffuse[4] = {0.0,0.0,0.0,1.0};
	GLfloat Specular[4] = {0.0,0.0,0.0,1.0};
	GLfloat Shininess = 0.0;
	//
	void SetMaterial(const liMaterial &material)
	{
		if(material.GetMetallic() > 0.9)
		{
			Specular[0] = material.GetAlbedo().GetRed();
			Specular[1] = material.GetAlbedo().GetGreen();
			Specular[2] = material.GetAlbedo().GetBlue();
		}
		else
		{
			float DiffuseWeight = (1.0 - material.GetReflectiveness());

			Diffuse[0] = material.GetAlbedo().GetRed() * DiffuseWeight;
			Diffuse[1] = material.GetAlbedo().GetGreen() * DiffuseWeight;
			Diffuse[2] = material.GetAlbedo().GetBlue() * DiffuseWeight;
			//
			Specular[0] = material.GetFresnel() * material.GetReflectiveness();
			Specular[1] = material.GetFresnel() * material.GetReflectiveness();
			Specular[2] = material.GetFresnel() * material.GetReflectiveness();
		}
		//
		Shininess = std::exp(-1.0 * material.GetRoughness());
	}
};

#endif //L_GL_DATA_H
