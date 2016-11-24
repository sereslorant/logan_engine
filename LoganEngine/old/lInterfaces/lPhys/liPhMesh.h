#ifndef LI_PH_MESH_H
#define LI_PH_MESH_H

class liPhMesh
{
public:

	class liPhTriangle
	{
	public:
		virtual unsigned int GetVertexId(unsigned int i) const = 0;
		virtual unsigned int GetNormalId() const = 0;

		liPhTriangle()
		{
			//Üres függvénytörzs
		}

		virtual ~liPhTriangle()
		{
			//Üres függvénytörzs
		}
	};

	virtual bool IsSuccessful() const = 0;

	virtual unsigned int VerticesSize() const	= 0;
	virtual unsigned int NormalsSize() const	= 0;
	virtual unsigned int TrianglesSize() const	= 0;

	virtual const lmVector3D	&GetVertex(unsigned int i) const	= 0;
	virtual const lmVector3D	&GetNormal(unsigned int i) const	= 0;
	virtual const liPhTriangle	&GetTriangle(unsigned int i) const	= 0;

	liPhMesh()
	{
		//Üres függvénytörzs
	}

	virtual ~liPhMesh()
	{
		//Üres függvénytörzs
	}
};

#endif // LI_PH_MESH_H
