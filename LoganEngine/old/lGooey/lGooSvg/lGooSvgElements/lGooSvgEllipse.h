#ifndef L_GOO_SVG_ELLIPSE_H
#define L_GOO_SVG_ELLIPSE_H

#include "lGooSvgShape.h"
#include "../lGooSvgAttributes/lGooSvgEllipseAttributes.h"

class lGooSvgRoot;

struct lGooSvgEllipse : public lGooSvgShape
{
private:
	lGooSvgEllipseAttributes EllipseAttributes;

	liGooObject *NewObject = nullptr;
	//liGooEllipse *NewEllipse = nullptr;

	bool LoadEllipseAnim(lGooSvgAnim *anim);

protected:

	virtual lGooScalarAnimateParam::liGooScalarAnimState *LoadShapeAttribAnim(lGooSvgAnim *anim) override;

	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	//virtual void ProcessElement() override;

public:
	virtual void CreateObject(liGooObject *&obj) override;

	lGooSvgEllipse(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root);
	virtual ~lGooSvgEllipse();
};

#endif // L_GOO_SVG_ELLIPSE_H
