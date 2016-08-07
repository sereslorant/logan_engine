#ifndef I_TURTLE_H
#define I_TURTLE_H

#include <string>
#include "ITreeGraph.hpp"

template<typename Type_T,unsigned int Dim_T>
class ITurtle
{
public:
	//
	virtual void ExecuteCommands(const std::string &commands,ITreeNode<Type_T,Dim_T> *&tree_skeleton) = 0;
	//
	ITurtle()
	{
		/*
		 * Empty
		 */
	}
	//
	virtual ~ITurtle()
	{
		/*
		 * Empty
		 */
	}
	//
	/*
	 * End of class
	 */
};

#endif // I_TURTLE_H
