
#include "lrMultiMaterial.h"

using namespace std;

float DefDiffuse [3] = {0.0,0.0,0.0};
float DefSpecular[3] = {0.0,0.0,0.0};
float DefShininess	 = 0.0;

lrMaterial lrMultiMaterial::DefaultMaterial = lrMaterial(DefDiffuse,DefSpecular,DefShininess);

void lrMultiMaterial::SetDefaultMaterial(float *diffuse,float *specular,float shininess)
{
	DefaultMaterial = lrMaterial(diffuse,specular,shininess);
}

bool lrMultiMaterial::CheckMaterial(const std::string &Key)
{
    auto I = Materials.find(Key);
    if(I == Materials.end())
    {
        return false;
    }

    return true;
}

const liMaterial *lrMultiMaterial::GetMaterial(const string &Key) const
{
    map<string,liMaterial *>::const_iterator i = Materials.find(Key);

    if(i == Materials.end())
    {
        return &DefaultMaterial;
    }

    return i->second;
}

bool lrMultiMaterial::AddMaterial(const string &Key,liMaterial *Mtl)
{
    map<string,liMaterial *>::iterator i = Materials.find(Key);

    if(i == Materials.end())
    {
        Materials[Key] = Mtl;
        return false;
    }

    return true;
}

lrMultiMaterial::lrMultiMaterial()
{

}

lrMultiMaterial::~lrMultiMaterial()
{
    for(std::map<std::string,liMaterial *>::iterator i = Materials.begin();i != Materials.end();i++)
    {
        delete i->second;
    }
}
