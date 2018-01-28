
#include "lrGLShaderLibrary.h"

#include <iostream>

const char *VertexShaderSource2D = R"(
#version 330

uniform mat3 CameraMatrix;
uniform mat3 TransformMatrix;

in vec2 Vertex;

void main()
{
	vec3 VertexPosition = CameraMatrix*TransformMatrix*vec3(Vertex,1.0);
	//
	gl_Position = vec4(VertexPosition.xy,0.0,1.0);
}
)";

const char *FragmentShaderSource2D = R"(
#version 330

uniform vec4 Color;

out vec4 FragColor;

void main()
{
	//
	FragColor = Color;
}
)";


const char *VertexShaderPrepassSource = R"(
	#version 330
	//
	uniform mat4 ProjectionMatrix;
	uniform mat4 ViewMatrix;
	uniform mat4 ModelMatrix;
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	//
	in vec3 Vertex;
	//
	void main()
	{
		gl_Position = MvpMatrix*vec4(Vertex,1.0);
	}
)";

const char *FragmentShaderPrepassSource = R"(
	#version 330
	
	out vec4 FragColor;
	
	void main()
	{
		FragColor = vec4(1.0,0.0,0.0,1.0);
	}
)";

const char *StaticMeshVertexShaderSource = R"(
	#version 330
	//
	uniform mat4 ProjectionMatrix;
	uniform mat4 ViewMatrix;
	uniform mat4 ModelMatrix;
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
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
		gl_Position = MvpMatrix*vec4(Vertex,1.0);
	}
)";

const char *PbEquationsSource = R"(
	#version 330
	//
	const float PI = 3.14159265359;
	//
	vec3 LambertianDiffuse(vec3 Diffuse,float LightDotNormal)
	{
		return (Diffuse * LightDotNormal) / PI;
	}
	//
	vec3 FresnelSchlick(vec3 Fresnel,float CameraDotHalfway)
	{
		return Fresnel + (1.0 - Fresnel)*pow(max(1e-4,(1.0 - CameraDotHalfway)),5);
	}
	//
	float BeckmannDistribution(float Roughness,float NormalDotHalfway)
	{
		if(Roughness < 1e-4)
		{return 0.0;}
		//
		float PowSqr = NormalDotHalfway*NormalDotHalfway;
		float RoughnessSqr = Roughness * Roughness;
		float Exponent = (PowSqr - 1.0)/(RoughnessSqr * PowSqr);
		//
		return exp(Exponent)/(PI * RoughnessSqr * PowSqr*PowSqr);
	}
	//
	float TrowbridgeReitzDistribution(float Roughness,float NormalDotHalfway)
	{
		if(Roughness < 1e-4)
		{return 0.0;}
		//
		float PowSqr = NormalDotHalfway*NormalDotHalfway;
		float RoughnessSqr = Roughness * Roughness;
		float Tan = (1 - PowSqr) / PowSqr;
		return 1.0 / (PI * RoughnessSqr * (1 + Tan/(RoughnessSqr)));
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
	float SmithLambda(float Roughness,float CosAngle)
	{
		float CosSqr = CosAngle * CosAngle;
		float TgSqr = (1.0 - CosSqr)/CosSqr;
		//
		return (-1.0 + sqrt(1 + Roughness * Roughness * TgSqr)) / 2.0;
	}
	//
	float SmithVisibilityFunction(float Roughness,float CameraDotHalfway,float LightDotHalfway,float CameraDotNormal,float LightDotNormal)
	{
		return HeavisideFunction(CameraDotHalfway) * HeavisideFunction(LightDotHalfway) / (1 + SmithLambda(Roughness,CameraDotNormal) + SmithLambda(Roughness,LightDotNormal));
	}
	//
	float CalculateSurfaceRoughness(float Roughness,float NormalDotHalfway,float CameraDotHalfway,float LightDotHalfway,float CameraDotNormal,float LightDotNormal)
	{
		float D = BeckmannDistribution(Roughness,NormalDotHalfway);
		//float D = TrowbridgeReitzDistribution(Roughness,NormalDotHalfway);
		float G = SmithVisibilityFunction(Roughness,CameraDotHalfway,LightDotHalfway,CameraDotNormal,LightDotNormal);
		//
		return (D * G) / (4.0 * CameraDotNormal);
	}
	//
	vec3 CalculateNonMetallicContrib(vec3 DiffuseContrib,float Fresnel,float CameraDotHalfway,float SurfaceRoughness,float Reflectiveness)
	{
		float F = FresnelSchlick(vec3(Fresnel),CameraDotHalfway).x;
		float SpecularContrib = F * SurfaceRoughness;
		//
		return DiffuseContrib * (1.0 - Reflectiveness) + vec3(SpecularContrib) * Reflectiveness;
	}
	//
	vec3 CalculateMetallicContrib(vec3 Fresnel,float CameraDotHalfway,float SurfaceRoughness)
	{
		vec3 F = FresnelSchlick(Fresnel,CameraDotHalfway);
		return F * SurfaceRoughness;
	}
	//
)";

