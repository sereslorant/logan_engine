#ifndef TRUNK_GENERATOR_H
#define TRUNK_GENERATOR_H

#include "Interfaces/ITreeGraph.hpp"

#include "TreeTrunk.hpp"

#include <list>

template<typename Type_T,unsigned int Dim_T>
class TrunkGenerator
{
private:
	//
	class BranchCounter : public ITreeNode<Type_T,Dim_T>::ITreeOperation, public ITreeVisitor<Type_T,Dim_T>
	{
	private:
		unsigned int NumBranches = 0;
		//
	public:
		//
		void Reset()
		{
			NumBranches = 0;
		}
		//
		unsigned int GetNumBranches()
		{
			return NumBranches;
		}
		//
		virtual void Operation(ITreeNode<Type_T,Dim_T> *tree_node) override
		{
			tree_node->Accept(this);
		}
		//
		virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) override
		{NumBranches++;}
		//
		virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) override
		{}
		//
		BranchCounter()
		{}
		//
		virtual ~BranchCounter() override
		{}
		//
		/*
		 * End of class
		 */
	};
	//
	//
	struct JunctionState : private ITreeVisitor<Type_T,Dim_T>
	{
	private:
		//
		virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) override
		{junctionTrunk = node;}
		//
		virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) override
		{}

	public:
		ITreeNode<Type_T,Dim_T> *junctionNode;
		ITrunkNode<Type_T,Dim_T> *junctionTrunk = nullptr;
		//
		unsigned int numBranches;
		unsigned int branchesCompleted = 0;
		//
		typename TreeTrunk<Type_T,Dim_T>::Junction *junction;
		//
		bool IsFinished()
		{
			return (branchesCompleted == numBranches);
		}
		//
		JunctionState(ITreeNode<Type_T,Dim_T> *junction_node,typename TreeTrunk<Type_T,Dim_T>::BranchSegment *root)
			:junctionNode(junction_node)
		{
			junction_node->Accept(this);
			//
			BranchCounter branchCounter;
			junctionNode->ForeachChildren(&branchCounter);
			//
			numBranches = branchCounter.GetNumBranches();
			//
			junction = new typename TreeTrunk<Type_T,Dim_T>::Junction;
			junction->Root = root;
		}
		//
		/*
		 * End of class
		 */
	};
	//
	class SegmentGenerator // : private ITreeVisitor<Type_T,Dim_T>
	{
	private:
		lmCatmullRomSpline<Type_T,Dim_T> Spline;
		//
		Type_T NextParam;
		const Type_T DELTA_NEXT_PARAM = 1.0;
		//
	public:
		/*
		virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) override
		{}
		//
		virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) override
		{}
		*/
		typename TreeTrunk<Type_T,Dim_T>::BranchSegment *CreateBranch(unsigned int depth)
		{
			typename TreeTrunk<Type_T,Dim_T>::BranchSegment *NewSegment = new typename TreeTrunk<Type_T,Dim_T>::BranchSegment;
			//
			NewSegment->Curve = Spline.Clone();
			//
			NewSegment->t_Begin = 0.0;
			NewSegment->t_End = NextParam;
			//
			NewSegment->Depth = depth;
			//
			return NewSegment;
		}
		//
		void AddBegin(ITrunkNode<Type_T,Dim_T> *node)
		{
			lmVectorND<Type_T,Dim_T> NullVector;
			Spline.Clear();
			NextParam = DELTA_NEXT_PARAM;
			//
			Spline.SetInitVelocity(NullVector);
			Spline.AddControlPoint(node->GetPosition(),0.0 - 0.125);
		}
		//
		void AddCtrlPoint(ITrunkNode<Type_T,Dim_T> *node)
		{
			Spline.AddControlPoint(node->GetPosition(),NextParam);
			NextParam += DELTA_NEXT_PARAM;
		}
		//
		void AddEnd(ITrunkNode<Type_T,Dim_T> *node)
		{
			lmVectorND<Type_T,Dim_T> NullVector;
			Spline.SetEndVelocity(NullVector);
			Spline.AddControlPoint(node->GetPosition(),NextParam + 0.125);
		}
		//
		SegmentGenerator()
		{}
		//
		~SegmentGenerator()
		{}
		//
		/*
		 * End of class
		 */
	};
	//
	struct TrunkVisitor : public ITreeNode<Type_T,Dim_T>::ITreeOperation, private ITreeVisitor<Type_T,Dim_T>
	{
	private:
		//
		enum TrunkVisitorState
		{
			CHECKING_TRUNK,
			VISITING_WORM,
			VISITING_JUNCTION,
		};
		//
		bool Begin = false;
		bool First = true;
		//
		bool IsTrunk = false;
		TrunkVisitorState VisitorState = CHECKING_TRUNK;
		std::list<JunctionState> junctionStack;
		//
		SegmentGenerator segmentGenerator;
		//
		TreeTrunk<Type_T,Dim_T> &Result;
		//
		//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!
		typename TreeTrunk<Type_T,Dim_T>::BranchSegment *LastSegment = nullptr;
		//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//
		void InsertJunction(ITreeNode<Type_T,Dim_T> *tree_node)
		{
			junctionStack.emplace_back(tree_node,LastSegment);
		}
		//
		void BeginBranch()
		{
			segmentGenerator.AddBegin(junctionStack.back().junctionTrunk);
		}
		//
		virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) override
		{
			switch(VisitorState)
			{
			case CHECKING_TRUNK:
				IsTrunk = true;
				return;
			case VISITING_WORM:
				if(!First)
				{
					segmentGenerator.AddCtrlPoint(node);
				}
				else
				{
					First = false;
				}
				return;
			case VISITING_JUNCTION:
				if(!First)
				{
					segmentGenerator.AddEnd(node);
					//
					typename TreeTrunk<Type_T,Dim_T>::BranchSegment *NewSegment = segmentGenerator.CreateBranch(junctionStack.size() - 1);
					junctionStack.back().junction->Children.push_back(NewSegment);
					Result.Branches.push_back(NewSegment);
					LastSegment = NewSegment;
					//
					junctionStack.back().branchesCompleted++;
					if(junctionStack.back().IsFinished())
					{
						//TODO: Hozzá kell adni a junctiont a hipergráfhoz.
						Result.Junctions.push_back(junctionStack.back().junction);
						junctionStack.pop_back();
					}
					//
					Begin = true;
				}
				/*else
				{
					First = false;
				}*/
				return;
			}
		}
		//
		virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) override
		{
			//Ezzel nem csinálunk semmit.
		}
		//
	public:
		//
		virtual void Operation(ITreeNode<Type_T,Dim_T> *tree_node) override
		{
			VisitorState = CHECKING_TRUNK;
			IsTrunk = false;
			//
			tree_node->Accept(this);
			//
			if(!IsTrunk)
			{return;}
			//
			if(First)
			{
				InsertJunction(tree_node);
				Begin = true;
			}
			//
			if(Begin)
			{
				BeginBranch();
				Begin = false;
			}
			//
			BranchCounter branchCounter;
			tree_node->ForeachChildren(&branchCounter);
			//
			if(branchCounter.GetNumBranches() == 1)
			{
				VisitorState = VISITING_WORM;
				//
				tree_node->Accept(this);
			}
			else
			{
				VisitorState = VISITING_JUNCTION;
				//
				tree_node->Accept(this);
				//
				if(branchCounter.GetNumBranches() != 0)
				{
					if(!First)
					{
						InsertJunction(tree_node);
					}
					else
					{
						First = false;
					}
				}
			}
		}
		//
		TrunkVisitor(TreeTrunk<Type_T,Dim_T> &result)
			:Result(result)
		{
			/*
			 * Empty
			 */
		}
		//
		virtual ~TrunkVisitor() override
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
	//
public:
	//
	void Process(ITreeNode<Type_T,Dim_T> *root,TreeTrunk<Type_T,Dim_T> *&result)
	{
		result = new TreeTrunk<Type_T,Dim_T>;
		TrunkVisitor trunkVisitor(*result);
		//
		DepthFirstSearch<Type_T,Dim_T,std::list<ITreeNode<Type_T,Dim_T> *> >(root,&trunkVisitor);
	}
	//
	TrunkGenerator(){}
	//
	virtual ~TrunkGenerator()
	{}
	//
	/*
	 * End of class
	 */
};

#endif // TRUNK_GENERATOR_H
