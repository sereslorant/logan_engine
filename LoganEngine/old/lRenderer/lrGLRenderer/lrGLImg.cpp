
#include <iostream>

#include "lrGLImg.h"

using namespace std;

bool lrGLImg::CheckError()
{
    if(Id == 0)
    {
        return false;
    }

    return true;
}

GLuint lrGLImg::GetId()
{
    return Id;
}

lrGLImg::lrGLImg(unsigned int width,unsigned int height,bool alphaBlending,GLuint id):lrImg(width,height,alphaBlending),Id(id)
{

}

lrGLImg::~lrGLImg()
{
    glDeleteTextures(1,&Id);
}