const char *FwdFragmentShaderSrc = R"(
	#version 330
	//
	uniform sampler2D AlbedoMap;
	//
	uniform vec3 MatAlbedo;
	uniform vec4 Material1;
	//
	// Fresnel Material1.x;
	// Roughness Material1.y;
	// Reflectiveness Material1.z;
	// Metallic Material1.w;
	//
	in vec3 fVertex;
	in vec3 fNormal;
	in vec2 fTexCoord;
	//
	vec3 GetAlbedo()
	{
		vec3 TexAlbedo = texture(AlbedoMap,fTexCoord).rgb;
		return MatAlbedo * TexAlbedo;
	}
	//
	float GetFresnel()
	{
		//float TexFresnel = texture(FresnelMap,fTexCoord).r;
		return Material1.x;// * TexFresnel;
	}
	//
	float GetRoughness()
	{
		//float TexRoughness = texture(RoughnessMap,fTexCoord).r;
		return Material1.y;// * TexRoughness;
	}
	//
	float GetReflectiveness()
	{
		//float TexReflectiveness = texture(ReflectivenessMap,fTexCoord).r;
		return Material1.z;// * TexReflectiveness;
	}
	//
	float GetMetallic()
	{
		//float TexMetallic = texture(MetallicMap,fTexCoord).r;
		return Material1.w;// * TexMetallic;
	}
	//
	vec3 GetPosition()
	{
		return fVertex;
	}
	//
	vec3 GetNormal()
	{
		return normalize(fNormal);
	}
	//
)";

