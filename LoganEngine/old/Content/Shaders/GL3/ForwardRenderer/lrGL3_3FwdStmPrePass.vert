#version 330

uniform mat4 lr_ModelViewProjectionMatrix;

layout(location = 0) in vec3 lr_vPosition;

//out vec3 lr_fPosition;

void main()
{
	/*
    lr_fPosition = vec3((lr_ModelMatrix * vec4(lr_vPosition,1.0)).xyz);

    lr_fNormal      = lr_NormalMatrix * lr_vNormal;
    lr_fTangent     = lr_NormalMatrix * lr_vTangent;
    lr_fBitangent   = lr_NormalMatrix * lr_vBitangent;

    lr_fTexCoord = lr_vTexCoord;
	*/

    gl_Position = lr_ModelViewProjectionMatrix * vec4(lr_vPosition,1.0);
}
