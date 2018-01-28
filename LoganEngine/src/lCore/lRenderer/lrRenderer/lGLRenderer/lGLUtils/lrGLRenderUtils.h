#ifndef LR_GL_RENDER_UTILS_H
#define LR_GL_RENDER_UTILS_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGLData/lrGLMaterialData.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGL3Renderer/lGL3DCachedLayer/lrGL3SceneCache/lrGLData/lrGLTransformArray.h>

class lrGLRenderUtils
{
public:
	/*
	static void UploadUniformLight(liGLPointLightShader &shader,const lrGLLightData &light)
	{
		glUniform3fv(shader.GetLightPositionLocation(0),1,light.Position);
		glUniform3fv(shader.GetLightColorLocation(0),1,light.Color);
		glUniform1f(shader.GetLightIntensityLocation(0),light.Intensity);
	}
	*//*
	static void UploadUniformLight(liGLPointLightShader &shader,const lrGLLightData &light,unsigned int id)
	{
		glUniform3fv(shader.GetLightPositionLocation(id),1,light.Position);
		glUniform3fv(shader.GetLightColorLocation(id),1,light.Color);
		glUniform1f(shader.GetLightIntensityLocation(id),light.Intensity);
	}
	*/
	static void UploadUniformLightArray(liGLPointLightShader &shader,float *positions,float *colors,float *intensities,unsigned int size)
	{
		glUniform1i(shader.GetLightCountLocation(),size);
		
		glUniform3fv(shader.GetLightPositionLocation(),size,positions);
		glUniform3fv(shader.GetLightColorLocation(),size,colors);
		glUniform1fv(shader.GetLightIntensityLocation(),size,intensities);
	}
	
	static void UploadUniformMaterial(liGLPbMatShader &shader,const lrGLMaterialData &material)
	{
		glUniform3fv(shader.GetMatAlbedoLocation(),1,material.Material[0]);
		glUniform4fv(shader.GetMaterial1Location(),1,material.Material[1]);
	}
	
	static void UploadUniformTransform(liGL3DShader &shader,const lrGLTransform &transform)
	{
		glUniformMatrix4fv(shader.GetModelMatrixLocation(),1,GL_FALSE,transform.ModelMatrix[0]);
		glUniformMatrix3fv(shader.GetNormalMatrixLocation(),1,GL_FALSE,transform.NormalMatrix[0]);
		glUniformMatrix4fv(shader.GetMvpMatrixLocation(),1,GL_FALSE,transform.MvpMatrix[0]);
	}
};

#endif // LR_GL_RENDER_UTILS_H
