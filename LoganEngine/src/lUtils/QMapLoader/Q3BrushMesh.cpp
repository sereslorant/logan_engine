
#include "Q3BrushMesh.h"

#include <functional>

Q3Plane *ChooseFirstPlane(std::list<Q3Plane *> &planes_to_process)
{
	Q3Plane *FirstPlane = planes_to_process.back();
	planes_to_process.pop_back();
	
	return FirstPlane;
}

Q3Plane *ChooseSecondPlane(	Q3Plane *FirstPlane,
							std::list<Q3Plane *> &planes_to_process,
							std::list<Q3Line> &lines
							)
{
	Q3Plane *ChosenPlane = nullptr;
	for(auto I = planes_to_process.begin();I != planes_to_process.end();I++)
	{
		Q3Plane *Plane = *I;
		if(Q3PlaneIntersects(*FirstPlane,*Plane))
		{
			ChosenPlane = Plane;
			planes_to_process.erase(I);
			
			lines.push_back(Q3LineOfIntersection(*FirstPlane,*ChosenPlane));
			break;
		}
	}
	
	return ChosenPlane;
}

Q3Plane *ChooseThirdPlane(	Q3Plane *FirstPlane,
							Q3Plane *SecondPlane,
							std::list<Q3Plane *> &planes_to_process,
							std::list<Q3Line> &lines,
							std::list<lmVector3D> &vertices
							)
{
	Q3Plane *ChosenPlane = nullptr;
	for(auto I = planes_to_process.begin();I != planes_to_process.end();I++)
	{
		Q3Plane *Plane = *I;
		if( Q3PlaneIntersects(*FirstPlane,*Plane) &&
			Q3PlaneIntersects(*SecondPlane,*Plane)
			)
		{
			ChosenPlane = Plane;
			planes_to_process.erase(I);
			
			Q3FlipLine(lines.back(),*ChosenPlane);
			
			lmVector3D NewVertex;
			lmRayPlaneIntersection(	lines.back().GetPoint(),lines.back().GetDirection(),
									Plane->GetPoint(),Plane->GetNormal(),
									(float *)nullptr,
									&NewVertex
									);
			
			vertices.push_back(NewVertex);
			
			/*
			 * TODO: DEBUGOLÁSI CÉLBÓL!
			 */
			lines.back() = Q3Line(NewVertex,lines.back().GetDirection());
			/*
			 * TODO: DEBUGOLÁSI CÉLBÓL!
			 */
			
			lines.push_back(Q3LineOfIntersection(*FirstPlane,*ChosenPlane));
			Q3FlipLine(lines.back(),*SecondPlane);
			
			/*
			 * TODO: DEBUGOLÁSI CÉLBÓL!
			 */
			lines.back() = Q3Line(NewVertex,lines.back().GetDirection());
			/*
			 * TODO: DEBUGOLÁSI CÉLBÓL!
			 */
			
			lines.push_back(Q3LineOfIntersection(*SecondPlane,*ChosenPlane));
			Q3FlipLine(lines.back(),*FirstPlane);
			
			/*
			 * TODO: DEBUGOLÁSI CÉLBÓL!
			 */
			lines.back() = Q3Line(NewVertex,lines.back().GetDirection());
			/*
			 * TODO: DEBUGOLÁSI CÉLBÓL!
			 */
			
			break;
		}
	}
	
	return ChosenPlane;
}

void ConnectInitialPlanes(Q3Plane *initial_planes[3],
						  std::list<Q3Line> &lines,
						  std::list<lmVector3D> &vertices,
						  WeMesh &result
						  )
{
	WeVertex *Vertex = result.CreateVertex(&vertices.back());
	
	WeEdge *Edges[3];
	auto LineIter = lines.begin();
	for(int i=0;i < 3;i++)
	{
		Edges[i] = result.CreateEdge(&(*LineIter));
		Edges[i]->BeginVertex = Vertex;
	}
	
	WeFace *Faces[3];
	for(int i=0;i < 3;i++)
	{
		Faces[i] = result.CreateFace(initial_planes[i]);
	}
	
	WeConnectFaceBorder(Vertex,Faces[0],Edges[1],Edges[0]);
	WeConnectFaceBorder(Vertex,Faces[1],Edges[0],Edges[2]);
	WeConnectFaceBorder(Vertex,Faces[2],Edges[2],Edges[1]);
}

