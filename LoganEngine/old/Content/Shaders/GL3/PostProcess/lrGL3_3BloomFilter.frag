#version 330

uniform sampler2D lr_PrevRenderedImg;
uniform vec3 lr_MinIntensity;

in vec2 lr_fTexCoord;

layout(location = 0) out vec4 lr_FragColor;

void main()
{
	vec3 Sample = vec3(texture(lr_PrevRenderedImg,lr_fTexCoord).rgb);

	if(Sample.r < lr_MinIntensity.r)
		{Sample.r = 0.0;}
	else
		{Sample.r = 1.0;}

	if(Sample.g < lr_MinIntensity.g)
		{Sample.g = 0.0;}
	else
		{Sample.g = 1.0;}

	if(Sample.b < lr_MinIntensity.b)
		{Sample.b = 0.0;}
	else
		{Sample.b = 1.0;}

	lr_FragColor = vec4(Sample,1.0);
}
