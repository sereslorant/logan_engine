#ifndef LR_3D_SCENE_H
#define LR_3D_SCENE_H

#include <cmath>

#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModElement.h"
#include "../../lScene/lr3DScene/lr3DCamera.h"


class lr3DElement : public li3DModElement
{
protected:
	bool Hidden = false;
	//
public:
	//
	virtual void Show() override
	{
		Hidden = false;
	}
	//
	virtual void Hide() override
	{
		Hidden = true;
	}
	//
	virtual bool IsHidden() const override
	{
		return Hidden;
	}
	//
	lr3DElement()
	{}
	//
	virtual ~lr3DElement() override
	{}
};


#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModMesh.h"

#include "../../lScene/lr3DScene/lrMaterialLibrary.h"

class lr3DMesh : public lr3DElement, public li3DModMesh
{
protected:
	lmVector3D Position;
	lmQuaternion Quaternion;
	lmVector3D Scale;
	//
	lrMaterialLibrary MaterialLibrary;
	//
	bool Hidden = false;
	//
public:
	//
	virtual void SetPosition(const lmVector3D &position) override
	{
		Position = position;
	}
	//
	virtual void SetOrientation(const lmQuaternion &quaternion) override
	{
		Quaternion = quaternion;
	}
	//
	virtual void SetScale(const lmVector3D &scale) override
	{
		Scale = scale;
	}
	//
	virtual void SetMaterialLibrary(const liMaterialLibrary &material_library) override
	{
		MaterialLibrary.Assign(material_library);
	}
	//
	virtual void Accept(li3DModElementVisitor &visitor) override
	{
		visitor.VisitMesh(*this);
	}
	//
	virtual const lmVector3D &GetPosition() const override
	{
		return Position;
	}
	//
	virtual const lmQuaternion &GetOrientation() const override
	{
		return Quaternion;
	}
	//
	virtual const lmVector3D &GetScale() const override
	{
		return Scale;
	}
	//
	virtual void Accept(li3DElementVisitor &visitor) const override
	{
		visitor.VisitMesh(*this);
	}
	//
	virtual const liMaterialLibrary &GetMaterialLibrary() const override
	{
		return MaterialLibrary;
	}
	//
	lr3DMesh(const lmVector3D &position,const lmQuaternion &quaternion,const lmVector3D &scale)
		:Position(position),Quaternion(quaternion),Scale(scale)
	{}
	//
	virtual ~lr3DMesh() override
	{}
};

class lr3DStaticMesh : public lr3DMesh, public li3DModStaticMesh
{
protected:
	std::string ModelName;
	//
public:
	//
	virtual void SetModelname(std::string &model_name) override
	{
		ModelName = model_name;
	}
	//
	virtual void Accept(li3DModMeshVisitor &visitor) override
	{
		visitor.VisitStaticMesh(*this);
	}
	//
	virtual const std::string &GetModelName() const override
	{
		return ModelName;
	}
	//
	virtual void Accept(li3DMeshVisitor &visitor) const override
	{
		visitor.VisitStaticMesh(*this);
	}
	//
	lr3DStaticMesh(const lmVector3D &position,const lmQuaternion &quaternion,const lmVector3D &scale,const std::string &model_name)
		:lr3DMesh(position,quaternion,scale),ModelName(model_name)
	{}
	//
	virtual ~lr3DStaticMesh() override
	{}
};

#include <list>

#include "../../lInterfaces/lScene/l3DScene/lModScene/li3DModScene.h"

#include "../../lScene/lrColor.h"

class lr3DScene : public li3DModScene
{
protected:
	//
	class lr3DElementFactory : public liElementFactory
	{
	private:
		lr3DScene &Scene;
		//
	public:
		//
		virtual li3DModElement *CreateStaticMesh(const lmVector3D &position,const lmQuaternion &quaternion,const lmVector3D &scale,const liMaterialLibrary &mtl_lib,const std::string &model_name) override
		{
			lr3DStaticMesh *StaticMesh = new lr3DStaticMesh(position,quaternion,scale,model_name);
			StaticMesh->SetMaterialLibrary(mtl_lib);
			//
			Scene.Elements.push_back(StaticMesh);
			//
			return StaticMesh;
		}
		//
		lr3DElementFactory(lr3DScene &scene)
			:Scene(scene)
		{}
		//
		virtual ~lr3DElementFactory()
		{}
	};
	//
	bool HasBackground;
	lrColor BgColor;
	//
	std::list<lr3DElement *> Elements;
	//
	lr3DElementFactory ElementFactory;
	//
public:
	//
	virtual bool HasBackgroundColor() const override
	{
		return HasBackground;
	}
	//
	virtual const liColor &GetBackgroundColor() const override
	{
		return BgColor;
	}
	//
	virtual void SetBackgroundColor(const liColor &bg_color) override
	{
		BgColor = bg_color;
		HasBackground = true;
	}
	//
	virtual void ClearBackgroundColor() override
	{
		HasBackground = false;
	}
	//
	virtual liElementFactory &GetElementFactory() override
	{
		return ElementFactory;
	}
	//
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void Draw(li3DSceneDrawer &scene_drawer) override
	{
		for(lr3DElement *Element : Elements)
		{
			scene_drawer.Draw(*Element);
		}
	}
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	lr3DScene()
		:HasBackground(false),BgColor(0.0,0.0,0.0,0.0),ElementFactory(*this)
	{
		//
	}
	//
	virtual ~lr3DScene()
	{
		for(lr3DElement *Element : Elements)
		{
			delete Element;
		}
	}
};

#endif // LR_3D_SCENE_H