int CalculateNumIntersects(const Q3Plane &Plane,const std::list<WeEdge *> &RayEdges)
{
	int NumIntersects = 0;
	for(WeEdge *Edge : RayEdges)
	{
		Q3Line &Line = *Edge->GetData<Q3Line>();
		if(lmRayPlaneIntersection(Line.GetPoint(),Line.GetDirection(),Plane.GetPoint(),Plane.GetNormal()))
			{NumIntersects++;}
	}
	
	return NumIntersects;
}

void GetMaxIntersectionPlane(std::list<WeEdge *> &ray_edges,
							 lmVector3D &vertex,
							 std::list<Q3Plane *> &planes_to_process,
							 int &max_plane_intersects,
							 std::list<Q3Plane *>::iterator &max_plane_iterator
							)
{
	for(auto I = planes_to_process.begin();I != planes_to_process.end();I++)
	{
		Q3Plane *CurrentPlane = *I;
		if(!CurrentPlane->PointIsInFront(vertex)) // Csak a vertexnek háttal álló síkok között keresünk!
		{
			int CurrentPlaneIntersects = CalculateNumIntersects(*CurrentPlane,ray_edges);
			if(CurrentPlaneIntersects > max_plane_intersects)
			{
				std::cout << "Current max: " << CurrentPlaneIntersects << std::endl;
				max_plane_intersects = CurrentPlaneIntersects;
				max_plane_iterator = I;
			}
		}
		else
		{
			std::cout << "Plane is in front!" << std::endl;
		}
	}
}

WeEdge *ChooseIntersectingEdge(std::list<WeEdge *> &ray_edges,Q3Plane &max_plane)
{
	for(WeEdge *RayEdge : ray_edges)
	{
		std::cout << "Ray edge " << RayEdge->GetData<Q3Line>() << std::endl;
		std::cout << "Max plane " << &max_plane << std::endl;
		Q3Line &Line = *RayEdge->GetData<Q3Line>();
		if(lmRayPlaneIntersection(Line.GetPoint(),Line.GetDirection(),max_plane.GetPoint(),max_plane.GetNormal()))
		{
			return RayEdge;
		}
	}
	
	return nullptr;
}

Q3Plane *ChooseMaxPlane(std::list<WeEdge *> &ray_edges,lmVector3D &one_vertex,std::list<Q3Plane *> &planes_to_process)
{
	int MaxPlaneIntersects = -1;
	auto MaxPlaneIterator = planes_to_process.end();
	GetMaxIntersectionPlane(ray_edges,one_vertex,planes_to_process,MaxPlaneIntersects,MaxPlaneIterator);
	
	if(MaxPlaneIterator == planes_to_process.end())
	{std::cout << "Serious WTF happened!" << std::endl;}
	
	Q3Plane *MaxPlane = *MaxPlaneIterator;
	return MaxPlane;
}

Q3Line *Q3CreateAndFlipIntersectionLine(Q3Plane &max_plane,Q3Plane &intersecting_plane,Q3Plane &flip_plane,std::list<Q3Line> &lines)
{
	lines.push_back(Q3LineOfIntersection(intersecting_plane,max_plane));
	Q3Line *NewLine = &lines.back();
	Q3FlipLine(*NewLine,flip_plane);
	
	return NewLine;
}

std::tuple<Q3Line *,Q3Line *> CreateBorderingEdges(Q3Plane &max_plane,WeEdge &chosen_edge,std::list<Q3Line> &lines)
{
	Q3Plane *RightPlane = chosen_edge.Side[WE_RIGHT].Face->GetData<Q3Plane>();
	Q3Plane *LeftPlane = chosen_edge.Side[WE_LEFT].Face->GetData<Q3Plane>();
	
	//lines.push_back(Q3LineOfIntersection(*RightPlane,max_plane));
	Q3Line *RightLine = Q3CreateAndFlipIntersectionLine(max_plane,*RightPlane,*LeftPlane,lines);//&lines.back();
	//Q3FlipLine(*RightLine,*LeftPlane);
	
	//lines.push_back(Q3LineOfIntersection(*LeftPlane,max_plane));
	Q3Line *LeftLine = Q3CreateAndFlipIntersectionLine(max_plane,*LeftPlane,*RightPlane,lines);//&lines.back();
	//Q3FlipLine(*LeftLine,*RightPlane);
	
	return {LeftLine,RightLine};
}

