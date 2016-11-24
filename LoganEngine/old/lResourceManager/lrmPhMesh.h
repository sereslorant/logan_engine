#ifndef LRM_PH_MESH_H
#define LRM_PH_MESH_H

#include "../lInterfaces/lPhys/liPhMesh.h"

#include <vector>

class lrmPhMesh : public liPhMesh
{
protected:

	class lrmPhTriangle : public liPhTriangle
	{
	public:
		unsigned int V[3];
		unsigned int Normal;

		virtual unsigned int GetVertexId(unsigned int i) const override
		{
			if(i < 3)
			{
				return V[i];
			}
			else
			{
				return 0;
			}
		}

		virtual unsigned int GetNormalId() const override
		{
			return Normal;
		}

		lrmPhTriangle()
			:Normal(0)
		{
			for(int i=0;i < 3;i++)
			{
				V[i] = 0;
			}
		}

		lrmPhTriangle(const lrmPhTriangle &t)
			:Normal(t.Normal)
		{
			for(int i=0;i < 3;i++)
			{
				V[i] = t.V[i];
			}
		}

		lrmPhTriangle(unsigned int v[],int normal)
			:Normal(normal)
		{
			for(int i=0;i < 3;i++)
			{
				V[i] = v[i];
			}
		}

		virtual ~lrmPhTriangle() override
		{
			//Üres függvénytörzs
		}
	};

	//Legyünk optimisták! :P
	bool Successful = false;

    std::vector<lmVector3D> Vertices;
    std::vector<lmVector3D> Normals;

    std::vector<lrmPhTriangle> Triangles;

public:

	virtual bool IsSuccessful() const override
	{
		return Successful;
	}

	virtual unsigned int VerticesSize() const override
	{
		return Vertices.size();
	}

	virtual unsigned int NormalsSize() const override
	{
		return Normals.size();
	}

	virtual unsigned int TrianglesSize() const override
	{
		return Triangles.size();
	}

	virtual const lmVector3D &GetVertex(unsigned int i) const override
	{
		return Vertices[i];
	}

	virtual const lmVector3D &GetNormal(unsigned int i) const override
	{
		return Normals[i];
	}

	virtual const liPhTriangle &GetTriangle(unsigned int i) const override
	{
		return Triangles[i];
	}

    lrmPhMesh()
	{
		//Üres függvénytörzs
	}

	virtual ~lrmPhMesh() override
	{
		//Üres függvénytörzs
	}
};

#endif // LRM_PH_MESH_H
