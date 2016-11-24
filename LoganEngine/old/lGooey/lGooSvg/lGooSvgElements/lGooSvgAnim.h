#ifndef L_GOO_SVG_ANIM_H
#define L_GOO_SVG_ANIM_H

#include "../XmlElement.h"

#include "../lGooSvgAttributes/lGooSvgAnimAttributes.h"

struct lGooSvgAnim : public XmlElement
{
private:
	lGooSvgAnimAttributes AnimAttributes;

protected:
	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	virtual void LoadChildElement(std::ifstream &file,const std::string &Tag) override;
	//virtual void ProcessElement() override;

public:
	const std::list<std::string> GetBegin()
	{
		return AnimAttributes.Attr_begin;
	}

	const std::list<std::string> GetEnd()
	{
		return AnimAttributes.Attr_end;
	}

	lGooSvgAnimAttributes &GetAnimAttributes();

	lGooSvgAnim(std::ifstream &ref_file);
	virtual ~lGooSvgAnim();
};

#endif
