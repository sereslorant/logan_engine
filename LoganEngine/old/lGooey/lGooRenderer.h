#ifndef L_GOO_RENDERER_H
#define L_GOO_RENDERER_H

/** \brief Absztrakciós réteg, egy általános interfészt valósít meg, amin keresztül a UI-okat ki lehet rajzolni.
 * A célja az, hogy összefogja a grafikus API függvényhívásokat, így a kód független lesz a használt
 * grafikus API-tól.
 *
 * Működés: TODO
 * \todo Dokumentálni kéne
 *
 * \author Seres Lóránt Gábor
 *
 */
/*class lGooRenderer : public liGuiRenderer
{
public:
    virtual liGuiElement *AddPolygon(const lmVector2D Vrt[],unsigned int Length) = 0;

    virtual liGuiElement *GetCircle() = 0;
    virtual liGuiElement *GetRect() = 0;
    virtual liGuiElement *GetLine() = 0;
    virtual liGuiElement *GetEllipse() = 0;

    //virtual void Add(liGooObject *Obj) = 0;
    virtual void SetRenderer(liGuiDisplayList *display_list) = 0;
    virtual void Draw(liFramebuffer *framebuffer) = 0;
    //virtual void ClearDL() = 0;

    lGooRenderer(){};
    virtual ~lGooRenderer(){};
};*/

#include "lGooObject.h"

#include <list>

class lGooContainer : public lGooObject, public liGooContainer
{
private:
	std::list<liGooObject *> DisplayList;
	std::map<std::string,liGooObject *> ObjIds;

public:

	virtual void AddObject(liGooObject *Obj) override
	{
		Obj->SetParent(this);
		DisplayList.push_back(Obj);
	}

	virtual void AddObjectId(const std::string &id,liGooObject *obj) override
	{
		auto I = ObjIds.find(id);

    	if(I == ObjIds.end())
		{
			ObjIds[id] = obj;
		}
	}

    virtual liGooObject *GetObject(const std::string &id) override
    {
    	auto I = ObjIds.find(id);

    	if(I == ObjIds.end())
		{
			return nullptr;
		}

		return I->second;
    }

    virtual void AddChildrenToGUI(liGooGUI *gui) override
    {
    	for(liGooObject *Obj : DisplayList)
		{
			gui->AddObject(Obj);
		}
    }
	/*
	virtual void AddToRenderer(lGooRenderer *Renderer) override
	{
		for(liGooObject *I : DisplayList)
		{
			((lGooObject *)I)->AddToRenderer(Renderer);
		}
	}
	*/

	virtual lmMatrix3x3 Transform() const override
	{
        lmMatrix3x3 mTranslate  = lmTranslate3x3(Position);
        lmMatrix3x3 mRotate     = lmRotate3x3(lmDegToRad(Orientation));
        lmMatrix3x3 mScale      = lmScale3x3(Scale);

        if(Parent != nullptr)
		{
			return Parent->Transform() * mTranslate * mRotate * mScale;
		}
		else
		{
			return mTranslate * mRotate * mScale;
		}
	}

    virtual void Step(lmScalar dt) override
    {
    	lGooObject::Step(dt);

		for(liGooObject *I : DisplayList)
		{
			I->Step(dt);
		}
    }

    virtual liGooContainer *ToContainer() override {return this;}

    lGooContainer()
		:lGooObject(nullptr)
	{}

	lGooContainer(const lmVector2D &position,const lmVector2D &scale,lmScalar orientation)
		:lGooObject(position,scale,orientation,nullptr)
	{

	}

	virtual ~lGooContainer() override
	{
		for(liGooObject *I : DisplayList)
		{
			delete I;
		}
	}
};

class lGooGUI : public lGooObject, public liGooGUI
{
private:
	liGuiDisplayList *DisplayList;

	struct lGooDLElement
	{
		liGooObject *Object;
		liGuiDisplayListNode *FillNode;
		liGuiDisplayListNode *OutlineNode;
	};

	std::list<liGooObject *> Children;

	std::list<lGooDLElement> Objects;
	std::map<std::string,liGooObject *> ObjIds;

	unsigned int ResX = 0;
	unsigned int ResY = 0;

	bool BgColorEnabled = false;
	lGooColor BgColor;

	class lGooDocBeginCallback : public liEventCallback
	{
	private:
		lGooGUI *GUI;

	public:
		lGooDocBeginCallback(lGooGUI *gui)
			:GUI(gui)
		{

		}

		virtual ~lGooDocBeginCallback()
		{

		}

		virtual void Callback() override
		{
			GUI->DocBeginEvent.Fire();
		}
	};

	lGooEvent DocBeginEvent;

	lGooDocBeginCallback DocBeginCallback;

public:

	virtual lGooEvent &GetDocBeginEvent() override
	{
		return DocBeginEvent;
	}

