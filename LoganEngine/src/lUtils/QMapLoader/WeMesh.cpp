
#include "WeMesh.h"

WeEdgeSide WeFace::GetSideOfEdge(WeEdge *edge)
{
	if(edge->Side[WE_LEFT].Face == this)
	{
		return WE_LEFT;
	}
	else if(edge->Side[WE_RIGHT].Face == this)
	{
		return WE_RIGHT;
	}
	else
	{
		return WE_NEITHER;
	}
}

WeEdgeSide WeFace::GetOtherSideOfEdge(WeEdge *edge)
{
	if(edge->Side[WE_LEFT].Face == this)
	{
		return WE_RIGHT;
	}
	else if(edge->Side[WE_RIGHT].Face == this)
	{
		return WE_LEFT;
	}
	else
	{
		return WE_NEITHER;
	}
}

void WeConnectFaceBorder(WeVertex *vertex,WeFace *face,WeEdge *previous_edge,WeEdge *next_edge)
{
	//previous_edge->EndVertex = vertex;
	previous_edge->Side[WE_RIGHT].Face = face;
	previous_edge->Side[WE_RIGHT].Successor = next_edge;
	
	//next_edge->BeginVertex = vertex;
	next_edge->Side[WE_LEFT].Face = face;
	next_edge->Side[WE_LEFT].Predecessor = previous_edge;
	
	vertex->OneEdge = next_edge;
	face->OneEdge = next_edge;
}

void WeAttachEdgeToFace(WeEdge *existing_edge,WeFace *face,WeEdge *new_edge,WeEdgeSide edge_side)
{
	/*
	 * Ez fura, mert most az, hogy a jobb vagy a bal oldalra rakjuk-e annak semmi köze nem lenne szabad,
	 * hogy legyen ahhoz, hogy a lista elejére vagy végére szúrjuk-e be.
	 */
	if(edge_side == WE_RIGHT)
	{
		existing_edge->Side[face->GetSideOfEdge(existing_edge)].Predecessor = new_edge;
		new_edge->Side[WE_RIGHT].Successor = existing_edge;
		new_edge->Side[WE_RIGHT].Face = face;
	}
	
	if(edge_side == WE_LEFT)
	{
		existing_edge->Side[face->GetSideOfEdge(existing_edge)].Successor = new_edge;
		new_edge->Side[WE_LEFT].Predecessor = existing_edge;
		new_edge->Side[WE_LEFT].Face = face;
	}
}

void WeCloseFace(WeFace *face,WeEdge *begin_edge,WeEdge *end_edge)
{
	begin_edge->Side[face->GetSideOfEdge(begin_edge)].Predecessor = end_edge;
	end_edge->Side[face->GetSideOfEdge(end_edge)].Successor = begin_edge;
}

void WeConnectNewFace(WeEdge *existing_edge,WeVertex *new_vertex,WeFace *new_face,WeEdge *new_edges[2])
{
	for(int i=0;i < 2;i++)
	{
		new_edges[i]->BeginVertex = new_vertex;
	}
	
	/*
	 * Make the two extra edges border the new_face
	 */
	WeConnectFaceBorder(new_vertex,new_face,new_edges[0],new_edges[1]);
	
	/*
	 * Make the two extra edges emanate from existing_edge
	 */
	existing_edge->EndVertex = new_vertex;
	
	WeAttachEdgeToFace(existing_edge,existing_edge->Side[WE_RIGHT].Face,new_edges[1],new_face->GetOtherSideOfEdge(new_edges[1]));
	WeAttachEdgeToFace(existing_edge,existing_edge->Side[WE_LEFT].Face,new_edges[0],new_face->GetOtherSideOfEdge(new_edges[0]));
}

void WeCloseFaceIntoVertex(WeFace *face,WeEdge *begin_edge,WeEdge *end_edge,WeVertex *vertex)
{
	begin_edge->EndVertex = vertex;
	end_edge->EndVertex = vertex;
	
	WeCloseFace(face,begin_edge,end_edge);
}

void WeMesh::FreeDeletedElements()
{
	for(auto I = Faces.begin();I != Faces.end();)
	{
		auto J = I;
		I++;
		if(J->IsDeleted())
		{
			Faces.erase(J);
		}
	}
	
	for(auto I = Edges.begin();I != Edges.end();)
	{
		auto J = I;
		I++;
		if(J->IsDeleted())
		{
			Edges.erase(J);
		}
	}
	
	for(auto I = Vertices.begin();I != Vertices.end();)
	{
		auto J = I;
		I++;
		if(J->IsDeleted())
		{
			Vertices.erase(J);
		}
	}
}
