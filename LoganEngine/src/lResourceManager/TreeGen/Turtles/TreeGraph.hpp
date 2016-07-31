#ifndef TREE_GRAPH_H
#define TREE_GRAPH_H

#include <vector>
#include <list>
#include "../Interfaces/ITreeGraph.hpp"

template<typename Type_T,unsigned int Dim_T>
class TreeNode : public ITreeNode<Type_T,Dim_T>
{
private:
	//int Id = -1;
	TreeNode *Parent = nullptr;
	std::list<TreeNode *> Children;
	//
public:
	//
	virtual void ForeachChildren(ITreeOperation<Type_T,Dim_T> *operation) override
	{
		for(TreeNode *Child : Children)
		{operation->Operation(Child);}
	}
	//
	/*
	TreeNode *GetParent()
	{
		return Parent;
	}
	//
	unsigned int GetNumChildren()
	{
		return Children.size();
	}*/
	/*
	std::list<TreeNode *>::iterator GetChildrenBegin()
	{
		return Children.begin();
	}

	std::list<TreeNode *>::iterator GetChildrenEnd()
	{
		return Children.end();
	}
	*/
	void AddChild(TreeNode *child)
	{
		//child->Id = Children.size();
		child->Parent = this;
		//
		Children.push_back(child);
	}
	//
	TreeNode()
	{
		/*
		 * Empty
		 */
	}
	//
	virtual ~TreeNode() override
	{
		for(auto I : Children)
		{
			delete I;
		}
	}
	//
	/*
	 * End of class
	 */
};

template<typename Type_T,unsigned int Dim_T>
class TrunkNode : public TreeNode<Type_T,Dim_T>, public ITrunkNode<Type_T,Dim_T>
{
private:
	lmVectorND<Type_T,Dim_T> Position;
	//
public:
	//
	virtual void Accept(ITreeVisitor<Type_T,Dim_T> *visitor) override
	{
		visitor->Visit(this);
	}
	//
	virtual const lmVectorND<Type_T,Dim_T> &GetPosition() const override
	{
		return Position;
	}
	//
	virtual void ForeachChildren(ITreeOperation<Type_T,Dim_T> *operation) override
	{
		TreeNode<Type_T,Dim_T>::ForeachChildren(operation);
	}
	//
	TrunkNode(const lmVectorND<Type_T,Dim_T> position)
		:Position(position)
	{
		/*
		 * Empty
		 */
	}
	//
	virtual ~TrunkNode() override
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

template<typename Type_T,unsigned int Dim_T>
class PolygonNode : public TreeNode<Type_T,Dim_T>,public IPolygonNode<Type_T,Dim_T>
{
private:
	std::vector<lmVectorND<Type_T,Dim_T> > VertexArray;
	//
	template<class Iterator>
	PolygonNode(Iterator array_begin,Iterator array_end,unsigned int array_size)
		:VertexArray(array_size)
	{
		auto I = array_begin;
		for(unsigned int i=0;i < VertexArray.size() && I != array_end;i++,I++)
		{
			VertexArray[i] = *I;
		}
	}
	//
public:
	//
	virtual void Accept(ITreeVisitor<Type_T,Dim_T> *visitor) override
	{
		visitor->Visit(this);
	}
	//
	virtual unsigned int Size() const override
	{
		return VertexArray.size();
	}
	//
	virtual const lmVectorND<Type_T,Dim_T> &GetVertex(unsigned int i) const override
	{
		return VertexArray[i];
	}
	//
	virtual const lmVectorND<Type_T,Dim_T> *GetArray() const override
	{
		return &VertexArray[0];
	}
	//
	virtual void ForeachChildren(ITreeOperation<Type_T,Dim_T> *operation) override
	{
		TreeNode<Type_T,Dim_T>::ForeachChildren(operation);
	}
	//
	class PolygonNodeBuilder
	{
	private:
		std::list<lmVectorND<Type_T,Dim_T> > VertexList;
		TreeNode<Type_T,Dim_T> *Parent;
		//
	public:
		//
		PolygonNodeBuilder &AddVertex(const lmVectorND<Type_T,Dim_T> &vertex)
		{
			VertexList.push_back(vertex);
			return *this;
		}
		//
		void Construct(TreeNode<Type_T,Dim_T> *&dest)
		{
			dest = new PolygonNode(VertexList.begin(),VertexList.end(),VertexList.size());
			Parent->AddChild(dest);
		}
		//
		PolygonNodeBuilder(TreeNode<Type_T,Dim_T> *parent)
			:Parent(parent)
		{
			/*
			 * Empty
			 */
		}
		//
		~PolygonNodeBuilder()
		{
			/*
			 * Empty
			 */
		}
	};
	//
	virtual ~PolygonNode() override
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

#endif // TREE_GRAPH_H
