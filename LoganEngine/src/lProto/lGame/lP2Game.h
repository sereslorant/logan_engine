#ifndef LP2_GAME_H
#define LP2_GAME_H

#include "../../lRenderer/lr2DScene/lr2DScene.h"
#include "../../lRenderer/lr3DScene/lr3DScene.h"
#include "../../lInterfaces/lRenderer/li2DRenderer.h"
#include "../../lGame/lGame.h"
#include "liWorld2D.h"

#include <list>

class lP2ProtoGameMode : public liGameMode
{
protected:
	//
	class Set2DLayer : public liLayerVisitor
	{
	private:
		li2DScene *Scene;
		li2DCamera *Camera;

	public:

		virtual void Visit2DLayer(li2DLayer &layer) override
		{
			layer.SetScene(Scene);
			layer.SetCamera(Camera);
		}

		virtual void Visit3DLayer(li3DLayer &layer) override
		{
			//Üres
		}

		Set2DLayer(li2DScene *scene,li2DCamera *camera)
			:Scene(scene),Camera(camera)
		{}

		virtual ~Set2DLayer() override {}
	};
	//
	class Set3DLayer : public liLayerVisitor
	{
	private:
		//li3DScene *Scene;
		liFrustum *Frustum;
		li3DCamera *Camera;

	public:

		virtual void Visit2DLayer(li2DLayer &layer) override
		{
			//Üres
		}

		virtual void Visit3DLayer(li3DLayer &layer) override
		{
			//layer.SetScene(Scene);
			layer.SetFrustum(Frustum);
			layer.SetCamera(Camera);
		}

		Set3DLayer(/*li3DScene *scene,*/liFrustum *frustum,li3DCamera *camera)
			:/*Scene(scene),*/Frustum(frustum),Camera(camera)
		{}

		virtual ~Set3DLayer() override
		{}
	};
	//
	liInput &Input;
	//
	liWorld2D &World;
	liRenderer &Renderer;
	//
	std::list<liAgent *> Agents;
	//
public:
	//
	virtual void Logic(double dt) override
	{
		for(auto I = Agents.begin();I != Agents.end();)
		{
			if((*I)->IsRemovable())
			{
				auto J = I;
				I++;
				Agents.erase(J);
			}
			else
			{
				//liAgent *Agent = *I;
				(*I)->Update(dt);
				I++;
			}
		}
	}
	//
	lP2ProtoGameMode(liInput &input,liWorld2D &world,liRenderer &renderer)
		:Input(input),World(world),Renderer(renderer)
	{

	}
	//
	virtual ~lP2ProtoGameMode() override
	{
		for(liAgent *Agent : Agents)
		{
			delete Agent;
		}
	}
};

#endif //LP2_GAME_H

