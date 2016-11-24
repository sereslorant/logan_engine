#ifndef LRM_HEIGHTMAP_LOADER_H
#define LRM_HEIGHTMAP_LOADER_H

#include "../lInterfaces/lResourceManager/liBitmap.h"
#include "../lMath/lMath.h"

#include <vector>

void LoadHeightmapVertices(liBitmap *bmp,std::vector<lmVector3D> &vertices,float TerrainX = 1.0,float TerrainY = 1.0,float TerrainZ = 1.0);
void LoadHeightmapTriangles_SM(liBitmap *bmp,int i,int j,const lmVector3D &normal_1,const lmVector3D &normal_2,std::vector<lmVector3D> &normals,std::vector<unsigned int> &index_buffer);

#endif // LRM_HEIGHTMAP_LOADER_H
