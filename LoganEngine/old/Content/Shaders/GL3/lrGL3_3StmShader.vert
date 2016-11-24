#version 330

uniform mat4 lr_ModelMatrix;
uniform mat4 lr_ViewMatrix;
uniform mat4 lr_ProjectionMatrix;
uniform mat4 lr_ModelViewMatrix;
uniform mat4 lr_ModelViewProjectionMatrix;
uniform mat3 lr_NormalMatrix;

layout(location = 0) in vec3 lr_vPosition;
layout(location = 1) in vec3 lr_vNormal;
layout(location = 2) in vec3 lr_vTangent;
layout(location = 3) in vec3 lr_vBitangent;
layout(location = 4) in vec2 lr_vTexCoord;

out vec3 lr_fPosition;

out vec3 lr_fNormal;
out vec3 lr_fTangent;
out vec3 lr_fBitangent;

out vec2 lr_fTexCoord;

void main()
{
    lr_fPosition = vec3((lr_ModelMatrix * vec4(lr_vPosition,1.0)).xyz);

    lr_fNormal      = lr_NormalMatrix * lr_vNormal;
    lr_fTangent     = lr_NormalMatrix * lr_vTangent;
    lr_fBitangent   = lr_NormalMatrix * lr_vBitangent;

    lr_fTexCoord = lr_vTexCoord;

    /*lr_ProjectionMatrix * lr_ViewMatrix * lr_ModelMatrix*/
    gl_Position = lr_ModelViewProjectionMatrix * vec4(lr_vPosition,1.0);
    //gl_Position = lr_ProjectionMatrix * lr_ViewMatrix * lr_ModelMatrix * vec4(lr_vPosition,1.0);
}
