#version 330

uniform sampler2D lr_PrevRenderedImg;
uniform vec2 lr_PixelSize;
uniform int lr_IsVertical;

in vec2 lr_fTexCoord;

layout(location = 0) out vec4 lr_FragColor;

void main()
{
	//A súlyok Gauss eloszlást kéne, hogy kövessenek kb, és a súlyok összege 1.0 kell, hogy legyen.
	//Egyelőre csak random számokkal helyettesítjük a Gauss eloszlást.
	float Weights[9];// = {0.05,0.09,0.11,0.15,0.2,0.15,0.11,0.09,0.05}; SHADER COMPILE ERRORT AD NVIDIA ALATT!!!

	Weights[0] = 0.05;
	Weights[1] = 0.09;
	Weights[2] = 0.11;
	Weights[3] = 0.15;
	Weights[4] = 0.2;
	Weights[5] = 0.15;
	Weights[6] = 0.11;
	Weights[7] = 0.09;
	Weights[8] = 0.05;

	vec4 Result;

	if(lr_IsVertical == 1)
	{
		vec2 CurrentPosition = lr_fTexCoord - vec2(0.0,4*lr_PixelSize.y);

		for(int i=0;i < 9;i++)
		{
			Result += texture(lr_PrevRenderedImg,CurrentPosition) * Weights[i];

			CurrentPosition.y += lr_PixelSize.y;
		}
	}
	else
	{
		vec2 CurrentPosition = lr_fTexCoord - vec2(4*lr_PixelSize.x,0.0);

		for(int i=0;i < 9;i++)
		{
			Result += texture(lr_PrevRenderedImg,CurrentPosition) * Weights[i];

			CurrentPosition.x += lr_PixelSize.x;
		}
	}

	lr_FragColor = Result;
}