lmVector3D *CreateTerminatingVertex(Q3Plane &max_plane,WeEdge &chosen_edge,std::list<lmVector3D> &vertices)
{
	Q3Line &ChosenLine = *chosen_edge.GetData<Q3Line>();
	
	vertices.emplace_back();
	lmVector3D *Vertex = &vertices.back();
	lmRayPlaneIntersection(	ChosenLine.GetPoint(),ChosenLine.GetDirection(),
							max_plane.GetPoint(),max_plane.GetNormal(),
							(float*)nullptr,Vertex
						);
	
	return Vertex;
}

WeEdge *WeGetEndOfFace(WeFace *face)
{
	WeEdge *IterEdge = face->OneEdge;
	while(IterEdge->Side[face->GetSideOfEdge(IterEdge)].Successor != nullptr)
		{IterEdge = IterEdge->Side[face->GetSideOfEdge(IterEdge)].Successor;}
	
	return IterEdge;
}

WeEdge *WeGetBeginOfFace(WeFace *face)
{
	WeEdge *IterEdge = face->OneEdge;
	while(IterEdge->Side[face->GetSideOfEdge(IterEdge)].Predecessor != nullptr)
		{IterEdge = IterEdge->Side[face->GetSideOfEdge(IterEdge)].Predecessor;}
	
	return IterEdge;
}

