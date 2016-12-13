
#include "lrGL3StaticMeshShader.h"

const char *VertexShaderSource = R"(
	#version 330
	//
	uniform mat4 ProjectionMatrix;
	uniform mat4 ViewMatrix;
	uniform mat4 ModelMatrix;
	uniform mat3 NormalMatrix;
	//
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	//
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	//
	//
	//
	//
	//
	//
	//
	//
	//
	void main()
	{
		fVertex = (ModelMatrix*vec4(Vertex,1.0)).xyz;
		fNormal = NormalMatrix*Normal;
		fTexCoord = TexCoord;
		gl_Position = ProjectionMatrix*ViewMatrix*ModelMatrix*vec4(Vertex,1.0);
	}
)";

const char *FragmentShaderSource = R"(
	#version 330
	//
	uniform vec3 CameraPosition;
	//
	const float PI = 3.14159265359;
	//
	uniform vec3 MatAlbedo;
	uniform vec4 Material1;
	//
	uniform vec3 LightPosition;
	uniform vec3 LightColor;
	uniform float LightIntensity;
	//
	// MatFresnel Material1.x;
	// MatRoughness Material1.y;
	// MatReflectiveness Material1.z;
	// MatMetallic Material1.w;
	//
	in vec3 fVertex;
	in vec3 fNormal;
	in vec2 fTexCoord;
	//
	out vec4 FragColor;
	//
	vec3 LambertianDiffuse(float LightDotNormal)
	{
		return (MatAlbedo * LightDotNormal) / PI;
	}
	//
	vec3 FresnelSchlick(vec3 Fresnel,float CameraDotHalfway)
	{
		return Fresnel + (1.0 - Fresnel)*pow((1.0 - CameraDotHalfway),5);
	}
	//
	float BeckmannDistribution(float NormalDotHalfway)
	{
		float Roughness = Material1.y;
		if(Roughness < 1e-4)
		{return 0.0;}
		//
		float PowSqr = pow(NormalDotHalfway,2);
		float RoughnessSqr = Roughness * Roughness;
		float Exponent = (PowSqr - 1.0)/(RoughnessSqr * PowSqr);
		//
		return exp(Exponent)/(PI * RoughnessSqr * PowSqr*PowSqr);
	}
	//
	float HeavisideFunction(float a)
	{
		if(a > 0)
		{
			return 1.0;
		}
		else
		{
			return 0.0;
		}
	}
	//
	float SmithLambda(float CosAngle)
	{
		float Roughness = Material1.y;
		//
		float CosSqr = CosAngle * CosAngle;
		float TgSqr = (1.0 - CosSqr)/CosSqr;
		//
		return (-1.0 + sqrt(1 + Roughness * Roughness * TgSqr)) / 2.0;
	}
	//
	float SmithVisibilityFunction(float CameraDotHalfway,float LightDotHalfway,float CameraDotNormal,float LightDotNormal)
	{
		return HeavisideFunction(CameraDotHalfway) * HeavisideFunction(LightDotHalfway) / (1 + SmithLambda(CameraDotNormal) + SmithLambda(LightDotNormal));
	}
	//
	void main()
	{
		vec3 LightDirection = LightPosition - fVertex;
		float LightDistance = length(LightDirection);
		vec3 Radiance = LightColor * LightIntensity / (LightDistance*LightDistance);
		//
		LightDirection = normalize(LightDirection);
		vec3 Normal = normalize(fNormal);
		//
		float LightDotNormal = dot(LightDirection,Normal);
		//
		if(LightDotNormal < 0.0)
		{
			FragColor = vec4(0.0,0.0,0.0,1.0);
			return;
		}
		//
		vec3 DiffuseContrib = LambertianDiffuse(LightDotNormal);
		//
		vec3 CameraDirection = normalize(CameraPosition - fVertex);
		//
		vec3 HalfwayVector = normalize(CameraDirection + LightDirection);
		float CameraDotNormal = dot(CameraDirection,Normal);
		float CameraDotHalfway = dot(CameraDirection,HalfwayVector);
		float LightDotHalfway = max(0.0,dot(LightDirection,HalfwayVector));
		float NormalDotHalfway = dot(Normal,HalfwayVector);
		//
		float D = BeckmannDistribution(NormalDotHalfway);
		float G = SmithVisibilityFunction(CameraDotHalfway,LightDotHalfway,CameraDotNormal,LightDotNormal);
		//
		float MatFresnel = Material1.x;
		float FNonMetallic = FresnelSchlick(vec3(MatFresnel),CameraDotHalfway).x;
		vec3 FMetallic = FresnelSchlick(MatAlbedo,CameraDotHalfway);
		//
		float SpeculatMultiplier = (D * G) / (4.0 * CameraDotNormal);
		//
		float SpecularContrib = FNonMetallic * SpeculatMultiplier;
		//
		vec3 MetallicContrib = FMetallic * SpeculatMultiplier;
		//
		//
		//
		//
		//
		vec3 SpecularContrib_v3 = vec3(SpecularContrib,SpecularContrib,SpecularContrib);
		float MatReflectiveness = Material1.z;
		//
		vec3 NonMetallicContrib = DiffuseContrib * (1.0 - MatReflectiveness) + SpecularContrib_v3 * MatReflectiveness;
		//
		float MatMetallic = Material1.w;
		vec3 Contrib = NonMetallicContrib * (1.0 - MatMetallic) + MetallicContrib * MatMetallic;
		//
		FragColor = vec4(Contrib * Radiance,1.0);
		//FragColor = vec4(Normal,1.0);
		//FragColor = vec4(CameraDirection,1.0);
		//FragColor = vec4(CameraDotHalfway,CameraDotNormal,0.0,1.0);
		//FragColor = vec4(1.0,0.0,0.0,1.0);
	}
)";
