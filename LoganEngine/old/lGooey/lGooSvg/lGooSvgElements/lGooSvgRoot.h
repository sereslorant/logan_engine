#ifndef L_GOO_SVG_ROOT_H
#define L_GOO_SVG_ROOT_H

#include "lGooSvgShape.h"
#include "../lGooSvgAttributes/lGooSvgRectAttributes.h"

struct lGooSvgRoot : public lGooSvgShape
{
private:
	lGooSvgRectAttributes SvgAttributes;

	typedef lGooSvgShape *(lGooSvgRoot::* ShapeElementLoader)(std::ifstream &file);

	lGooSvgShape *LoadRect(std::ifstream &file);

	lGooSvgShape *LoadCircle(std::ifstream &file);

	lGooSvgShape *LoadLine(std::ifstream &file);

	lGooSvgShape *LoadEllipse(std::ifstream &file);

	lGooSvgShape *LoadSvg(std::ifstream &file)
	{
		std::cout << "Svg found!" << std::endl;

		lGooSvgRoot *SvgRoot = new lGooSvgRoot(file,ObjectFactory);

		return SvgRoot;
	}

	ShapeElementLoader ElementLoader[5] = { &lGooSvgRoot::LoadRect,
											&lGooSvgRoot::LoadCircle,
											&lGooSvgRoot::LoadLine,
											&lGooSvgRoot::LoadEllipse,
											//Ennek mindig az utolsónak kell lennie!
											&lGooSvgRoot::LoadSvg,
											};

protected:

	virtual lGooScalarAnimateParam::liGooScalarAnimState *LoadShapeAttribAnim(lGooSvgAnim *anim) override
	{
		return nullptr;
	}

	virtual void LoadAttribs(const std::string &attribute,std::string &value) override;
	virtual void ProcessAttribs() override;
	virtual void LoadChildElement(std::ifstream &file,const std::string &Tag) override;

	void SetAnimTriggers(liGooGUI *Svg);

	struct svgAnimAnyad
	{
		lGooSvgAnim	  *SvgAnim;
		lGooAnimation *Animation;

		svgAnimAnyad(lGooSvgAnim *svg_anim,lGooAnimation *animation)
		{
			SvgAnim   = svg_anim;
			Animation = animation;
		}
	};

public:

	std::list<lGooSvgShape *> Shapes;
	//std::map<std::string,lGooSvgShape *> ShapeIds;

	std::list<svgAnimAnyad> Animations;
	std::map<std::string,lGooAnimation *> AnimIds;

	virtual void CreateObject(liGooObject *&obj) override
	{
		/*
		liGooObject *Svg = ObjectFactory->NewContainer();//new lGooGUI()
		liGooContainer *SvgContainer = Svg->ToContainer();

		BgRect->SetWidth(SvgAttributes.Attr_width);
		BgRect->SetHeight(SvgAttributes.Attr_height);
		*/

		liGooObject *Svg = ObjectFactory->NewGUI();
		Svg->SetPosition({SvgAttributes.Attr_x,SvgAttributes.Attr_y});

		liGooGUI *SvgGui = Svg->ToGUI();

		SvgGui->SetResX(SvgAttributes.Attr_width);
		SvgGui->SetResY(SvgAttributes.Attr_height);

		for(auto I : Shapes)
		{
			liGooObject *obj;
			I->CreateObject(obj);

			SvgGui->AddObject(obj);

			if(I->HasId)
			{
				SvgGui->AddObjectId(I->Id,obj);
			}

			SetAnimTriggers(SvgGui);
		}

		obj = Svg;
	}

	void CreateSvg(liGooGUI *&dest)
	{
		liGooObject *Svg;
		CreateObject(Svg);

		dest = Svg->ToGUI();
	}

	lGooSvgRoot(std::ifstream &ref_file,liGooObjectFactory *object_factory,lGooSvgRoot *svg_root = nullptr);
	virtual ~lGooSvgRoot();
};

#endif // L_GOO_SVG_ROOT_H
