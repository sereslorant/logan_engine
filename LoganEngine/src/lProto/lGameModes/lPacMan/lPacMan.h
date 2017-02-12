#ifndef L_PACMAN_H
#define L_PACMAN_H


#include "lPM_Model/lPM_Model.h"
#include "lPM_JSONLoader.h"

#include "lPM_Controller/lPM_PacManJoystickController.h"

#include "lPM_View/lPM_CommandLineView.h"
#include "lPM_View/lPM_GraphicalView.h"

#include "lPM_AI.h"

#include <lGame/liGame.h>
#include <lApiAdapter/liApiAdapter.h>
#include <lRenderer/liRenderer.h>

class lPM_Game : public liGameMode
{
protected:
	//
	float dS = 40.0;
	//
	float TimeStep;
	float TimeElapsed = 0.0;
	//
	liPM_AgentController *AgentController = nullptr;
	lPM_AI_Percept *AI_Percept = nullptr;
	//
	lPM_Model Model;
	//
	//lPM_CommandLineView CommandLineView;
	lPM_GraphicalView GraphicalView;
	//
public:

	virtual void Logic(double dt) override
	{
		TimeElapsed += dt;
		if(TimeElapsed > TimeStep)
		{
			TimeElapsed -= TimeStep;
			Model.Step();
		}
	}

	lPM_Game(liInput &input,liRenderer &renderer,double time_step,const liJSON_Value &json_map)
		:GraphicalView(renderer),TimeStep(time_step)
	{
		//
		if(input.GetNumControllers() > 0)
		{
			liController *Controller = input.GetController(0);
			AgentController = new lPM_PacManJoystickController(Controller);
		}
		else
		{
			AI_Percept = new lPM_AI_Percept;
			Model.Subscribe(AI_Percept);
			//
			lPM_AI_Controller *Ctrl = new lPM_AI_Controller;
			Ctrl->SetPercept(AI_Percept);
			AgentController = Ctrl;
		}
		//
		lPM_JSONLoader MapLoader(Model.GetAgentFactory(),AgentController);
		json_map.Accept(MapLoader);
		//
		Model.SetDimensions(MapLoader.GetWidth(),MapLoader.GetHeight());
		//
		//Model.Subscribe(&CommandLineView);
		Model.Subscribe(&GraphicalView);
	}

	virtual ~lPM_Game() override
	{
		delete AgentController;
		delete AI_Percept;
	}
};

#endif // L_PACMAN_H
