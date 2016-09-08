#ifndef LI_3D_SCENE_H
#define LI_3D_SCENE_H

#include "../../../lMath/lMath.h"

#include "li3DCamera.h"

#include "li3DElement.h"

/*
 * TMP!!!!!!!!!!!!!!!!
 */
class li3DSceneDrawer
{
public:
	//
	virtual void Draw(const li3DElement &element) = 0;
	//
	li3DSceneDrawer(){}
	virtual ~li3DSceneDrawer(){}
};

/*
 * TODO: Kurvasok kódduplikálás lesz a 2D-s színtérrel, esélyes, hogy refaktorálni kell!
 */
class li3DScene
{
public:
	//
	virtual bool HasBackgroundColor() const = 0;
	virtual const liColor &GetBackgroundColor() const = 0;
	//
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	virtual void Draw(li3DSceneDrawer &scene_visitor) = 0;
	//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//
	li3DScene(){}
	virtual ~li3DScene(){}
};


#endif // LI_3D_SCENE_H
