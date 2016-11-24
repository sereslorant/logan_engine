#ifndef L_GL_DATA_H
#define L_GL_DATA_H

#include "../../lRenderer/lGLRenderer/lGLIncludes.h"

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
		Ambient[0] = light.GetAmbient(L_RED_INDEX);
		Diffuse[0] = light.GetDiffuse(L_RED_INDEX);
		Specular[0] = light.GetSpecular(L_RED_INDEX);
		//
		Ambient[1] = light.GetAmbient(L_GREEN_INDEX);
		Diffuse[1] = light.GetDiffuse(L_GREEN_INDEX);
		Specular[1] = light.GetSpecular(L_GREEN_INDEX);
		//
		Ambient[2] = light.GetAmbient(L_BLUE_INDEX);
		Diffuse[2] = light.GetDiffuse(L_BLUE_INDEX);
		Specular[2] = light.GetSpecular(L_BLUE_INDEX);
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
		Diffuse[0] = material.GetDiffuse(L_RED_INDEX);
		Specular[0] = material.GetSpecular(L_RED_INDEX);
		//
		Diffuse[1] = material.GetDiffuse(L_GREEN_INDEX);
		Specular[1] = material.GetSpecular(L_GREEN_INDEX);
		//
		Diffuse[2] = material.GetDiffuse(L_BLUE_INDEX);
		Specular[2] = material.GetSpecular(L_BLUE_INDEX);
		//
		Shininess = material.GetShininess();
	}
};

#endif //L_GL_DATA_H
