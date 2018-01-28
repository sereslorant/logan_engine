#ifndef LR_GL_DATA_H
#define LR_GL_DATA_H

#include <lScene/l3DScene/liMaterialLibrary.h>

struct lrGLMaterialData
{
	/*
	 * Material[0]: albedo
	 * Material[1]: Fresnel,roughness,reflectiveness,metallic
	 */
	float Material[2][4] = {{0.0,0.0,0.0,1.0},{0.0,0.0,0.0,0.0}};
	
	void SetMaterial(const liMaterial &material)
	{
		Material[0][0] = material.GetAlbedo().GetRed();
		Material[0][1] = material.GetAlbedo().GetGreen();
		Material[0][2] = material.GetAlbedo().GetBlue();
		
		Material[1][0] = material.GetFresnel();
		Material[1][1] = material.GetRoughness();
		Material[1][2] = material.GetReflectiveness();
		Material[1][3] = material.GetMetallic();
	}
	
	lrGLMaterialData()
	{}
	
	lrGLMaterialData(const liMaterial &material)
	{
		Material[0][0] = material.GetAlbedo().GetRed();
		Material[0][1] = material.GetAlbedo().GetGreen();
		Material[0][2] = material.GetAlbedo().GetBlue();
		
		Material[1][0] = material.GetFresnel();
		Material[1][1] = material.GetRoughness();
		Material[1][2] = material.GetReflectiveness();
		Material[1][3] = material.GetMetallic();
	}
};

#endif // LR_GL_DATA_H
