
#include "lrGLVertexData.h"

void lrGLVertexData::UploadData(lrmStaticMesh &static_mesh)
{
	VertexBuffer.UploadData(
						GL_ARRAY_BUFFER,
						static_mesh.Vertices.size()*sizeof(lmVector3D),
						&static_mesh.Vertices[0]
					);
	
	NormalBuffer.UploadData(
						GL_ARRAY_BUFFER,
						static_mesh.Normals.size()*sizeof(lmVector3D),
						&static_mesh.Normals[0]
					);
	
	TangentBuffer.UploadData(
						GL_ARRAY_BUFFER,
						static_mesh.Tangents.size()*sizeof(lmVector3D),
						&static_mesh.Tangents[0]
					);
	
	BitangentBuffer.UploadData(
						GL_ARRAY_BUFFER,
						static_mesh.Bitangents.size()*sizeof(lmVector3D),
						&static_mesh.Bitangents[0]
					);
	
	TexCoordBuffer.UploadData(
						GL_ARRAY_BUFFER,
						static_mesh.TexCoords.size()*sizeof(lmVector3D),
						&static_mesh.TexCoords[0]
					);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
