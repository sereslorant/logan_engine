#ifndef L_RANDOM_GENERATOR_H
#define L_RANDOM_GENERATOR_H

#include <cstdlib>
#include <ctime>

class lRandomGenerator
{
private:
	static lRandomGenerator RandomGenerator;

	lRandomGenerator()
	{
		srand(time(0));
	}

public:

	~lRandomGenerator()
	{
		//Üres függvénytörzs
	}

	double RandomDouble(double LowerBound,double HigherBound)
	{
		double Rand = (double)rand() / (double)(RAND_MAX);

		return LowerBound + (Rand)*(HigherBound-LowerBound);
	}

	float RandomFloat(float LowerBound,float HigherBound)
	{
		float Rand = (float)rand() / (float)(RAND_MAX);

		return LowerBound + (Rand)*(HigherBound-LowerBound);
	}

	static lRandomGenerator *GetInstance()
	{
		return &RandomGenerator;
	}

	/*
	 * Class vége
	 */
};

#endif // L_RANDOM_GENERATOR_H
