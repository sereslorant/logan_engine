#ifndef L_JSON_LOADER_H
#define L_JSON_LOADER_H

#include "lPM_Model.h"

#include "../../../lJSON/lJSON.h"

class lPM_JSONLoader : public liJSON_Visitor
{
private:

	float dS = 40.0;

	liPM_AgentFactory *AgentFactory;
	liPM_AgentController *AgentController;

	unsigned int X = 0;
	unsigned int Y = 0;

	bool Puszcsy;

	void LoadCell(const liJSON_String &cell)
	{
		if(cell.GetValue() == "e")
		{
			/*
			 * Itt nem csinálunk semmit.
			 */
		}
		else if(cell.GetValue() == "p")
		{
			AgentFactory->AddPacMan(X,Y,AgentController);
		}
		else if(cell.GetValue() == "f")
		{
			AgentFactory->AddCoin(X,Y);
		}
		else
		{
			AgentFactory->AddAgent(X,Y,false);
		}
	}

	void LoadRow(const liJSON_Array &row)
	{
		X = 0;
		for(unsigned int i=0;i < row.Size();i++)
		{
			row.GetElement(i)->Accept(*this);
			X++;
		}
	}

	void LoadMap(const liJSON_Array &pm_map)
	{
		Puszcsy = true;
		Y = 0;
		for(unsigned int i=0;i < pm_map.Size();i++)
		{
			pm_map.GetElement(i)->Accept(*this);
			std::cout << std::endl;
			Y++;
		}
		Puszcsy = false;
	}

public:

	virtual void Visit(const liJSON_Array &param) override
	{
		if(Puszcsy == false)
		{
			LoadMap(param);
		}
		else
		{
			LoadRow(param);
		}
	}

	virtual void Visit(const liJSON_Integer &param) override
	{
		//Üres
	}

	virtual void Visit(const liJSON_Double &param) override
	{
		//Üres
	}

	virtual void Visit(const liJSON_Bool &param) override
	{
		//Üres
	}


	virtual void Visit(const liJSON_String &param) override
	{
		LoadCell(param);
	}

	virtual void Visit(const liJSON_Object &param) override
	{
		//Üres
	}

	virtual void VisitNull(const liJSON_Value &param) override
	{
		//Üres
	}

	virtual void Visit(liJSON_Array &param) override
	{
		if(Puszcsy == false)
		{
			LoadMap(param);
		}
		else
		{
			LoadRow(param);
		}
	}

	virtual void Visit(liJSON_Integer &param) override
	{
		//Üres
	}

	virtual void Visit(liJSON_Double &param) override
	{
		//Üres
	}

	virtual void Visit(liJSON_Bool &param) override
	{
		//Üres
	}

	virtual void Visit(liJSON_String &param) override
	{
		LoadCell(param);
	}

	virtual void Visit(liJSON_Object &param) override
	{
		//Üres
	}

	virtual void VisitNull(liJSON_Value &param) override
	{
		//Üres
	}

	lPM_JSONLoader(liPM_AgentFactory *agent_factory,liPM_AgentController *agent_control)
		:AgentFactory(agent_factory),AgentController(agent_control)
	{
		Puszcsy = false;
	}

	virtual ~lPM_JSONLoader() override
	{
		//Üres
	}
};

#endif // L_JSON_LOADER_H
