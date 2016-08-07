#ifndef I_TREE_GRAPH_H
#define I_TREE_GRAPH_H

#include "../../lMath/lMath.h"

template<typename Type_T,unsigned int Dim_T>
class ITrunkNode
{
public:
	//
	virtual const lmVectorND<Type_T,Dim_T> &GetPosition() const = 0;
	//
	//virtual void ForeachChildren(ITreeOperation<Type_T,Dim_T> *operation) = 0;
	//
	ITrunkNode()
	{}
	//
	virtual ~ITrunkNode()
	{}
};

template<typename Type_T,unsigned int Dim_T>
class IPolygonNode
{
private:

public:
	//
	virtual unsigned int Size() const = 0;
	virtual const lmVectorND<Type_T,Dim_T> &GetVertex(unsigned int i) const = 0;
	virtual const lmVectorND<Type_T,Dim_T> *GetArray() const = 0;
	//
	//virtual void ForeachChildren(ITreeOperation<Type_T,Dim_T> *operation) = 0;
	//
	IPolygonNode()
	{}
	//
	virtual ~IPolygonNode()
	{}
};

template<typename Type_T,unsigned int Dim_T>
class ITreeVisitor
{
public:
	//
	virtual void Visit(ITrunkNode<Type_T,Dim_T> *node) = 0;
	virtual void Visit(IPolygonNode<Type_T,Dim_T> *node) = 0;
	//
	ITreeVisitor(){}
	virtual ~ITreeVisitor(){}
};

template<typename Type_T,unsigned int Dim_T>
class ITreeNode
{
public:
	class ITreeOperation
	{
	public:
		virtual void Operation(ITreeNode<Type_T,Dim_T> *Node) = 0;

		ITreeOperation(){}
		virtual ~ITreeOperation(){}
	};
	//
	virtual void Accept(ITreeVisitor<Type_T,Dim_T> *visitor) = 0;
	virtual void ForeachChildren(ITreeOperation *operation) = 0;
	//
	ITreeNode()
	{}
	//
	virtual ~ITreeNode()
	{}
};
/*
template<typename Type_T,unsigned int Dim_T,class FifoWrapper>
void Search(ITreeNode<Type_T,Dim_T> *root,ITreeVisitor<Type_T,Dim_T> *visitor)
{
	FifoWrapper fifoWrapper;
	fifoWrapper.fifo.push_back(root);
	//
	do
	{
		for(auto I = fifoWrapper.fifo.begin();I != fifoWrapper.fifo.end();)
		{
			ITreeNode<Type_T,Dim_T> *node = *I;
			fifoWrapper.fifo.pop_front();
			//
			node->Accept(visitor);
			node->ForeachChildren(&fifoWrapper);
			//
			I = fifoWrapper.fifo.begin();
		}
		//
	}
	while(fifoWrapper.fifo.size() != 0);
}
*/
template<typename Type_T,unsigned int Dim_T,class FifoWrapper>
void Search(ITreeNode<Type_T,Dim_T> *root,typename ITreeNode<Type_T,Dim_T>::ITreeOperation *operation)
{
	FifoWrapper fifoWrapper;
	fifoWrapper.fifo.push_back(root);
	//
	do
	{
		for(auto I = fifoWrapper.fifo.begin();I != fifoWrapper.fifo.end();)
		{
			ITreeNode<Type_T,Dim_T> *node = *I;
			fifoWrapper.fifo.pop_front();
			//
			operation->Operation(node);
			node->ForeachChildren(&fifoWrapper);
			//
			I = fifoWrapper.fifo.begin();
		}
		//
	}
	while(fifoWrapper.fifo.size() != 0);
}

template<typename Type_T,unsigned int Dim_T,class stl_container_T>
void DepthFirstSearch(ITreeNode<Type_T,Dim_T> *root,typename ITreeNode<Type_T,Dim_T>::ITreeOperation *operation)
{
	struct DFS_FifoWrapper : public ITreeNode<Type_T,Dim_T>::ITreeOperation
	{
		stl_container_T fifo;
		//
		virtual void Operation(ITreeNode<Type_T,Dim_T> *node) override {fifo.push_front(node);}
		//
		DFS_FifoWrapper(){}
		virtual ~DFS_FifoWrapper() override {}
	};
	//
	Search<Type_T,Dim_T,DFS_FifoWrapper>(root,operation);
}

template<typename Type_T,unsigned int Dim_T,class stl_container_T>
void BreadthFirstSearch(ITreeNode<Type_T,Dim_T> *root,typename ITreeNode<Type_T,Dim_T>::ITreeOperation *operation)
{
	struct BFS_FifoWrapper : public ITreeNode<Type_T,Dim_T>::ITreeOperation
	{
		stl_container_T fifo;
		//
		virtual void Operation(ITreeNode<Type_T,Dim_T> *node) override {fifo.push_back(node);}
		//
		BFS_FifoWrapper(){}
		virtual ~BFS_FifoWrapper() override {}
	};
	//
	Search<Type_T,Dim_T,BFS_FifoWrapper>(root,operation);
}

#endif // I_TREE_GRAPH_H
