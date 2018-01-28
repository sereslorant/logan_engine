#ifndef TEST_SHADERS_H
#define TEST_SHADERS_H

const char *TestStaticVertexShader = R"(
	#version 330
	
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	
	void main()
	{
		fVertex = (/*ModelMatrix**/vec4(Vertex,1.0)).xyz;
		fNormal = /*NormalMatrix* */Normal;
		fTexCoord = TexCoord;
		gl_Position = /* MvpMatrix* */vec4(Vertex / 4.0,1.0);
	}
)";

const char *TestSkeletalVertexShader = R"(
	#version 330
	
	const int MAX_WEIGHTS = 4;
	
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	
	uniform mat4 BoneTransform[128];
	
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	
	in ivec4 BoneId;
	in vec4  Weight;
	
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	
	void main()
	{
		mat4 Transform[MAX_WEIGHTS];
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			if(BoneId[i] > -1)
			{
				Transform[i] = BoneTransform[BoneId[i]];
			}
			else
			{
				Transform[i] = mat4(0.0);
			}
		}
		
		vec4 v4Vertex = vec4(Vertex,1.0);

		vec3 AnimVertex = vec3(0.0,0.0,0.0);
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			AnimVertex += (Weight[i] * (Transform[i] * v4Vertex)).xyz;
		}
		
		fVertex = (/*ModelMatrix**/vec4(Vertex,1.0)).xyz;
		fNormal = /*NormalMatrix* */Normal;
		fTexCoord = TexCoord;
		//gl_Position = /* MvpMatrix* */vec4(Vertex / 4.0,1.0) - vec4(0.0,0.5,0.0,0.0);
		gl_Position = /* MvpMatrix* */vec4(AnimVertex / 4.0,1.0) - vec4(0.0,0.5,0.0,0.0);
	}
)";

const char *TestMvpStaticVertexShader = R"(
	#version 330
	
	uniform mat4 ModelMatrix;
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	
	void main()
	{
		fVertex = (ModelMatrix*vec4(Vertex,1.0)).xyz;
		fNormal = NormalMatrix*Normal;
		fTexCoord = TexCoord;
		gl_Position = MvpMatrix*vec4(Vertex,1.0);
	}
)";

constexpr unsigned int NUM_BONES = 128;

const char *TestMvpSkeletalVertexShader = R"(
	#version 330
	
	const int MAX_WEIGHTS = 4;
	
	uniform mat4 ModelMatrix;
	uniform mat3 NormalMatrix;
	uniform mat4 MvpMatrix;
	
	uniform mat4 BoneTransform[128];
	
	in vec3 Vertex;
	in vec3 Normal;
	in vec3 Tangent;
	in vec3 Bitangent;
	in vec2 TexCoord;
	
	in ivec4 BoneId;
	in vec4  Weight;
	
	out vec3 fVertex;
	out vec3 fNormal;
	out vec2 fTexCoord;
	
	void main()
	{
		mat4 Transform[MAX_WEIGHTS];
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			if(BoneId[i] > -1)
			{
				Transform[i] = BoneTransform[BoneId[i]];
			}
			else
			{
				Transform[i] = mat4(0.0);
			}
		}
		
		vec4 v4Vertex = vec4(Vertex,1.0);

		vec3 AnimVertex = vec3(0.0,0.0,0.0);
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			AnimVertex += (Weight[i] * (Transform[i] * v4Vertex)).xyz;
		}
		
		vec4 v4Normal = vec4(Normal,0.0);
		
		vec3 AnimNormal		= vec3(0.0,0.0,0.0);
		for(int i=0;i < MAX_WEIGHTS;i++)
		{
			AnimNormal	+= (Weight[i] * (Transform[i] * v4Normal)).xyz;
		}
		
		fVertex = (ModelMatrix*vec4(AnimVertex,1.0)).xyz;
		fNormal = NormalMatrix*AnimNormal;
		fTexCoord = TexCoord;
		gl_Position = MvpMatrix*vec4(AnimVertex,1.0);
	}
)";

const char *TestFragmentShader = R"(
	#version 330
	
	in vec3 fVertex;
	in vec3 fNormal;
	in vec2 fTexCoord;
	
	out vec4 FragColor;
	
	void main()
	{
		FragColor = vec4((1.0 + normalize(fNormal))/2,1.0);
		//FragColor = vec4(fTexCoord,0.0,1.0);
	}
)";

#endif // TEST_SHADERS_H