void CloseInitialMesh(	std::list<Q3Plane *> &planes_to_process,
						std::list<Q3Line> &lines,
						std::list<lmVector3D> &vertices,
						WeMesh &result
					 )
{
	std::list<WeEdge *> RayEdges;
	for(WeEdge &Edge : result.Edges)
		{RayEdges.push_back(&Edge);}
	
	lmVector3D &Vertex = *result.Vertices.back().GetData<lmVector3D>();
	
	while(RayEdges.size() > 0)
	{
		std::cout << "New iteration " << planes_to_process.size() << std::endl;
		Q3Plane *MaxPlane = ChooseMaxPlane(RayEdges,Vertex,planes_to_process);
		WeEdge *ChosenEdge = ChooseIntersectingEdge(RayEdges,*MaxPlane);
		
		lmVector3D *NewVertexData = CreateTerminatingVertex(*MaxPlane,*ChosenEdge,vertices);
		
		Q3Line *LeftLine;
		Q3Line *RightLine;
		std::tie(LeftLine,RightLine) = CreateBorderingEdges(*MaxPlane,*ChosenEdge,lines);
		
		/*
		 * TODO: DEBUGOLÁSI CÉLBÓL!
		 */
		*LeftLine	= Q3Line(*NewVertexData,LeftLine->GetDirection());
		*RightLine	= Q3Line(*NewVertexData,RightLine->GetDirection());
		/*
		 * TODO: DEBUGOLÁSI CÉLBÓL!
		 */
		
		WeVertex	*NewVertex	= result.CreateVertex(NewVertexData);
		WeFace		*NewFace	= result.CreateFace(MaxPlane);
		WeEdge		*LeftEdge	= result.CreateEdge(LeftLine);
		WeEdge		*RightEdge	= result.CreateEdge(RightLine);
		
		{
			WeEdge		*NewEdges[2]	= {LeftEdge,RightEdge};
			WeConnectNewFace(ChosenEdge,NewVertex,NewFace,NewEdges);
		}
		RayEdges.remove(ChosenEdge);
		planes_to_process.remove(MaxPlane);
		
		bool ClosedLoop = false;
		{
			bool Running = true;
			
			WeEdge *CurrentEdge		= RightEdge;//NewEdges[1];
			WeFace *CurrentFace		= ChosenEdge->Side[WE_RIGHT].Face;
			WeEdge *CurrentFaceRay	= WeGetEndOfFace(CurrentFace);
			do
			{
				WeFace *NextFace = CurrentFaceRay->Side[CurrentFace->GetOtherSideOfEdge(CurrentFaceRay)].Face;
				
				Q3Line &CurrentLine		= *CurrentEdge->GetData<Q3Line>();
				Q3Plane &NextPlane		= *NextFace->GetData<Q3Plane>();
				
				lmVector3D IntersectionPoint;
				if(	lmRayPlaneIntersection(
						CurrentLine.GetPoint(),CurrentLine.GetDirection(),
						NextPlane.GetPoint(),NextPlane.GetNormal(),
						(float *)nullptr,&IntersectionPoint
					)
					&& lmDot(NextPlane.GetNormal(),CurrentLine.GetDirection()) > 0.0)
				{
					vertices.push_back(IntersectionPoint);
					WeVertex *NewVertex = result.CreateVertex(&vertices.back());
					
					WeCloseFaceIntoVertex(CurrentFace,CurrentEdge,CurrentFaceRay,NewVertex);
					RayEdges.remove(CurrentFaceRay);
					
					if(NextFace == ChosenEdge->Side[WE_LEFT].Face)
					{
						LeftEdge->EndVertex = NewVertex;
						
						WeCloseFace(NewFace,LeftEdge,CurrentEdge);
						
						WeCloseFace(LeftEdge->Side[WE_LEFT].Face,CurrentFaceRay,LeftEdge);
						
						ClosedLoop = true;
						Running = false;
					}
					else
					{
						Q3Plane &CurrentPlane	= *CurrentFace->GetData<Q3Plane>();
						Q3Line *NewLine = Q3CreateAndFlipIntersectionLine(*MaxPlane,NextPlane,CurrentPlane,lines);
						
						WeEdge *NewEdge = result.CreateEdge(NewLine);
						NewEdge->BeginVertex = NewVertex;
						
						
						/*
						* TODO: DEBUGOLÁSI CÉLBÓL!
						*/
						*NewLine	= Q3Line(IntersectionPoint,NewLine->GetDirection());
						/*
						* TODO: DEBUGOLÁSI CÉLBÓL!
						*/
						
						
						/*
						* Setting up succeeding state
						*/
						
						WeEdge *PreviousEdge	= CurrentEdge;
						WeEdge *PreviousFaceRay	= CurrentFaceRay;
						
						CurrentEdge		= NewEdge;
						CurrentFace		= NextFace;
						CurrentFaceRay	= WeGetEndOfFace(NextFace);
						
						WeAttachEdgeToFace(PreviousEdge,NewFace,CurrentEdge,WE_LEFT);
						
						WeAttachEdgeToFace(PreviousFaceRay,CurrentFace,CurrentEdge,NewFace->GetOtherSideOfEdge(CurrentEdge));
					}
				}
				else
				{
					RayEdges.push_back(CurrentEdge);
					Running = false;
				}
			}
			while (Running);
		}
		
		if(!ClosedLoop)
		{
			std::cout << "Loop not closed!" << std::endl;
			
			bool Running = true;
			
			WeEdge *CurrentEdge		= LeftEdge;
			WeFace *CurrentFace		= ChosenEdge->Side[WE_LEFT].Face;
			WeEdge *CurrentFaceRay	= WeGetBeginOfFace(CurrentFace);
			do
			{
				WeFace *NextFace = CurrentFaceRay->Side[CurrentFace->GetOtherSideOfEdge(CurrentFaceRay)].Face;
				
				Q3Line &CurrentLine		= *CurrentEdge->GetData<Q3Line>();
				Q3Plane &NextPlane		= *NextFace->GetData<Q3Plane>();
				
				lmVector3D IntersectionPoint;
				if(	lmRayPlaneIntersection(
						CurrentLine.GetPoint(),CurrentLine.GetDirection(),
						NextPlane.GetPoint(),NextPlane.GetNormal(),
						(float *)nullptr,&IntersectionPoint
					)
					&& lmDot(NextPlane.GetNormal(),CurrentLine.GetDirection()) > 0.0)
				{
					vertices.push_back(IntersectionPoint);
					WeVertex *NewVertex = result.CreateVertex(&vertices.back());
					
					WeCloseFaceIntoVertex(CurrentFace,CurrentFaceRay,CurrentEdge,NewVertex);
					RayEdges.remove(CurrentFaceRay);
					
					Q3Plane &CurrentPlane	= *CurrentFace->GetData<Q3Plane>();
					Q3Line *NewLine = Q3CreateAndFlipIntersectionLine(*MaxPlane,NextPlane,CurrentPlane,lines);
					
					WeEdge *NewEdge = result.CreateEdge(NewLine);
					NewEdge->BeginVertex = NewVertex;
					
					/*
					 * TODO: DEBUGOLÁSI CÉLBÓL!
					 */
					*NewLine	= Q3Line(IntersectionPoint,NewLine->GetDirection());
					/*
					 * TODO: DEBUGOLÁSI CÉLBÓL!
					 */
					
					/*
					 * Setting up succeeding state
					 */
					
					WeEdge *PreviousEdge	= CurrentEdge;
					WeEdge *PreviousFaceRay	= CurrentFaceRay;
					
					CurrentEdge		= NewEdge;
					CurrentFace		= NextFace;
					CurrentFaceRay	= WeGetBeginOfFace(NextFace);
					
					WeAttachEdgeToFace(PreviousEdge,NewFace,CurrentEdge,WE_RIGHT);
					
					WeAttachEdgeToFace(PreviousFaceRay,CurrentFace,CurrentEdge,NewFace->GetOtherSideOfEdge(CurrentEdge));
				}
				else
				{
					RayEdges.push_back(CurrentEdge);
					Running = false;
				}
			}
			while (Running);
		}
		
		
		
		//RayEdges.clear();
	}
}

