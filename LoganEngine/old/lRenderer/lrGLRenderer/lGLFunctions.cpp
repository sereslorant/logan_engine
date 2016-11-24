
#include "lGLFunctions.h"

GLuint lGL_CreateEmptyTexture(unsigned int width,unsigned int height,bool is_depth_texture)
{
	GLuint Id;
    glGenTextures(1, &Id);
    glBindTexture(GL_TEXTURE_2D, Id);

    GLenum InternalFormat;
    GLenum Format;
    if(is_depth_texture)
	{
		InternalFormat	= GL_DEPTH_COMPONENT;
		Format			= GL_DEPTH_COMPONENT;
	}
	else
	{
		InternalFormat	= GL_RGBA8;
		Format			= GL_RGBA;
	}

    glTexImage2D(GL_TEXTURE_2D,0,InternalFormat,width,height,0,Format,GL_FLOAT,nullptr);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    return Id;
}

void lGL_GetImageInfo(liBitmap *Bmp,GLenum *InternalFormat,GLenum *Type,bool *AlphaBlending)
{
    switch(Bmp->GetPixelFormat())
    {
    case liBitmap::PF_R8_G8_B8:
        *InternalFormat = GL_BGR;
        *Type = GL_UNSIGNED_BYTE;
        *AlphaBlending = false;
        break;

    case liBitmap::PF_B8_G8_R8:
        *InternalFormat = GL_RGB;
        *Type = GL_UNSIGNED_BYTE;
        *AlphaBlending = false;
        break;

    case liBitmap::PF_LUT:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_BYTE;
        *AlphaBlending = false;
        break;

        //16
        //5_6_5
    case liBitmap::PF_B5_G6_R5:
        *InternalFormat = GL_RGB;
        *Type = GL_UNSIGNED_SHORT_5_6_5;
        *AlphaBlending = false;
        break;

    case liBitmap::PF_R5_G6_B5:
        *InternalFormat = GL_RGB;
        *Type = GL_UNSIGNED_SHORT_5_6_5_REV;
        *AlphaBlending = false;
        break;

        //4_4_4_4
    case liBitmap::PF_A4_B4_G4_R4:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_SHORT_4_4_4_4;
        *AlphaBlending = true;
        break;

    case liBitmap::PF_A4_R4_G4_B4:
        *InternalFormat = GL_BGRA;
        *Type = GL_UNSIGNED_SHORT_4_4_4_4;
        *AlphaBlending = true;
        break;

        //1_5_5_5
    case liBitmap::PF_B5_G5_R5_A1:
        *InternalFormat = GL_BGRA;
        *Type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        *AlphaBlending = true;
        break;

    case liBitmap::PF_B5_G5_R5_X1:
        *InternalFormat = GL_BGRA;
        *Type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        *AlphaBlending = false;
        break;

    case liBitmap::PF_R5_G5_B5_A1:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        *AlphaBlending = true;
        break;

    case liBitmap::PF_R5_G5_B5_X1:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
        *AlphaBlending = false;
        break;

        //32
        //2_10_10_10
    case liBitmap::PF_A2_B10_G10_R10:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_INT_10_10_10_2;
        *AlphaBlending = true;
        break;

    case liBitmap::PF_A2_R10_G10_B10:
        *InternalFormat = GL_BGRA;
        *Type = GL_UNSIGNED_INT_10_10_10_2;
        *AlphaBlending = true;
        break;

        //8_8_8_8
    case liBitmap::PF_A8_B8_G8_R8:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_INT_8_8_8_8;
        *AlphaBlending = true;
        break;

    case liBitmap::PF_X8_B8_G8_R8:
        *InternalFormat = GL_RGBA;
        *Type = GL_UNSIGNED_INT_8_8_8_8;
        *AlphaBlending = false;
        break;

    case liBitmap::PF_A8_R8_G8_B8:
        *InternalFormat = GL_BGRA;
        *Type = GL_UNSIGNED_INT_8_8_8_8;
        *AlphaBlending = true;
        break;

    case liBitmap::PF_X8_R8_G8_B8:
        *InternalFormat = GL_BGRA;
        *Type = GL_UNSIGNED_INT_8_8_8_8;
        *AlphaBlending = false;
        break;

    }
}
