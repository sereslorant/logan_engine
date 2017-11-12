#ifndef LR_GL_PRIMITIVE_GROUP_H
#define LR_GL_PRIMITIVE_GROUP_H

#include "../lrGLBuffer.h"

#include <lResourceManager/liResourceManager.h>

struct lrGLPrimitiveGroup
{
	unsigned int BufferLength = 0;	/**< Az index buffer hossza */
	lrGLBuffer IndexBuffer;			/**< Az index buffer azonosítója */
	
	std::string MaterialName;
	
	void UploadData(lrmStaticMesh::lrmMtlGroup &mtl_group)
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
	
	lrGLPrimitiveGroup()
	{}
	
	~lrGLPrimitiveGroup()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_PRIMITIVE_GROUP_H