void CreateInitialModel(std::list<Q3Plane *> &planes_to_process,
						std::list<Q3Line> &lines,
						std::list<lmVector3D> &vertices,
						WeMesh &result
					    )
{
	Q3Plane *InitialPlanes[3] = {nullptr,nullptr,nullptr};
	InitialPlanes[0] = ChooseFirstPlane(planes_to_process);
	InitialPlanes[1] = ChooseSecondPlane(InitialPlanes[0],planes_to_process,lines);
	InitialPlanes[2] = ChooseThirdPlane(InitialPlanes[0],InitialPlanes[1],
										planes_to_process,
										lines,
										vertices
										);
	/*
	std::cout << "Initial planes:" << std::endl;
	for(int i=0;i < 3;i++)
	{
		std::cout << "[" << InitialPlanes[i]->GetPoint()[0] << "," << InitialPlanes[i]->GetPoint()[1] << "," << InitialPlanes[i]->GetPoint()[2] <<  "]" << std::endl;
		std::cout << "[" << InitialPlanes[i]->GetNormal()[0] << "," << InitialPlanes[i]->GetNormal()[1] << "," << InitialPlanes[i]->GetNormal()[2] <<  "]" << std::endl;
	}
	*/
	ConnectInitialPlanes(InitialPlanes,lines,vertices,result);
	
	for(Q3Plane *Plane : InitialPlanes)
		{planes_to_process.remove(Plane);}
	
	CloseInitialMesh(planes_to_process,lines,vertices,result);
}

void Iterate(	Q3Plane *plane,
				std::list<Q3Line> &lines,
				std::list<lmVector3D> &vertices,
				WeMesh &result
				)
{
	WeVertex *SeveredVertex = nullptr;
	for(WeVertex &Vertex : result.Vertices)
	{
		if(plane->PointIsInFront(*Vertex.GetData<lmVector3D>()))
		{
			SeveredVertex = &Vertex;
			break;
		}
	}
	
	if(SeveredVertex != nullptr)
	{
		/*
		 * TODO: Rekurzívan le kell csapni az összes belőle kiinduló levágott elemet!
		 */
	}
}

void ConstructBrushMesh(std::vector<Q3Plane> &planes,
						std::list<Q3Line> &lines,
						std::list<lmVector3D> &vertices,
						WeMesh &result
					    )
{
	std::list<Q3Plane *> PlanesToProcess;
	for(Q3Plane &Plane : planes)
		{PlanesToProcess.push_back(&Plane);}
	
	CreateInitialModel(PlanesToProcess,lines,vertices,result);
	
	
	while(PlanesToProcess.size() > 0)
	{
		Q3Plane *NextPlane = PlanesToProcess.back();
		PlanesToProcess.pop_back();
		
		Iterate(NextPlane,lines,vertices,result);
	}
}
