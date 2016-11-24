
#include "lUtils.h"

int GetHexDigit(char Digit)
{
	if(Digit >= '0' && Digit <= '9')
	{
		return Digit - '0';
	}
	else if(tolower(Digit) >= 'a' && tolower(Digit) <= 'f')
	{
		return 10 + (tolower(Digit) - 'a');
	}
	else
	{
		return -1;
	}
}

void LoadQuotedString(std::istream &in,std::string &str,bool step_back)
{
    char c;

    if(step_back)
	{
		in.seekg(-1,std::ios_base::cur);
	}

    in.get(c);
    while(c != '\"')
    {
        in.get(c);
    }

    in.get(c);
    while(c != '\"')
    {
        str.push_back(c);
        in.get(c);
    }
}
