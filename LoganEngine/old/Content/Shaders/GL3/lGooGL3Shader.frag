#version 330

in vec2 lGooTexCoord_f;

uniform sampler2D lGooImage;
uniform int lGooImgAlphaEnabled;

uniform vec4 lGooColor;
uniform int lGooImgEnabled;

layout(location = 0) out vec4 lGooFragColor;

void main()
{
    vec4 ImgColor;

    if(lGooImgAlphaEnabled == 1)
    {
        ImgColor = texture(lGooImage,lGooTexCoord_f);
    }
    else
    {
        ImgColor = vec4(texture(lGooImage,lGooTexCoord_f).rgb,1.0);
    }

    if(lGooImgEnabled == 1)
    {
        lGooFragColor = ImgColor * lGooColor;
    }
    else
    {
        lGooFragColor = lGooColor;
    }
}
