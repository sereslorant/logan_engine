#ifndef LR_GL_VERTEX_DATA_H
#define LR_GL_VERTEX_DATA_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLBuffer.h>

#include <lResourceManager/liResourceManager.h>

struct lrGLVertexData
{
	lrGLBuffer VertexBuffer; 	/**< A csúcspontokat tároló buffer object */
	lrGLBuffer NormalBuffer;	/**< A normálvektorokat tároló buffer object */
	lrGLBuffer TangentBuffer;	/**< A tangent vektorokat tároló buffer object */
	lrGLBuffer BitangentBuffer;	/**< A bitangent vektorokat tároló buffer object */
	lrGLBuffer TexCoordBuffer;	/**< A textúrakoordinátákat tároló buffer object */
	
	void UploadData(lrmStaticMesh &static_mesh);
	
	lrGLVertexData()
	{}
	
	~lrGLVertexData()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_VERTEX_DATA_H
