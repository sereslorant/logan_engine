#ifndef L_GOO_SVG_RECT_H
#define L_GOO_SVG_RECT_H

#include "lGooSvgShape.h"
#include "../lGooSvgAttributes/lGooSvgRectAttributes.h"

class lGooSvgRoot;

struct lGooSvgRect : public lGooSvgShape
{
private:
	lGooSvgRectAttributes RectAttributes;

	liGooObject *NewObject = nullptr;

	bool LoadRectAnim(lGooSvgAnim *anim);

protected:

	virtual lGooScalarAnimateParam::liGooScalarAnimState *LoadShapeAttribAnim(lGooSvgAnim *anim) override;

	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	//virtual void ProcessElement() override;

public:
	virtual void CreateObject(liGooObject *&obj) override;

	lGooSvgRect(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root);
	virtual ~lGooSvgRect();
};

#endif
