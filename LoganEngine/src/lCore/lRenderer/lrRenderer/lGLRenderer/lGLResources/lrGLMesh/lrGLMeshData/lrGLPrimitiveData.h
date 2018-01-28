#ifndef LR_GL_PRIMITIVE_GROUP_H
#define LR_GL_PRIMITIVE_GROUP_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLBuffer.h>

#include <lResourceManager/liResourceManager.h>

struct lrGLPrimitiveData
{
	unsigned int BufferLength = 0;	/**< Az index buffer hossza */
	lrGLBuffer IndexBuffer;			/**< Az index buffer azonosítója */
	
	std::string MaterialName;
	
	void UploadData(lrmStaticMesh::lrmMtlGroup &mtl_group);
	
	lrGLPrimitiveData()
	{}
	
	~lrGLPrimitiveData()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_PRIMITIVE_GROUP_H
