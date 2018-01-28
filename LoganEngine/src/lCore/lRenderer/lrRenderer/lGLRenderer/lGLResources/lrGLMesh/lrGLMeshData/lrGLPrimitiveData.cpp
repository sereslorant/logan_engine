
#include "lrGLPrimitiveData.h"

void lrGLPrimitiveData::UploadData(lrmStaticMesh::lrmMtlGroup &mtl_group)
{
	BufferLength = mtl_group.IndexBuffer.size();
	
	IndexBuffer.UploadData(
					GL_ELEMENT_ARRAY_BUFFER,
					mtl_group.IndexBuffer.size() * sizeof(unsigned int),
					&mtl_group.IndexBuffer[0]
				);
	
	MaterialName = mtl_group.Material;
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
