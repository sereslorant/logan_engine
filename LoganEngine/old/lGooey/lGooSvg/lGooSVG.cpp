
#include "lGooSVG.h"

using namespace std;

std::map<std::string,lGooColor> lGooSVG::ColorTable;

void lGooSVG::ParseLengthDimension(std::string &value,SVG_LENGTH_DIMENSION &value_type)
{
	//Alapértelmezetten a mértékegység legyen pixelben mérve!
	value_type = PX;

	if(value[value.size()-1] == '%')
	{
		value.pop_back();
		value_type = PERCENTAGE;
	}
	else if(value[value.size()-2] == 'e' && value[value.size()-1] == 'm')
	{
		value.pop_back();value.pop_back();
		//value_type = EM;
		value_type = PX;
	}
	else if(value[value.size()-2] == 'e' && value[value.size()-1] == 'x')
	{
		value.pop_back();value.pop_back();
		//value_type = EX;
		value_type = PX;
	}
	else if(value[value.size()-2] == 'p' && value[value.size()-1] == 'x')
	{
		value.pop_back();value.pop_back();
		value_type = PX;
	}
	else if(value[value.size()-2] == 'i' && value[value.size()-1] == 'n')
	{
		value.pop_back();value.pop_back();
		value_type = IN;
	}
	else if(value[value.size()-2] == 'c' && value[value.size()-1] == 'm')
	{
		value.pop_back();value.pop_back();
		value_type = CM;
	}
	else if(value[value.size()-2] == 'm' && value[value.size()-1] == 'm')
	{
		value.pop_back();value.pop_back();
		value_type = MM;
	}
	else if(value[value.size()-2] == 'p' && value[value.size()-1] == 'x')
	{
		value.pop_back();value.pop_back();
		value_type = PT;
	}
	else if(value[value.size()-2] == 'p' && value[value.size()-1] == 'c')
	{
		value.pop_back();value.pop_back();
		value_type = PC;
	}
}

void lGooSVG::ParseColor(std::string &value,lGooColor &color)
{
	if(value[0] == '#')
	{
		//Load hex color
		if(value.size() == 4)
		{
			int Red = GetHexDigit(value[1]);
			Red |= Red << 4;

			int Green = GetHexDigit(value[2]);
			Green |= Green << 4;

			int Blue = GetHexDigit(value[3]);
			Blue |= Blue << 4;

			color.SetRed(Red/255.0f);
			color.SetGreen(Green/255.0f);
			color.SetBlue(Blue/255.0f);
		}
		else if(value.size() == 7)
		{
			int	Red  = GetHexDigit(value[1]);
				Red |= GetHexDigit(value[2]) << 4;

			int Green  = GetHexDigit(value[3]);
				Green |= GetHexDigit(value[4]) << 4;

			int Blue  = GetHexDigit(value[5]);
				Blue |= GetHexDigit(value[6]) << 4;

			color.SetRed(Red/255.0f);
			color.SetGreen(Green/255.0f);
			color.SetBlue(Blue/255.0f);
		}
		else
		{
			//Error |= SVG_SVG_SYNTAX_ERROR;
		}
	}
	else if(value[0] == 'r' && value[1] == 'g' && value[2] == 'b' && value[3] == '(')
	{
		enum ColorParserSM
		{
			RED,
			GREEN,
			BLUE,
			FINISHED,
		};
		int Red = 0;
		int Green = 0;
		int Blue = 0;

		bool Percentage = false;
		ColorParserSM ParserSM = RED;

		for(int i=4;i < value.size();i++)
		{
			if(isspace(value[i]))
			{continue;}

			switch(ParserSM)
			{
			case RED:

				if(isdigit(value[i]))
				{
					Red = 10*Red + GetHexDigit(value[i]);
				}
				else if(value[i] == ',')
				{
					ParserSM = GREEN;
				}
				else if(value[i] == '%')
				{
					Percentage = true;
				}
				break;
			case GREEN:

				if(isdigit(value[i]))
				{
					Green = 10*Green + GetHexDigit(value[i]);
				}
				else if(value[i] == ',')
				{
					ParserSM = BLUE;
				}
				else if(value[i] == '%')
				{
					if(!Percentage)
					{
						//Error |= SVG_SVG_SYNTAX_ERROR;
					}
				}
				break;
			case BLUE:

				if(isdigit(value[i]))
				{
					Blue = 10*Blue + GetHexDigit(value[i]);
				}
				else if(value[i] == ')')
				{
					ParserSM = FINISHED;
				}
				else if(value[i] == '%')
				{
					if(!Percentage)
					{
						//Error |= SVG_SVG_SYNTAX_ERROR;
					}
				}
				break;
			}
		}

		if(Percentage)
		{
			color.SetRed(Red/100.0f);
			color.SetGreen(Green/100.0f);
			color.SetBlue(Blue/100.0f);
		}
		else
		{
			color.SetRed(Red/255.0f);
			color.SetGreen(Green/255.0f);
			color.SetBlue(Blue/255.0f);
		}
	}
	else
	{
		color = ColorTable[value];
	}
}

