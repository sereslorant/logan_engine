#ifndef L_GOO_SVG_LINE_H
#define L_GOO_SVG_LINE_H

#include "lGooSvgShape.h"
#include "../lGooSvgAttributes/lGooSvgLineAttributes.h"

class lGooSvgRoot;

struct lGooSvgLine : public lGooSvgShape
{
private:
	lGooSvgLineAttributes LineAttributes;

	liGooObject *NewObject = nullptr;
	//liGooLine *NewLine = nullptr;

	bool LoadLineAnim(lGooSvgAnim *anim);

protected:

	virtual lGooScalarAnimateParam::liGooScalarAnimState *LoadShapeAttribAnim(lGooSvgAnim *anim) override;

	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	//virtual void ProcessElement() override;

public:
	virtual void CreateObject(liGooObject *&obj) override;

	lGooSvgLine(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root);
	virtual ~lGooSvgLine();
};

#endif // L_GOO_SVG_LINE_H
