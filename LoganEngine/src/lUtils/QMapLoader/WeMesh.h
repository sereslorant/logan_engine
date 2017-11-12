#ifndef WE_MESH_H
#define WE_MESH_H

#include <list>

class WeElement
{
private:
	bool Deleted = false;
	
public:
	
	void *Data = nullptr;
	
	template<class Data_T>
	Data_T *GetData()
	{
		return (Data_T *)Data;
	}

	bool IsDeleted() const
	{
		return Deleted;
	}
	
	void Delete()
	{
		Deleted = true;
	}
};

struct WeEdge;

enum WeEdgeSide
{
	WE_LEFT = 0,
	WE_RIGHT = 1,
	WE_NEITHER = -1,
};

struct WeVertex : WeElement
{
	WeEdge *OneEdge	= nullptr;
};

struct WeFace : WeElement
{
	WeEdge *OneEdge	= nullptr;
	
	WeEdgeSide GetSideOfEdge(WeEdge *edge);
	WeEdgeSide GetOtherSideOfEdge(WeEdge *edge);
};


struct WeEdge : WeElement
{
	struct WeSideData
	{
		WeFace *Face			= nullptr;
		WeEdge *Predecessor		= nullptr;
		WeEdge *Successor		= nullptr;
	};
	
	WeVertex *BeginVertex		= nullptr;
	WeVertex *EndVertex			= nullptr;
	
	WeSideData Side[2];
	
	bool IsLineSegment()
	{
		return BeginVertex != nullptr && EndVertex != nullptr;
	}
};

void WeConnectFaceBorder(WeVertex *vertex,WeFace *face,WeEdge *previous_edge,WeEdge *next_edge);

void WeAttachEdgeToFace(WeEdge *existing_edge,WeFace *face,WeEdge *new_edge,WeEdgeSide edge_side);

void WeCloseFace(WeFace *face,WeEdge *begin_edge,WeEdge *end_edge);

void WeConnectNewFace(WeEdge *existing_edge,WeVertex *new_vertex,WeFace *new_face,WeEdge *new_edges[2]);

void WeCloseFaceIntoVertex(WeFace *face,WeEdge *begin_edge,WeEdge *end_edge,WeVertex *vertex);

struct WeMesh
{
	std::list<WeVertex> Vertices;
	std::list<WeEdge> Edges;
	std::list<WeFace> Faces;
	
	WeVertex *CreateVertex()
	{
		Vertices.emplace_back();
		return &Vertices.back();
	}
	
	WeVertex *CreateVertex(void *data)
	{
		WeVertex *NewVertex = CreateVertex();
		NewVertex->Data = data;
		
		return NewVertex;
	}
	
	WeEdge *CreateEdge()
	{
		Edges.emplace_back();
		return &Edges.back();
	}
	
	WeEdge *CreateEdge(void *data)
	{
		WeEdge *NewEdge = CreateEdge();
		NewEdge->Data = data;
		
		return NewEdge;
	}
	
	WeFace *CreateFace()
	{
		Faces.emplace_back();
		return &Faces.back();
	}
	
	WeFace *CreateFace(void *data)
	{
		WeFace *NewFace = CreateFace();
		NewFace->Data = data;
		
		return NewFace;
	}
	
	void FreeDeletedElements();
	
	WeMesh()
	{}
	
	~WeMesh()
	{}
};

#endif // WE_MESH_H
