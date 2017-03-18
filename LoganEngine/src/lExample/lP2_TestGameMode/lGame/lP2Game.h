#ifndef LP2_GAME_H
#define LP2_GAME_H

#include "liWorld2D.h"

#include <list>

#include <lGame/liGame.h>

#include <lApiAdapter/liInput.h>
#include <lRenderer/liRenderer.h>

class liAgent
{
public:
	virtual void MarkRemovable() = 0;
	virtual bool IsRemovable() = 0;

	virtual void Update(double dt) = 0;

	liAgent(){}

	virtual ~liAgent() {}
};

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

