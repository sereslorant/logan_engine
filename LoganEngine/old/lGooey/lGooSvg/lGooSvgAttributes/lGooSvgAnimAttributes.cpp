
#include "lGooSvgAnimAttributes.h"
#include "../lGooSVG.h"

void lGooSvgAnimAttributes::LoadAttribs(const std::string &attribute,std::string &value)
{
	if(attribute == "id")
	{
		HasId = true;
		Id = value;

		std::cout << "id="<< Id <<std::endl;
	}

	//Timing attributes
	if(attribute == "begin")
	{
		lGooSVG::ParseSemiColonSeparatedList(Attr_begin,value);

		std::cout << "begin="<< value <<std::endl;
	}
	if(attribute == "dur")
	{
		Attr_dur = lGooSVG::ParseClockValue(value);

		std::cout << "dur="<< Attr_dur <<"sec" <<std::endl;
	}
	if(attribute == "end")
	{
		lGooSVG::ParseSemiColonSeparatedList(Attr_end,value);

		std::cout << "end="<< value <<std::endl;
	}
	if(attribute == "restart")
	{
		if(value == "always")
		{
			Attr_restart = RST_ALWAYS;
		}
		if(value == "never")
		{
			Attr_restart = RST_NEVER;
		}
		if(value == "whenNotActive")
		{
			Attr_restart = RST_WHEN_NOT_ACTIVE;
		}

		std::cout << "restart="<< value << Attr_restart <<std::endl;
	}
	if(attribute == "repeatCount")
	{
		if(value == "indefinite")
		{
			Attr_repeatCount = 0.0;
		}
		else
		{
			Attr_repeatCount = std::stod(value);
		}
	}
	if(attribute == "repeatDur")
	{
		if(value == "indefinite")
		{
			Attr_repeatDur = 0.0;
		}
		else
		{
			Attr_repeatDur = lGooSVG::ParseClockValue(value);
		}
	}
	if(attribute == "fill")
	{
		if(value == "remove")
		{
			Attr_fill = F_REMOVE;
		}
		if(value == "freeze")
		{
			Attr_fill = F_FREEZE;
		}

		std::cout << "fill="<< value << Attr_fill <<std::endl;
	}
	//Anim attributes
	if(attribute == "attributeName")
	{
		Attr_attributeName = value;

		std::cout << "attributeName="<< Attr_attributeName << std::endl;
	}
	if(attribute == "attributeType")
	{
		if(value == "CSS")
		{
			Attr_attributeType = T_CSS;
		}
		if(value == "XML")
		{
			Attr_attributeType = T_XML;
		}
		if(value == "auto")
		{
			Attr_attributeType = T_AUTO;
		}

		std::cout << "attributeType="<< value << Attr_attributeType <<std::endl;
	}
	if(attribute == "additive")
	{
		if(value == "replace")
		{
			Attr_additive = ADD_REPLACE;
		}
		if(value == "sum")
		{
			Attr_additive = ADD_SUM;
		}

		std::cout << "additive="<< value << Attr_additive <<std::endl;
	}
	if(attribute == "accumulate")
	{
		if(value == "none")
		{
			Attr_accumulate = ACC_NONE;
		}
		if(value == "sum")
		{
			Attr_accumulate = ACC_SUM;
		}

		std::cout << "accumulate="<< value << Attr_accumulate <<std::endl;
	}
	//Animate attributes
	if(attribute == "calcMode")
	{
		if(value == "discrete")
		{
			Attr_calcMode = CM_DISCRETE;
		}
		if(value == "linear")
		{
			Attr_calcMode = CM_LINEAR;
		}
		if(value == "paced")
		{
			Attr_calcMode = CM_PACED;
		}
		if(value == "spline")
		{
			Attr_calcMode = CM_SPLINE;
		}

		std::cout << "calcMode="<< value << Attr_calcMode <<std::endl;
	}
	//std::list<float> Attr_keyTimes;
	//std::list<float> Attr_keySplines;
	if(attribute == "values")
	{
		ValuesFound = true;
		//Be kéne tölteni az értékek listáját
		//Attr_values = ...;
		std::cout << "values="<< value << "" <<std::endl;
	}
	if(attribute == "from")
	{
		FromFound = true;
		Attr_from = value;

		std::cout << "from="<< Attr_from <<std::endl;
	}
	if(attribute == "to")
	{
		ToFound = true;
		Attr_to = value;

		std::cout << "to="<< Attr_to <<std::endl;
	}
	if(attribute == "by")
	{
		ByFound = true;
		Attr_by = value;

		std::cout << "by="<< Attr_by <<std::endl;
	}
}

void lGooSvgAnimAttributes::Process()
{
	//Legyünk optimisták! Majd rácáfolunk. :P
	Error = false;

	if(!AttributeFound)
	{
		Error = true;
	}

	if(ValuesFound)
	{
		AnimType = ANIM_VALUE;
	}
	else
	{
		if(FromFound)
		{
			if(ToFound)
			{
				AnimType = ANIM_FROM_TO;
			}
			else if(ByFound)
			{
				AnimType = ANIM_FROM_BY;
			}
			else
			{
				Error = true;
			}
		}
		else
		{
			if(ToFound)
			{
				AnimType = ANIM_TO;
			}
			else if(ByFound)
			{
				AnimType = ANIM_BY;
			}
			else
			{
				Error = true;
			}
		}
	}
}

lGooSvgAnimAttributes::lGooSvgAnimAttributes()
{
	Error = true;

	HasId = false;

	//Timing attributes
	Attr_restart	= RST_ALWAYS;
	Attr_repeatCount = 1.0;
	//Attr_repeatDur;
	Attr_fill		= F_REMOVE;

	//Anim attributes
	AttributeFound		= false;
	Attr_attributeType	= T_AUTO;
	Attr_additive		= ADD_REPLACE;
	Attr_accumulate		= ACC_SUM;

	//Animate attributes
	Attr_calcMode		= CM_LINEAR;

	ValuesFound	= false;
	FromFound	= false;
	ToFound		= false;
	ByFound		= false;
}
