#version 330

uniform sampler2D lr_PrevRenderedImg;

in vec2 lr_fTexCoord;

layout(location = 0) out vec4 lr_FragColor;

void main()
{
	lr_FragColor = texture(lr_PrevRenderedImg,lr_fTexCoord);
}

