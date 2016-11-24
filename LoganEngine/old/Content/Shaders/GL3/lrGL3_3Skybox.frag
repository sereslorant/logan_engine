#version 330

uniform samplerCube lr_SkyboxImg;

in vec3 lr_fPosition;

layout(location = 0) out vec4 lr_FragColor;

void main()
{
	lr_FragColor = vec4(texture(lr_SkyboxImg,lr_fPosition).rgb,1.0);
}
