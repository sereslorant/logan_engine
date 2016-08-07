#ifndef TREE_TRUNK_H
#define TREE_TRUNK_H

#include "../lMath/lMath.h"

//#include <algorithm>

template<typename Type_T,unsigned int Dim_T>
class TreeTrunk
{
public:
	//
	struct BranchSegment
	{
		int Id = -1;
		unsigned int Depth = 0;
		//
		liCurve<Type_T,Dim_T> *Curve = nullptr;
		//
		Type_T t_Begin;
		Type_T t_End;
		//
		BranchSegment()
		{}
		//
		~BranchSegment()
		{
			delete Curve;
		}
		//
		/*
		 * End of class
		 */
	};
	//
	struct Junction
	{
		BranchSegment *Root = nullptr;
		std::list<BranchSegment *> Children;
		//
		int Id = -1;
		//
		Junction()
		{}
		//
		~Junction()
		{}
		//
		/*
		 * End of class
		 */
	};
	//
//private:
	std::list<BranchSegment *> Branches;
	std::list<Junction *> Junctions;

//public:
	/*
	void Prepare()
	{
		unsigned int BranchId = 0;
		for(Branch *B : Branches)
		{
			B->Id = BranchId;
			BranchId++;
		}

		unsigned int JunctionId = 0;
		for(Junction *J : Junctions)
		{
			float dt_min = 1.0/10.0;

			std::list<float> Roots;
			if(J->Root != nullptr)
			{
				float t0_0 = J->Root->t_Begin;
				float t0_fin = J->Root->t_End;

				if(BisectionMethod(*J->Root->Curve,t0_0,t0_fin,dt_min,J->JunctionSphere,Roots))
				{
					J->Root->t_End = *std::max_element(std::begin(Roots),std::end(Roots));
				}
				Roots.clear();
			}

			for(Branch *B : J->Children)
			{
				float t0_1 = B->t_Begin;
				float t1_fin = B->t_End;

				if(BisectionMethod(*B->Curve,t0_1,t1_fin,dt_min,J->JunctionSphere,Roots))
				{
					B->t_Begin = *std::min_element(std::begin(Roots),std::end(Roots));
				}
				Roots.clear();
			}

			J->Id = JunctionId;
			JunctionId++;
		}
	}
	*/
	TreeTrunk()
	{}

	~TreeTrunk()
	{
		for(auto I : Branches)
		{
			delete I;
		}

		for(auto I : Junctions)
		{
			delete I;
		}
	}
};

#endif // TREE_TRUNK_H
