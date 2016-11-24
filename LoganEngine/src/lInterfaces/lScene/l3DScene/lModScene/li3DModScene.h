#ifndef LI_3D_MOD_SCENE_H
#define LI_3D_MOD_SCENE_H

#include "../li3DScene.h"

#include "li3DModElement.h"

class li3DModScene : public li3DScene
{
public:
	//
	class liElementFactory
	{
	public:
		//
		virtual li3DModElement *CreateStaticMesh(const lmVector3D &position,const lmQuaternion &orientation,const lmVector3D &scale,const liMaterialLibrary &mtl_lib,const std::string &model_name) = 0;
		virtual li3DModElement *CreatePointLight(const lmVector3D &position) = 0;
		//
		liElementFactory(){}
		virtual ~liElementFactory(){}
	};
	//
	virtual void SetBackgroundColor(const liColor &bg_color) = 0;
	virtual void ClearBackgroundColor() = 0;
	//
	virtual liElementFactory &GetElementFactory() = 0;
	//
	li3DModScene(){}
	virtual ~li3DModScene() override{}
};


#endif // LI_3D_MOD_SCENE_H
