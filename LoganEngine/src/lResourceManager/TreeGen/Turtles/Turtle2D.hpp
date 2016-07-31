#ifndef TURTLE_2D_H
#define TURTLE_2D_H

#include <list>
#include <vector>

#include <string>

#include "../Interfaces/ITurtle.hpp"
#include "TreeGraph.hpp"

/*
 * TODO: Konzisztens elnevezési konvenciókat kéne használni.
 *
 * TODO: KURVASOK KÓDOT DUPLIKÁLUNK A 3D-S TEKNŐCCEL!!!!
 */
template<typename Type_T>
class Turtle2D : public ITurtle<Type_T,2>
{
private:
	//
	struct TurtleState
	{
		lmVectorND<Type_T,2> Position;
		Type_T Orientation;

		TreeNode<Type_T,2> *Node;
	};
	//
	TurtleState CurrentState;
	std::list<TurtleState> StateStack;
	//
	std::list<typename PolygonNode<Type_T,2>::PolygonNodeBuilder> PolygonStack;
	//
	Type_T dDist;
	Type_T dAngle;
	//
	void Move()
	{
		Type_T DirX = cos(CurrentState.Orientation);
		Type_T DirY = sin(CurrentState.Orientation);
		lmVectorND<Type_T,2> Displacement = dDist * lmVectorND<Type_T,2>({DirX,DirY});
		CurrentState.Position += Displacement;
	}
	//
	void Rotate(bool positive_direction)
	{
		if(positive_direction)
			{CurrentState.Orientation += dAngle;}
		else
			{CurrentState.Orientation -= dAngle;}
	}
	//
	void Push()
	{
		StateStack.push_back(CurrentState);
	}
	//
	void Pop()
	{
		CurrentState = StateStack.back();
		StateStack.pop_back();
	}
	//
public:
	//
	void Set_dAngle(Type_T d_angle)
	{
		dAngle = d_angle;
	}
	//
	void Set_dDist(Type_T d_dist)
	{
		dDist = d_dist;
	}
	//
	virtual void ExecuteCommands(const std::string &commands,ITreeNode<Type_T,2> *&tree_skeleton) override
	{
		CurrentState.Node = new TrunkNode<Type_T,2>(CurrentState.Position);
		tree_skeleton = CurrentState.Node;
		//
		TreeNode<Type_T,2> *NewNode;
		for(unsigned int i=0;i < commands.size();i++)
		{
			switch(commands[i])
			{
			case 'F':
				Move();
				//
				//if(/*!DrawPolygon*/PolygonStack.size() == 0)
				//{
					//
				//}
				//else
				//{
				NewNode = new TrunkNode<Type_T,2>(CurrentState.Position);
				CurrentState.Node->AddChild(NewNode);
				//
				CurrentState.Node = NewNode;
				//}
				//
				break;
			case 'f':
				Move();
				//
				break;
			case '+':
				Rotate(true);
				//
				break;
			case '-':
				Rotate(false);
				//
				break;
			case '[':
				Push();
				//
				break;
			case ']':
				Pop();
				//
				break;
			case '{':
				PolygonStack.emplace_back(CurrentState.Node);
				//
				break;
			case '}':
				PolygonStack.back().Construct(NewNode);
				PolygonStack.pop_back();
				//
				break;
			case '.':
				PolygonStack.back().AddVertex(CurrentState.Position);
				//
				break;
			}
		}
	}
	//
	Turtle2D(Type_T d_dist,Type_T d_angle)
		:dDist(d_dist),dAngle(d_angle)
	{
		/*
		 * Empty
		 */
	}
	//
	virtual ~Turtle2D() override
	{
		/*
		 * Empty
		 */
	}
	//
	/*
	 * Class vége
	 */
};

#endif // TURTLE_2D_H
