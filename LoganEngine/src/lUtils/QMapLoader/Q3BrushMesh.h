#ifndef BRSUH_MESH_H
#define BRUSH_MESH_H

#include "WeMesh.h"

#include "Q3Geometry.h"

#include <vector>
#include <list>

void ConstructBrushMesh(std::vector<Q3Plane> &planes,
						std::list<Q3Line> &lines,
						std::list<lmVector3D> &vertices,
						WeMesh &result);

#endif // BRUSH_MESH_H
