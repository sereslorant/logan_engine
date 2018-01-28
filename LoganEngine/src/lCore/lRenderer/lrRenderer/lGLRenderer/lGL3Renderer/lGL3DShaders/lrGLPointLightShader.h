#ifndef LR_GL_POINT_LIGHT_SHADER
#define LR_GL_POINT_LIGHT_SHADER

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>

#include <string>

class lrGLPointLightShader : public liGLPointLightShader
{
protected:
	lrGLShader &Shader;
	
public:
	
	virtual GLint GetLightCountLocation() override
	{
		return Shader.GetUniformLocation("NumLights");
	}
	
	virtual GLint GetLightPositionLocation() override
	{
		return Shader.GetUniformLocation("LightPosition[0]");
	}
	
	virtual GLint GetLightColorLocation() override
	{
		return Shader.GetUniformLocation("LightColor[0]");
	}
	
	virtual GLint GetLightIntensityLocation() override
	{
		return Shader.GetUniformLocation("LightIntensity[0]");
	}
	/*
	virtual GLint GetLightPositionLocation(unsigned int id) override
	{
		std::string VariableName = "LightPosition[" + std::to_string(id) + "]";
		return Shader.GetUniformLocation(VariableName.c_str());
	}
	
	virtual GLint GetLightColorLocation(unsigned int id) override
	{
		std::string VariableName = "LightColor[" + std::to_string(id) + "]";
		return Shader.GetUniformLocation(VariableName.c_str());
	}
	
	virtual GLint GetLightIntensityLocation(unsigned int id) override
	{
		std::string VariableName = "LightIntensity[" + std::to_string(id) + "]";
		return Shader.GetUniformLocation(VariableName.c_str());
	}
	*//*
	virtual GLint GetLightPositionLocation(unsigned int id) override
	{
		std::string VariableName = "Lights[" + std::to_string(id) + "].LightPosition";
		return Shader.GetUniformLocation(VariableName.c_str());
	}
	
	virtual GLint GetLightColorLocation(unsigned int id) override
	{
		std::string VariableName = "Lights[" + std::to_string(id) + "].LightColor";
		return Shader.GetUniformLocation(VariableName.c_str());
	}
	
	virtual GLint GetLightIntensityLocation(unsigned int id) override
	{
		std::string VariableName = "Lights[" + std::to_string(id) + "].LightIntensity";
		return Shader.GetUniformLocation(VariableName.c_str());
	}
	*/
	
	lrGLPointLightShader(lrGLShader &shader)
		:Shader(shader)
	{}
	
	virtual ~lrGLPointLightShader() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_POINT_LIGHT_SHADER
