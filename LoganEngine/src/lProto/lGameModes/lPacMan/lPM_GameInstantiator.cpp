
#include "lPM_GameInstantiator.h"

#include "lPacMan.h"
#include "../../../lGame/lGame.h"

#include <sstream>

const char *Map0 =
R"(
[["e",    "sc_ul","sh","sc_ur","e"    ],
  ["sc_ul","sc_lr","p", "sc_ll","sc_ur"],
  ["sv",   "f",    "f", "f",    "sv"   ],
  ["sv",   "f",    "dv","f",    "sv"   ],
  ["sv",   "f",    "f", "f",    "sv"   ],
  ["sc_ll","sc_ur","f", "sc_ul","sc_lr"],
  ["e",    "sc_ll","sh","sc_lr","e"    ]]
  )";

const char *Map1 =
R"(
[["sc_ul","sh","sh",   "sh","sh","hc_ur","sh","sh","sh",   "sh","sc_ur" ],
  ["sv",   "f", "f",    "f", "f", "dv",   "f", "f", "f",    "f", "sv"   ],
  ["sv",   "f", "dc_ul","dh","f", "dv",   "f", "dh","dc_ur","f", "sv"   ],
  ["sv",   "f", "dv",   "f", "f", "f",    "f", "f", "dv",   "f", "sv"   ],
  ["sv",   "f", "dv",   "f", "dh","dh",   "dh","f", "dv",   "f", "sv"   ],
  ["sv",   "f", "f",    "f", "f", "f",    "f", "f", "f",    "f", "sv"   ],
  ["sv",   "f", "dv",   "f", "dh","dh",   "dh","f", "dv",   "f", "sv"   ],
  ["sv",   "f", "dv",   "f", "f", "f",    "f", "f", "dv",   "f", "sv"   ],
  ["sv",   "f", "dc_ll","dh","f", "p",    "f", "dh","dc_lr","f", "sv"   ],
  ["sv",   "f", "f",    "f", "f", "dv",   "f", "f", "f",    "f", "sv"   ],
  ["sc_ll","sh","sh",   "sh","sh","hc_ll","sh","sh","sh",   "sh","sc_lr"]]
  )";

#include "../../../lResourceManager/lJSON/lJSON.h"

liGame *lPM_GameInstantiator::CreateGame(liApiAdapter &api_adapter,lrmResourceManager &resource_manager,liRenderer &renderer,liConsole &console)
{
	bool ChosenMap = false;
	liJSON_Value *JSON_Map;
	//
	if(ChosenMap)
	{
		std::stringstream MapStream(Map0);
		lJSON_Util::Parse(MapStream,JSON_Map);
	}
	else
	{
		std::stringstream MapStream(Map1);
		lJSON_Util::Parse(MapStream,JSON_Map);
	}
	//
	liGameMode *GameMode = new lPM_Game(api_adapter.GetInput(),renderer,0.125,*JSON_Map);
	//
	delete JSON_Map;
	//
	liGame *Game = new lGame(console,GameMode,renderer);
	//
	return Game;
}

liGameInstantiator *CreateGameInstantiator()
{
    return new lPM_GameInstantiator;
}
