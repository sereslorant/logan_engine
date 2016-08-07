#ifndef TURTLE_3D_H
#define TURTLE_3D_H

#include <list>
#include <vector>

#include <string>

#include "../Interfaces/ITurtle.hpp"
#include "TreeGraph.hpp"

/*
 * TODO: Konzisztens elnevezési konvenciókat kéne használni.
 *
 * TODO: KURVASOK KÓDOT DUPLIKÁLUNK A 2D-S TEKNŐCCEL!!!!
 */
template<typename Type_T>
class Turtle3D : public ITurtle<Type_T,3>
{
//public:
private:
	//
	struct TurtleState
	{
		lmVectorND<Type_T,3>	Position;
		lmVectorND<Type_T,3>	Direction_H;
		lmVectorND<Type_T,3>	Direction_L;
		lmVectorND<Type_T,3>	Direction_U;
		//
		TreeNode<Type_T,3> *Node;
	};
	//
	enum TurtleRotation
	{
		ROT_X,
		ROT_Y,
		ROT_Z,
	};
	//
//private:
	//
	TurtleState CurrentState;
	std::list<TurtleState> StateStack;
	//
	std::list<typename PolygonNode<Type_T,3>::PolygonNodeBuilder> PolygonStack;
	//
	Type_T dDist;
	Type_T dAngle;
	//
	void Move()
	{
		lmVectorND<Type_T,3> Displacement = dDist * CurrentState.Direction_U;
		CurrentState.Position += Displacement;
	}
	//
	void Rotate(TurtleRotation rotation,bool positive_direction,Type_T angle)
	{
		Type_T Angle;
		if(positive_direction)
			{Angle = angle;}
		else
			{Angle = -1.0*angle;}
		//
		lmQuaternionT<Type_T> Rotation;
		lmVectorND<Type_T,3> Axle;
		//
		switch(rotation)
		{
		case ROT_X:
			Axle = CurrentState.Direction_H;
			break;
			//
		case ROT_Y:
			Axle = CurrentState.Direction_L;
			break;
			//
		case ROT_Z:
			Axle = CurrentState.Direction_U;
			break;
		}
		Axle.Normalize();
		Rotation = lmQuaternionT<Type_T>(Axle,Angle);
		//
		CurrentState.Direction_H = lmQuaternionRotate(CurrentState.Direction_H,Rotation);
		CurrentState.Direction_L = lmQuaternionRotate(CurrentState.Direction_L,Rotation);
		CurrentState.Direction_U = lmQuaternionRotate(CurrentState.Direction_U,Rotation);
		//
		CurrentState.Direction_H.Normalize();
		CurrentState.Direction_L.Normalize();
		CurrentState.Direction_U.Normalize();
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
	virtual void ExecuteCommands(const std::string &commands,ITreeNode<Type_T,3> *&tree_skeleton) override
	{
		CurrentState.Node = new TrunkNode<Type_T,3>(CurrentState.Position);
		tree_skeleton = CurrentState.Node;
		//
		TreeNode<Type_T,3> *NewNode;
		for(unsigned int i=0;i < commands.size();i++)
		{
			switch(commands[i])
			{
			case 'F':
				Move();
				/*if(DrawPolygon)
				{
					//
				}
				else
				{*/
				NewNode = new TrunkNode<Type_T,3>(CurrentState.Position);
				CurrentState.Node->AddChild(NewNode);

				CurrentState.Node = NewNode;
				//}
				//
				break;
			case 'f':
				Move();
				//
				break;
			case '(':
				Rotate(ROT_X,true,dAngle);
				//
				break;
			case ')':
				Rotate(ROT_X,false,dAngle);
				//
				break;
			case '<':
				Rotate(ROT_Y,true,dAngle);
				//
				break;
			case '>':
				Rotate(ROT_Y,false,dAngle);
				//
				break;
			case '+':
				Rotate(ROT_Z,true,dAngle);
				//
				break;
			case '-':
				Rotate(ROT_Z,false,dAngle);
				//
				break;
			case '|':
				Rotate(ROT_Y,true,PI);
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
	Turtle3D(const lmVectorND<Type_T,3> &direction_h,const lmVectorND<Type_T,3> &direction_l,Type_T d_dist,Type_T d_angle)
		:dDist(d_dist),dAngle(d_angle)
	{
		CurrentState.Direction_H = direction_h;
		CurrentState.Direction_L = direction_l;
		CurrentState.Direction_U = lmCross(direction_h,direction_l);
		//
		CurrentState.Direction_H.Normalize();
		CurrentState.Direction_L.Normalize();
		CurrentState.Direction_U.Normalize();
	}
	//
	virtual ~Turtle3D() override
	{
		/*
		 * Üres függvénytörzs
		 */
	}
	//
	/*
	 * Class vége
	 */
};


#endif // TURTLE_2D_H
