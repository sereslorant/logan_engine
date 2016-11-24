#version 330

//uniform mat4 lr_ModelViewProjectionMatrix;

layout(location = 0) in vec3 lr_vPosition;
layout(location = 1) in vec2 lr_vTexCoord;

out vec2 lr_fTexCoord;

void main()
{
    lr_fTexCoord = lr_vTexCoord;
    gl_Position	 = vec4(lr_vPosition,1.0);
}

