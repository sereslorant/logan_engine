
#include "lGooSvgAnim.h"

void lGooSvgAnim::LoadAttribs(const std::string &attribute,std::string &value)
{
	AnimAttributes.LoadAttribs(attribute,value);
}

void lGooSvgAnim::ProcessAttribs()
{
	AnimAttributes.Process();
}

void lGooSvgAnim::LoadChildElement(std::ifstream &file,const std::string &Tag)
{

}
/*
void lGooSvgAnim::ProcessElement()
{

}
*/
lGooSvgAnimAttributes &lGooSvgAnim::GetAnimAttributes()
{
	return AnimAttributes;
}

lGooSvgAnim::lGooSvgAnim(std::ifstream &ref_file)
	:XmlElement(ref_file,"animate")
{

}

lGooSvgAnim::~lGooSvgAnim()
{

}