	virtual const lGooEvent &GetDocBeginEvent() const override
	{
		return DocBeginEvent;
	}

	virtual liEventCallback *GetDocBeginCallback() override
	{
		return &DocBeginCallback;
	}

	virtual unsigned int GetResX() const override
	{
		return ResX;
	}

	virtual unsigned int GetResY() const override
	{
		return ResY;
	}

	virtual void SetResX(unsigned int res_x) override
	{
		ResX = res_x;
		DisplayList->SetWidth(ResX);
	}

	virtual void SetResY(unsigned int res_y) override
	{
		ResY = res_y;
		DisplayList->SetHeight(ResY);
	}

	virtual bool GetBgColorEnabled() const override
	{
		return BgColorEnabled;
	}

	virtual const liGooColor &GetBgColor() const override
	{
		return BgColor;
	}

	virtual void SetBgColorEnabled(bool bg_color_enabled) override
	{
		BgColorEnabled = bg_color_enabled;
		DisplayList->SetBgColorEnabled(BgColorEnabled);
	}

	virtual void SetBgColor(const liGooColor &bg_color) override
	{
		BgColor = bg_color;
		DisplayList->SetBgColor(BgColor);
	}

	virtual void AddChild(liGooObject *obj) override
	{
		AddObject(obj);
		Children.push_back(obj);

		liGooContainer *ObjContainer = obj->ToContainer();
		if(ObjContainer != nullptr)
		{
			ObjContainer->AddChildrenToGUI(this);
		}
	}

	virtual void AddObject(liGooObject *Obj) override
	{
		//lGooContainer::AddObject(Obj);

		if(Obj->IsTextured())
		{
			liGuiDisplayListNode *FillNode = DisplayList->CreateTexturedListNode(Obj->GetTexImage());
			FillNode->SetGuiElement(Obj->GetPolygon());
			FillNode->SetBrush(Obj->GetFillBrush());

			liGuiDisplayListNode *OutlineNode = nullptr;
			if(Obj->HasOutline())
			{
				OutlineNode = DisplayList->CreateNonTexturedListNode();
				OutlineNode->SetGuiElement(Obj->GetPolygon());
				OutlineNode->SetBrush(Obj->GetFillBrush());
			}

			Objects.push_back({Obj,FillNode,OutlineNode});
		}
		else
		{
			liGuiDisplayListNode *FillNode = DisplayList->CreateNonTexturedListNode();

			FillNode->SetGuiElement(Obj->GetPolygon());
			FillNode->SetBrush(Obj->GetFillBrush());

			liGuiDisplayListNode *OutlineNode = nullptr;
			if(Obj->HasOutline())
			{
				OutlineNode = DisplayList->CreateNonTexturedListNode();

				OutlineNode->SetGuiElement(Obj->GetPolygon());
				OutlineNode->SetBrush(Obj->GetFillBrush());
			}

			Objects.push_back({Obj,FillNode,OutlineNode});
		}
	}

	virtual void AddObjectId(const std::string &id,liGooObject *obj)
	{
		ObjIds[id] = obj;
	}

	virtual liGuiDisplayList *GetDisplayList()
	{
		return DisplayList;
	}

    virtual void Step(lmScalar dt) override
    {
    	//lGooContainer::Step(dt);

    	for(liGooObject *Child : Children)
		{
			Child->Step(dt);
		}

    	DocBeginEvent.Update(dt);

    	for(auto &I : Objects)
		{
			I.FillNode->SetTransformation(I.Object->Transform());

			if(I.OutlineNode != nullptr)
			{
				I.OutlineNode->SetTransformation(I.Object->Transform());
			}
		}
    }

	virtual lmMatrix3x3 Transform() const override
	{
        lmMatrix3x3 mTranslate  = lmTranslate3x3(Position);
        lmMatrix3x3 mRotate     = lmRotate3x3(lmDegToRad(Orientation));
        lmMatrix3x3 mScale      = lmScale3x3(Scale);

        if(Parent != nullptr)
		{
			return Parent->Transform() * mTranslate * mRotate * mScale;
		}
		else
		{
			return mTranslate * mRotate * mScale;
		}
	}

    virtual liGooGUI *ToGUI(){return this;}

	lGooGUI(liGuiDisplayList *display_list/*const lmVector2D &position,const lmVector2D &scale,lmScalar orientation,float res_x,float res_y*/)
		:lGooObject(nullptr),DisplayList(display_list),DocBeginCallback(this)//:lGooContainer(position,scale,orientation),ResX(res_x),ResY(res_y),BgColorEnabled(false),BgColor(0.0,0.0,0.0,1.0),DocBeginCallback(this)
	{

	}

	virtual ~lGooGUI() override
	{

	}
};

#endif