const char *EnvMapShaderSource = R"(
	#version 330
	//
	uniform vec3 CameraPosition;
	//
	const float PI = 3.14159265359;
	//
	//uniform float AmbientIntensity = 0.5;
	//uniform vec3 AmbientColor = vec3(0.125,0.35,0.35);
	uniform samplerCube EnvironmentMap;
	//
	out vec4 FragColor;
	//
	vec3 GetPosition();
	vec3 GetNormal();
	//
	vec3 GetAlbedo();
	float GetFresnel();
	float GetRoughness();
	float GetReflectiveness();
	float GetMetallic();
	//
	vec3 GetEnvironmentData(vec3 ReflectionVector)
	{
		vec3 EnvironmentData = /* AmbientColor * AmbientIntensity * */ texture(EnvironmentMap,ReflectionVector).rgb;
		//
		return EnvironmentData;
	}
	//
	float WarrenInverse(uint bits)
	{
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
		bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
		return float(bits) * 2.3283064365386963e-10; // / 0x100000000
	}
	//
	vec2 Hammersley(uint i, uint N)
	{
		return vec2(float(i)/float(N), WarrenInverse(i));
	}
	//
	vec2 ImportanceSampleBeckmann(float Roughness,vec2 Random)
	{
		float x = atan(sqrt(-Roughness*Roughness*log(1.0-Random.x)));
		float y = 2.0*PI*Random.y;
		//
		return vec2(x,y);
	}
	//
	vec3 GetHemispherePoint(vec2 Params)
	{
		float SinTheta = sin(Params.x);
		//
		float x = cos(Params.y) * SinTheta;
		float y = sin(Params.y) * SinTheta;
		float z = cos(Params.x);
		//
		return vec3(x,y,z);
	}
	//
	vec3 LambertianDiffuse(vec3 Diffuse,float LightDotNormal);
	float SmithVisibilityFunction(float Roughness,float CameraDotHalfway,float LightDotHalfway,float CameraDotNormal,float LightDotNormal);
	//
	vec3 CalculateNonMetallicContrib(vec3 DiffuseContrib,float Fresnel,float CameraDotHalfway,float SurfaceRoughness,float Reflectiveness);
	//
	vec3 CalculateMetallicContrib(vec3 Fresnel,float CameraDotHalfway,float SurfaceRoughness);
	//
	void main()
	{
		vec3 Position = GetPosition();
		vec3 Normal = GetNormal();
		//
		vec3 Albedo = GetAlbedo();
		float Fresnel = GetFresnel();
		float Roughness = GetRoughness();
		float Reflectiveness = GetReflectiveness();
		float Metallic = GetMetallic();
		//
		vec3 CameraDirection = normalize(CameraPosition - Position);
		//
		float CameraDotNormal = max(1e-4,dot(CameraDirection,Normal));
		//
		//vec3 ReflectionVector = reflect(-CameraDirection,Normal);
		//vec3 Up = ReflectionVector;
		//
		vec3 Up = Normal;
		//
		vec3 BaseX = normalize(cross(Up,CameraDirection));
		//
		if((1.0 - CameraDotNormal) < 1e-4)
		{
			BaseX = vec3(0.0,1.0,0.0);
		}
		vec3 BaseY = normalize(cross(Up,BaseX));
		//
		//CameraDotNormal = max(1e-4,CameraDotNormal);
		//
		const uint NumSamples = 25u;
		vec3 SampleVectors[NumSamples];
		//
		for(uint i=0u;i < NumSamples;i++)
		{
			vec2 HammersleyPoint = Hammersley(i,NumSamples);
			vec2 SampleParams = ImportanceSampleBeckmann(Roughness,HammersleyPoint);
			vec3 SampleVector = GetHemispherePoint(SampleParams);
			SampleVectors[i] = SampleVector.y*BaseY + SampleVector.x*BaseX + SampleVector.z*Normal;
		}
		//
		vec3 DiffuseSum = vec3(0);
		vec3 SpecularSum = vec3(0);
		for(uint i=0u;i < NumSamples;i++)
		{
			//vec3 LightDirection = normalize(SampleVectors[i]);
			//vec3 HalfwayVector = normalize(CameraDirection + LightDirection);
			//
			vec3 HalfwayVector = SampleVectors[i];
			vec3 LightDirection = normalize(reflect(-CameraDirection,HalfwayVector));
			//
			float LightDotNormal = max(1e-4,dot(LightDirection,Normal));
			//
			float CameraDotHalfway = max(1e-4,dot(CameraDirection,HalfwayVector));
			float LightDotHalfway = max(1e-4,dot(LightDirection,HalfwayVector));
			//
			float NormalDotHalfway = max(1e-4,dot(Normal,HalfwayVector));
			//
			vec3 EnvironmentData = GetEnvironmentData(LightDirection);
			//
			float G = SmithVisibilityFunction(Roughness,CameraDotHalfway,LightDotHalfway,CameraDotNormal,LightDotNormal);
			float SurfaceRoughness = G / (4.0 * CameraDotNormal);
			//
			vec3 NonMetallicContrib = CalculateNonMetallicContrib(vec3(0.0),Fresnel,CameraDotHalfway,SurfaceRoughness,Reflectiveness);
			vec3 MetallicContrib = CalculateMetallicContrib(Albedo,CameraDotHalfway,SurfaceRoughness);
			//
			/*
			 * Adding it all up
			 */
			//float SpecularImportance = NormalDotHalfway / (4.0*max(1e-1,CameraDotHalfway));
			float SpecularImportance = NormalDotHalfway / (4.0*CameraDotHalfway);
			vec3 SpecularContrib = (NonMetallicContrib * (1.0 - Metallic) + MetallicContrib * Metallic);
			SpecularSum += SpecularContrib * EnvironmentData / SpecularImportance;
			//
			float DiffuseImportance = LightDotNormal/PI;
			vec3 DiffuseContrib = LambertianDiffuse(Albedo,LightDotNormal) * (1.0 - Metallic) * (1.0 - Reflectiveness);
			DiffuseSum += DiffuseContrib * EnvironmentData / DiffuseImportance;
		}
		//
		FragColor = vec4((DiffuseSum + SpecularSum) / NumSamples,1.0);
	}
)";
/*
const char *SingleLightFragmentShaderSource = R"(
	#version 330
	//
	uniform vec3 CameraPosition;
	//
	const float PI = 3.14159265359;
	//
	uniform vec3 LightPosition;
	uniform vec3 LightColor;
	uniform float LightIntensity;
	//
	out vec4 FragColor;
	//
	vec3 GetPosition();
	vec3 GetNormal();
	//
	vec3 GetAlbedo();
	float GetFresnel();
	float GetRoughness();
	float GetReflectiveness();
	float GetMetallic();
	//
	vec3 LambertianDiffuse(vec3 Diffuse,float LightDotNormal);
	//
	float CalculateSurfaceRoughness(float Roughness,float NormalDotHalfway,float CameraDotHalfway,float LightDotHalfway,float CameraDotNormal,float LightDotNormal);
	//
	vec3 CalculateNonMetallicContrib(vec3 DiffuseContrib,float Fresnel,float CameraDotHalfway,float SurfaceRoughness,float Reflectiveness);
	//
	vec3 CalculateMetallicContrib(vec3 Fresnel,float CameraDotHalfway,float SurfaceRoughness);
	//
	void main()
	{
		vec3 Position = GetPosition();
		vec3 Normal = GetNormal();
		//
		vec3 Albedo = GetAlbedo();
		float Fresnel = GetFresnel();
		float Roughness = GetRoughness();
		float Reflectiveness = GetReflectiveness();
		float Metallic = GetMetallic();
		//
		vec3 LightDirection = LightPosition - Position;
		float LightDistance = length(LightDirection);
		vec3 Radiance = LightColor * LightIntensity / (LightDistance*LightDistance);
		//
		LightDirection = normalize(LightDirection);
		vec3 CameraDirection = normalize(CameraPosition - Position);
		//
		float LightDotNormal = max(1e-4,dot(LightDirection,Normal));
		float CameraDotNormal = max(1e-4,dot(CameraDirection,Normal));
		//
		/*if(LightDotNormal <= 0.0 || CameraDotNormal <= 0.0)
		{
			FragColor = vec4(0.0,0.0,0.0,1.0);
			return;
		}* /
		//
		vec3 HalfwayVector = normalize(CameraDirection + LightDirection);
		//
		float CameraDotHalfway = max(1e-4,dot(CameraDirection,HalfwayVector));
		float LightDotHalfway = max(1e-4,dot(LightDirection,HalfwayVector));
		//
		float NormalDotHalfway = max(1e-4,dot(Normal,HalfwayVector));
		//
		/*
		 * Calculating diffuse
		 * /
		vec3 DiffuseContrib = LambertianDiffuse(Albedo,LightDotNormal);
		//
		/*
		 * Calculating non metallic and metallic
		 * /
		float SurfaceRoughness = CalculateSurfaceRoughness(Roughness,NormalDotHalfway,CameraDotHalfway,LightDotHalfway,CameraDotNormal,LightDotNormal);
		//
		vec3 NonMetallicContrib = CalculateNonMetallicContrib(DiffuseContrib,Fresnel,CameraDotHalfway,SurfaceRoughness,Reflectiveness);
		vec3 MetallicContrib = CalculateMetallicContrib(Albedo,CameraDotHalfway,SurfaceRoughness);
		//
		/*
		 * Adding it all up
		 * /
		vec3 Contrib = NonMetallicContrib * (1.0 - Metallic) + MetallicContrib * Metallic;
		//
		FragColor = vec4(Contrib * Radiance,1.0);
		//FragColor = vec4(Normal/2.0,1.0);
		//FragColor = vec4(CameraDirection,1.0);
		//FragColor = vec4(CameraDotHalfway,CameraDotNormal,0.0,1.0);
		//FragColor = vec4(1.0,0.0,0.0,1.0);
	}
)";
*/
const char *FragmentShaderSource = R"(
	#version 330
	//
	uniform vec3 CameraPosition;
	//
	const float PI = 3.14159265359;
	//
	const int MAX_LIGHTS = 256;
	//
	uniform int NumLights;
	uniform vec3 LightPosition[MAX_LIGHTS];
	uniform vec3 LightColor[MAX_LIGHTS];
	uniform float LightIntensity[MAX_LIGHTS];
	//
	out vec4 FragColor;
	//
	vec3 GetPosition();
	vec3 GetNormal();
	//
	vec3 GetAlbedo();
	float GetFresnel();
	float GetRoughness();
	float GetReflectiveness();
	float GetMetallic();
	//
	vec3 LambertianDiffuse(vec3 Diffuse,float LightDotNormal);
	//
	float CalculateSurfaceRoughness(float Roughness,float NormalDotHalfway,float CameraDotHalfway,float LightDotHalfway,float CameraDotNormal,float LightDotNormal);
	//
	vec3 CalculateNonMetallicContrib(vec3 DiffuseContrib,float Fresnel,float CameraDotHalfway,float SurfaceRoughness,float Reflectiveness);
	//
	vec3 CalculateMetallicContrib(vec3 Fresnel,float CameraDotHalfway,float SurfaceRoughness);
	//
	void main()
	{
		vec3 Position = GetPosition();
		vec3 Normal = GetNormal();
		//
		vec3 Albedo = GetAlbedo();
		float Fresnel = GetFresnel();
		float Roughness = GetRoughness();
		float Reflectiveness = GetReflectiveness();
		float Metallic = GetMetallic();
		//
		vec3 Accumulator = vec3(0.0,0.0,0.0);
		for(int i=0;i < NumLights;i++)
		{
			vec3 LightDirection = LightPosition[i] - Position;
			float LightDistance = length(LightDirection);
			vec3 Radiance = LightColor[i] * LightIntensity[i] / (LightDistance*LightDistance);
			//
			LightDirection = normalize(LightDirection);
			vec3 CameraDirection = normalize(CameraPosition - Position);
			//
			float LightDotNormal = max(1e-4,dot(LightDirection,Normal));
			float CameraDotNormal = max(1e-4,dot(CameraDirection,Normal));
			//
			vec3 HalfwayVector = normalize(CameraDirection + LightDirection);
			//
			float CameraDotHalfway = max(1e-4,dot(CameraDirection,HalfwayVector));
			float LightDotHalfway = max(1e-4,dot(LightDirection,HalfwayVector));
			//
			float NormalDotHalfway = max(1e-4,dot(Normal,HalfwayVector));
			/*
			 * Calculating diffuse
			 */
			vec3 DiffuseContrib = LambertianDiffuse(Albedo,LightDotNormal);
			//
			/*
			 * Calculating non metallic and metallic
			 */
			float SurfaceRoughness = CalculateSurfaceRoughness(Roughness,NormalDotHalfway,CameraDotHalfway,LightDotHalfway,CameraDotNormal,LightDotNormal);
			//
			vec3 NonMetallicContrib = CalculateNonMetallicContrib(DiffuseContrib,Fresnel,CameraDotHalfway,SurfaceRoughness,Reflectiveness);
			vec3 MetallicContrib = CalculateMetallicContrib(Albedo,CameraDotHalfway,SurfaceRoughness);
			//
			/*
			 * Adding it all up
			 */
			vec3 Contrib = NonMetallicContrib * (1.0 - Metallic) + MetallicContrib * Metallic;
			Accumulator += Contrib * Radiance;
		}
		//
		FragColor = vec4(Accumulator,1.0);
		//FragColor = vec4(Normal/2.0,1.0);
		//FragColor = vec4(CameraDirection,1.0);
		//FragColor = vec4(CameraDotHalfway,CameraDotNormal,0.0,1.0);
		
		//FragColor = vec4(LastStuff,0.0,0.0,1.0);
	}
)";

