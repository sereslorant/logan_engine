#version 330

const int MAX_WEIGHTS = 4;

uniform mat4 lr_ModelMatrix;
uniform mat4 lr_ViewMatrix;
uniform mat4 lr_ProjectionMatrix;
uniform mat4 lr_ModelViewMatrix;
uniform mat4 lr_ModelViewProjectionMatrix;
uniform mat3 lr_NormalMatrix;

uniform mat4 lr_BoneTransform[64];

layout(location = 0)	in vec3 lr_vPosition;
layout(location = 1)	in vec3 lr_vNormal;
layout(location = 2)	in vec3 lr_vTangent;
layout(location = 3)	in vec3 lr_vBitangent;

layout(location = 4)	in vec2 lr_vTexCoord;

layout(location = 5)	in ivec4 lr_vBoneId;
layout(location = 6)	in vec4  lr_vWeight;

out vec3 lr_fPosition;

out vec3 lr_fNormal;
out vec3 lr_fTangent;
out vec3 lr_fBitangent;

out vec2 lr_fTexCoord;

void main()
{
    mat4 lr_vTransform[MAX_WEIGHTS];
    for(int i=0;i <MAX_WEIGHTS;i++)
	{
		if(lr_vBoneId[i] > -1)
		{
			lr_vTransform[i] = lr_BoneTransform[lr_vBoneId[i]];
		}
		else
		{
			lr_vTransform[i] = mat4(0.0);
		}
	}

    vec4 v4Position = vec4(lr_vPosition,1.0);

    vec3 lr_vPosition = vec3(0.0,0.0,0.0);
    for(int i=0;i < MAX_WEIGHTS;i++)
	{
		lr_vPosition += (lr_vWeight[i] * (lr_vTransform[i] * v4Position)).xyz;
	}

	lr_fPosition = vec3((lr_ModelMatrix * vec4(lr_vPosition,1.0)).xyz);

    mat3 m3Transform[MAX_WEIGHTS];
    for(int i=0;i < MAX_WEIGHTS;i++)
	{
		m3Transform[i] = mat3(lr_vTransform[i]);
	}

    vec3 Normal		= vec3(0.0,0.0,0.0);
    vec3 Tangent	= vec3(0.0,0.0,0.0);
    vec3 Bitangent	= vec3(0.0,0.0,0.0);

	for(int i=0;i < MAX_WEIGHTS;i++)
	{
		Normal		+= (lr_vWeight[i] * (m3Transform[i] * lr_vNormal));
		Tangent		+= (lr_vWeight[i] * (m3Transform[i] * lr_vTangent));
		Bitangent	+= (lr_vWeight[i] * (m3Transform[i] * lr_vBitangent));
	}

	lr_fNormal		= lr_NormalMatrix * Normal;
	lr_fTangent		= lr_NormalMatrix * Tangent;
	lr_fBitangent	= lr_NormalMatrix * Bitangent;

    lr_fTexCoord = lr_vTexCoord;

    gl_Position = lr_ModelViewProjectionMatrix * vec4(lr_vPosition,1.0);
}