double lGooSVG::ParseClockValue(std::string value)
{
	enum ValueParserSM
	{
		CLOCK_VALUE,
		TIMECOUNT_VALUE,
	};

	enum ClockValueParserSM
	{
		CV_HOURS,
		CV_MINUTES,
		CV_SECONDS,
		CV_FRACTION,
	};

	enum TimecountValueParserSM
	{
		TCV_SECONDS,
		TCV_FRACTION,
		TCV_METRICS,
	};

	enum METRICS
	{
		M_HOUR,
		M_MIN,
		M_SEC,
		M_MSEC,
	};

	ValueParserSM VParserSM;
	ClockValueParserSM CVParserSM;
	TimecountValueParserSM TCVParserSM;

	METRICS Metrics = M_SEC;

	double Time = 0.0;

	int ColonCount = std::count(value.begin(),value.end(),':');

	if(ColonCount == 2)
	{
		VParserSM = CLOCK_VALUE;
		CVParserSM = CV_HOURS;
	}
	else if(ColonCount == 1)
	{
		VParserSM = CLOCK_VALUE;
		CVParserSM = CV_MINUTES;
	}
	else
	{
		VParserSM = TIMECOUNT_VALUE;
		TCVParserSM = TCV_SECONDS;
	}

	int Hours = 0;
	int Mins  = 0;
	double Seconds = 0.0;
	double Fraction = 0.0;
	int nFraction = 1;

	switch(VParserSM)
	{
	case CLOCK_VALUE:
		for(int i=0;i < value.size();i++)
		{
			switch(CVParserSM)
			{
			case CV_HOURS:
				if(isdigit(value[i]))
				{
					Hours = 10*Hours + GetHexDigit(value[i]);
				}
				else if(value[i] == ':')
				{
					CVParserSM = CV_MINUTES;
				}
				break;

			case CV_MINUTES:
				if(isdigit(value[i]))
				{
					Mins = 10*Mins + GetHexDigit(value[i]);
				}
				else if(value[i] == ':')
				{
					CVParserSM = CV_SECONDS;
				}
				break;

			case CV_SECONDS:
				if(isdigit(value[i]))
				{
					Seconds = 10.0*Seconds + GetHexDigit(value[i]);
				}
				else if(value[i] == '.')
				{
					CVParserSM = CV_FRACTION;
				}
				break;

			case CV_FRACTION:
				if(isdigit(value[i]))
				{
					Seconds = Seconds/10.0 + GetHexDigit(value[i])/10.0;
				}
				else if(value[i] == '.')
				{
					CVParserSM = CV_FRACTION;
				}
			}
		}

		return 60.0*60.0*Hours + 60*Mins + Seconds + Fraction;

	case TIMECOUNT_VALUE:
		for(int i=0;i < value.size();i++)
		{
			switch(TCVParserSM)
			{
			case TCV_SECONDS:
				if(isdigit(value[i]))
				{
					Seconds = 10.0*Seconds + GetHexDigit(value[i]);
				}
				else if(value[i] == '.')
				{
					TCVParserSM = TCV_FRACTION;
				}
				else
				{
					TCVParserSM = TCV_METRICS;
				}
				break;

			case TCV_FRACTION:
				if(isdigit(value[i]))
				{
					Fraction += GetHexDigit(value[i]) / pow(10.0,nFraction);

					nFraction++;
				}
				else if(value[i] == '.')
				{
					TCVParserSM = TCV_FRACTION;
				}
				else
				{
					TCVParserSM = TCV_METRICS;
				}
				break;

			case TCV_METRICS:

				if(value[i] == 'h')
				{
					Metrics = M_HOUR;
				}
				else if(value[i] == 's')
				{
					Metrics = M_SEC;
				}
				else if(value[i] == 'm')
				{
					i++;
					if(value[i] == 's')
					{
						Metrics = M_MSEC;
					}
				}
				else if(value[i] == 'm')
				{
					i++;
					if(value[i] == 'i')
					{
						i++;
						if(value[i] == 'n')
						{
							Metrics = M_MIN;
						}
					}
				}
			}
		}

		switch(Metrics)
		{
		case M_HOUR:
			return 60*60*(Seconds+Fraction);

		case M_MIN:
			return 60*(Seconds+Fraction);

		case M_SEC:
			return (Seconds+Fraction);

		case M_MSEC:
			return (Seconds+Fraction) / 1000.0;
		}
		break;

	}

	return 0.0;
}

void lGooSVG::ParseSemiColonSeparatedList(std::list<std::string> &dest,const std::string &src)
{
	std::string NewValue;

	for(int i=0;i < src.size();i++)
	{
		if(isspace(src[i]))
		{
			continue;
		}

		if(src[i] == ';')
		{
			dest.push_back(NewValue);
			NewValue.clear();
			continue;
		}

		NewValue += src[i];
	}

	dest.push_back(NewValue);
}
