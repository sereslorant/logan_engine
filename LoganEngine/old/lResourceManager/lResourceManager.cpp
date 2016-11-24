
#include <iostream>
#include <fstream>

#include "lrmResourceManager.h"

extern "C" {

liResourceManager *lrmCreateResourceManager(lmScalar *def_diffuse,lmScalar *def_specular,lmScalar def_shininess)
{
	return new lrmResourceManager(def_diffuse,def_specular,def_shininess);
}
/*
bool lrmLoadText(const std::string &filename,std::string &destination)
{
	std::ifstream In;
	In.open(filename.c_str());

	if(!In.is_open())
	{
        #ifdef L_TESTING
            std::cerr << "Couldn't open " << filename << "\n";
		#endif

		return false;
	}

	while(!In.eof())
	{
		std::string Tmp;
		getline(In,Tmp);

		destination += Tmp;
		destination += "\n";
	}

	return true;
}
*/
}
