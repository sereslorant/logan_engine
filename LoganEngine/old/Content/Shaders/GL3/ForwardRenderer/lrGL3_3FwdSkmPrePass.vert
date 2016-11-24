#version 330

const int MaxWeights = 4;

uniform mat4 lr_ModelViewProjectionMatrix;

uniform mat4 lr_BoneTransform[64];

layout(location = 0)	in vec3 lr_vPosition;

layout(location = 5)	in ivec4 lr_vBoneId;
layout(location = 6)	in vec4  lr_vWeight;

void main()
{
    mat4 lr_vTransform[MaxWeights];
    for(int i=0;i <MaxWeights;i++)
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
    for(int i=0;i < MaxWeights;i++)
	{
		lr_vPosition += (lr_vWeight[i] * (lr_vTransform[i] * v4Position)).xyz;
	}

    gl_Position = lr_ModelViewProjectionMatrix * vec4(lr_vPosition,1.0);
}