GLuint lrGLShaderLibrary::LoadShader(const char *src,GLenum shader_type,bool print_status)
{
	GLuint Id;
	Id = glCreateShader(shader_type);
	
	glShaderSource(Id,1,&src,NULL);
	glCompileShader(Id);
	
	if(print_status)
	{
		char Error[1024];
		
		glGetShaderInfoLog(Id,1024,NULL,Error);
		std::cout << "Shader compile status:\n" << Error << std::endl;
	}
	
	return Id;
}

lrGLShaderLibrary::lrGLShaderLibrary()
{
	lrGLShaderObject &VertexShader2D = ShaderObjects["VertexShader2D"];
	VertexShader2D.Initialize(LoadShader(VertexShaderSource2D,GL_VERTEX_SHADER,true),GL_VERTEX_SHADER);
	
	lrGLShaderObject &FragmentShader2D = ShaderObjects["FragmentShader2D"];
	FragmentShader2D.Initialize(LoadShader(FragmentShaderSource2D,GL_FRAGMENT_SHADER,true),GL_FRAGMENT_SHADER);
	
	lrGLShaderObject &StaticMeshPrepassVertexShader = ShaderObjects["StaticMeshPrepassVertexShader"];
	StaticMeshPrepassVertexShader.Initialize(LoadShader(VertexShaderPrepassSource,GL_VERTEX_SHADER,true),GL_VERTEX_SHADER);
	
	lrGLShaderObject &PrepassFragmentShader = ShaderObjects["PrepassFragmentShader"];
	PrepassFragmentShader.Initialize(LoadShader(FragmentShaderPrepassSource,GL_FRAGMENT_SHADER,true),GL_FRAGMENT_SHADER);
	
	lrGLShaderObject &StaticMeshVertexShader = ShaderObjects["StaticMeshVertexShader"];
	StaticMeshVertexShader.Initialize(LoadShader(StaticMeshVertexShaderSource,GL_VERTEX_SHADER,true),GL_VERTEX_SHADER);
	
	lrGLShaderObject &PbEquationsShader = ShaderObjects["PbEquationsShader"];
	PbEquationsShader.Initialize(LoadShader(PbEquationsSource,GL_FRAGMENT_SHADER,true),GL_FRAGMENT_SHADER);
	
	lrGLShaderObject &FwdRendererShader = ShaderObjects["FwdRendererShader"];
	FwdRendererShader.Initialize(LoadShader(FwdFragmentShaderSrc,GL_FRAGMENT_SHADER,true),GL_FRAGMENT_SHADER);
	
	lrGLShaderObject &PbEnvMapShaderMain = ShaderObjects["PbEnvMapShaderMain"];
	PbEnvMapShaderMain.Initialize(LoadShader(EnvMapShaderSource,GL_FRAGMENT_SHADER,true),GL_FRAGMENT_SHADER);
	
	lrGLShaderObject &PbFragmentShaderMain = ShaderObjects["PbFragmentShaderMain"];
	PbFragmentShaderMain.Initialize(LoadShader(FragmentShaderSource,GL_FRAGMENT_SHADER,true),GL_FRAGMENT_SHADER);
}
