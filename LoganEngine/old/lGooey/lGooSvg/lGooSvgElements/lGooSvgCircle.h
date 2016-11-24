#ifndef L_GOO_SVG_CIRCLE_H
#define L_GOO_SVG_CIRCLE_H

#include "lGooSvgShape.h"
#include "../lGooSvgAttributes/lGooSvgCircleAttributes.h"

class lGooSvgRoot;

struct lGooSvgCircle : public lGooSvgShape
{
private:
	lGooSvgCircleAttributes CircleAttributes;

	liGooObject *NewObject = nullptr;
	//liGooCircle *NewCircle = nullptr;

	bool LoadCircleAnim(lGooSvgAnim *anim);

protected:

	virtual lGooScalarAnimateParam::liGooScalarAnimState *LoadShapeAttribAnim(lGooSvgAnim *anim) override;

	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	//virtual void ProcessElement() override;

public:

	virtual void CreateObject(liGooObject *&obj) override;

	lGooSvgCircle(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root);
	virtual ~lGooSvgCircle();
};

#endif
