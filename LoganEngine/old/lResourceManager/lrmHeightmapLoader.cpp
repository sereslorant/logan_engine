
#include "lrmHeightmapLoader.h"

void LoadHeightmapVertices(liBitmap *bmp,std::vector<lmVector3D> &vertices,float TerrainX,float TerrainY,float TerrainZ)
{
	vertices.resize(bmp->GetHeight() * bmp->GetWidth());

	lmVector3D NewVrt;
	for(int i=0;i < bmp->GetHeight();i++)
	{
		NewVrt[0] = ((i - (bmp->GetHeight() / 2.0)) * 2.0) / bmp->GetHeight();
		NewVrt[0] *= TerrainX;

		for(int j=0;j < bmp->GetWidth();j++)
		{
			NewVrt[2] = ((j - (bmp->GetWidth() / 2.0)) * 2.0) / bmp->GetWidth();
			NewVrt[2] *= TerrainZ;

			NewVrt[1] = (bmp->GetSampler()->GetRed(i,j) - 0.5) * 2.0;
			NewVrt[1] *= TerrainY;

			vertices[i*bmp->GetWidth() + j] = NewVrt;
		}
	}
}

void LoadHeightmapTriangles_SM(liBitmap *bmp,int i,int j,const lmVector3D &normal_1,const lmVector3D &normal_2,
							std::vector<lmVector3D> &normals,std::vector<unsigned int> &index_buffer)
{
	normals[ i	  * bmp->GetWidth() +  j   ]	+= normal_1;
	normals[ i	  * bmp->GetWidth() + (j+1)]	+= normal_1;
	normals[(i+1) * bmp->GetWidth() + (j+1)]	+= normal_1;

	normals[ i	  * bmp->GetWidth() +  j   ]	+= normal_2;
	normals[(i+1) * bmp->GetWidth() +  j   ]	+= normal_2;
	normals[(i+1) * bmp->GetWidth() + (j+1)]	+= normal_2;

	index_buffer.push_back( i	* bmp->GetWidth() +  j   );
	index_buffer.push_back( i	* bmp->GetWidth() + (j+1));
	index_buffer.push_back((i+1)* bmp->GetWidth() + (j+1));

	index_buffer.push_back( i	* bmp->GetWidth() +  j   );
	index_buffer.push_back((i+1)* bmp->GetWidth() +  j   );
	index_buffer.push_back((i+1)* bmp->GetWidth() + (j+1));
}

