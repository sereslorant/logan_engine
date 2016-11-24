#ifndef L_GOO_SVG_SHAPE_H
#define L_GOO_SVG_SHAPE_H

#include "../../lGooey.h"

#include "../XmlElement.h"

#include "../../lGooAnimation/lGooAnimate/lGooAnimate.h"
#include "../../lGooAnimation/lGooAnimate/lGooScalarAnimate.h"

#include "../lGooSvgAttributes/lGooSvgPaintingAttributes.h"
#include "lGooSvgAnim.h"

struct lGooSvgRoot;

struct lGooSvgShape : public XmlElement
{
protected:
	liGooObjectFactory *ObjectFactory;

	lGooSvgRoot  *SvgRoot;
	lGooSvgPaintingAttributes PaintingAttributes;

	std::list<lGooSvgAnim *> Animates;

	virtual lGooScalarAnimateParam::liGooScalarAnimState *LoadShapeAttribAnim(lGooSvgAnim *anim) = 0;

	lGooAnimate::liGooAnimateParam *CreateScalarAnimate(lGooSvgAnim *anim,lmScalar duration);

	bool LoadAnim(lGooSvgAnim *anim,liGooObject *object);

	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	virtual void LoadChildElement(std::ifstream &file,const std::string &Tag) override;

public:

	virtual void CreateObject(liGooObject *&obj) = 0;

	lGooSvgShape(std::ifstream &ref_file,const std::string &xml_tag,lGooSvgRoot *svg_root,liGooObjectFactory *object_factory);
	virtual ~lGooSvgShape();
};

#endif // L_GOO_SVG_SHAPE_H
