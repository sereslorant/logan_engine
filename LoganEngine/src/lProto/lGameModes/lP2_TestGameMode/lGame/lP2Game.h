#ifndef LP2_GAME_H
#define LP2_GAME_H

#include "liWorld2D.h"

#include <list>
#include "../../../../lInterfaces/lGame/liGame.h"
#include "../../../../lInterfaces/lGame/liAgent.h"
#include "../../../../lInterfaces/lRenderer/liRenderer.h"
#include "../../../../lScene/lr2DScene/lr2DScene.h"
#include "../../../../lScene/lr3DScene/lr3DScene.h"

class lP2ProtoGameMode : public liGameMode
{
protected:
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

