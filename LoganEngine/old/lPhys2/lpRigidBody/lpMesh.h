#ifndef LP_MESH_H
#define LP_MESH_H

#include "lpRigidBody.h"
#include "../../lInterfaces/lPhys/liPhMesh.h"

/*TMP!!!!!!!!!!*/
#include <cmath>
/*TMP!!!!!!!!!!*/



#include <vector>

/** \brief Egy háromszög csúcspontjainak és normálvektorainak a tömbindexét indexét tárolja.
 *
 * \author Seres Lóránt Gábor
 *
 */
struct lpTriangle : public liMesh::liTriangle
{
    unsigned int V[3];
    unsigned int Normal;

	virtual unsigned int GetVertex(unsigned int i) const override
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

	virtual unsigned int GetNormal() const override
	{
		return Normal;
	}

	lpTriangle(){}
	lpTriangle(unsigned int v0,unsigned int v1,unsigned int v2,unsigned int normal)
	{
		V[0] = v0;V[1] = v1;V[2] = v2; Normal = normal;
	}
	virtual ~lpTriangle(){}
};

/*
class lpTriangleCache
{
private:
	bool Valid = false;
	lpTriangle T;

	lmVector3D Vertices[3];
	lmVector3D Normal;

public:

	bool IsValid(){return Valid;}
	void Invalidate(){Valid = false;}

	void Update(lpMesh *mesh,liState *state)
	{
		Normal = lmQuaternionRotate(mesh->GetNormal(T.Normal),state->GetOrientationQuaternion());

		for(int i=0;i <3;i++)
		{
			Vertices[i] = lmQuaternionRotate(mesh->GetVertex(T.V[i]),state->GetOrientationQuaternion()) + state->GetPosition();
		}
	}

	lpTriangleCache(){}
	~lpTriangleCache(){}
};
*/
#include <list>


struct lpTriangleCacheVector
{
	bool Initialized = false;
	lmVector3D Vector;
};

struct lpTriangleMeshCache
{
	std::vector<lpTriangleCacheVector> Vertices;
	std::vector<lpTriangleCacheVector> Normals;
};

class lpTriangleCache : public liTriangleCache
{
private:
	bool Valid = false;
	lpTriangle T;

	//lmVector3D Vertices[3];
	//lmVector3D Normal;

	lpTriangleMeshCache *MeshCache = nullptr;

public:

	void SetMeshCache(lpTriangleMeshCache *mesh_cache)
	{
		MeshCache = mesh_cache;
	}

	virtual bool IsValid() override {return Valid;}

	virtual void Invalidate() override
	{
		if(Valid)
		{
			if(MeshCache != nullptr)
			{
				MeshCache->Normals[T.Normal].Initialized = false;

				for(int i=0;i <3;i++)
				{
					MeshCache->Vertices[T.V[i]].Initialized = false;
				}
			}
		}

		Valid = false;
	}

	virtual void Update(liMesh *mesh,const lmVector3D &position,const lmQuaternion &orientation) override
	{
		if(MeshCache != nullptr)
		{
			if(MeshCache->Vertices.size() != mesh->GetVerticesSize())
				{MeshCache->Vertices.resize(mesh->GetVerticesSize());}
			if(MeshCache->Normals.size() != mesh->GetNormalsSize())
				{MeshCache->Normals.resize(mesh->GetNormalsSize());}

			//Normal = lmQuaternionRotate(mesh->GetNormal(T.Normal),orientation);
			if(MeshCache->Normals[T.Normal].Initialized == false)
			{
				MeshCache->Normals[T.Normal].Vector = lmQuaternionRotate(mesh->GetNormal(T.Normal),orientation);
				MeshCache->Normals[T.Normal].Initialized = true;
			}

			for(int i=0;i <3;i++)
			{
				if(MeshCache->Vertices[T.V[i]].Initialized == false)
				{
					MeshCache->Vertices[T.V[i]].Vector = lmQuaternionRotate(mesh->GetVertex(T.V[i]),orientation) + position;
					MeshCache->Vertices[T.V[i]].Initialized = true;
				}
			}

			Valid = true;
		}
	}

	virtual void SetTriangle(const liMesh::liTriangle &t) override
	{
		T.Normal = t.GetNormal();

		for(int i=0;i < 3;i++)
		{
			T.V[i] = t.GetVertex(i);
		}
	}

	virtual const lmVector3D &GetNormal() override
	{
		if(MeshCache != nullptr)
		{
			//return Normal;
			return MeshCache->Normals[T.Normal].Vector;
		}
		else
		{
			return {0.0,0.0,0.0};
		}
	}

	virtual const lmVector3D &GetVertex(unsigned int i) override
	{
		if(i < 3)
		{
			if(MeshCache != nullptr)
			{
				//return Vertices[i];
				return MeshCache->Vertices[T.V[i]].Vector;
			}
			else
			{
				return {0.0,0.0,0.0};
			}
		}
		else
		{
			return {0.0,0.0,0.0};
		}
	}

	lpTriangleCache()
	{}

	virtual ~lpTriangleCache() override
	{}
};

template<unsigned int N_CHILDREN>
class lBoundingVolume : public liBoundingVolume
{
protected:
	lpTriangleMeshCache MeshCache;
	std::vector<lpTriangleCache> Data;

	lBoundingVolume *Parent = nullptr;
	liBoundingVolume *Children[N_CHILDREN];

public:

	lpTriangleMeshCache *GetMeshCache()
	{
		if(Parent == nullptr)
		{
			return &MeshCache;
		}
		else
		{
			Parent->GetMeshCache();
		}
	}

	virtual void ResizeData(unsigned int size) override
	{
		unsigned int prev_size = Data.size();
		Data.resize(size);

		if(prev_size < size)
		{
			lpTriangleMeshCache *ParentMeshCache = GetMeshCache();
			for(unsigned int i=prev_size;i < Data.size();i++)
			{
				Data[i].SetMeshCache(ParentMeshCache);
			}
		}
	}

	virtual void InsertData(const liMesh::liTriangle &data,unsigned int i) override
	{
		Data[i].SetTriangle(data);
	}

	virtual unsigned int GetDataSize()
	{
		return Data.size();
	}

	virtual liTriangleCache *GetData(unsigned int i)
	{
		if(i < Data.size())
		{
			return &Data[i];
		}
		else
		{
			return nullptr;
		}
	}

	virtual void InvalidateCache() override
	{
		for(int i=0;i < Data.size();i++)
		{
			Data[i].Invalidate();
		}

		for(int i=0;i < N_CHILDREN;i++)
		{
			if(Children[i] != nullptr)
			{
				Children[i]->InvalidateCache();
			}
		}
	}

	virtual void SetParent(liBoundingVolume *parent) override
	{
		Parent = (lBoundingVolume *)parent;
	}

	virtual liBoundingVolume *GetParent() override
	{
		return Parent;
	}

	virtual unsigned int GetNumChildren() override
	{
		return N_CHILDREN;
	}

	virtual void AddChild(liBoundingVolume *child,unsigned int i) override
	{
		if(i < N_CHILDREN)
		{
			Children[i] = child;
			child->SetParent(this);
		}
	}

	virtual liBoundingVolume *GetChild(unsigned int i) override
	{
		if(i < N_CHILDREN)
		{
			return Children[i];
		}
	}

	lBoundingVolume()
	{
		for(unsigned int i=0;i < N_CHILDREN;i++)
		{
			Children[i] = nullptr;
		}
	}

	virtual ~lBoundingVolume() override
	{
		for(auto I : Children)
		{
			delete I;
		}
	}
};

template<unsigned int N_CHILDREN>
class lSphereTree : public lBoundingVolume<N_CHILDREN>,public liSphere
{
private:
	lmVector3D Position;
	lmScalar Radius;

public:

	virtual liBoundingVolume::lpBvType GetType() override {return liBoundingVolume::LP_SPHERE_TREE;}

	virtual liSphere *ToSphere() override
	{
		return this;
	}

	virtual liAABB *ToAABB() override
	{
		return nullptr;
	}

	virtual const lmVector3D &GetPosition() const override
	{
		return Position;
	}

	virtual void SetPosition(const lmVector3D &position) override
	{
		Position = position;
	}

	virtual lmScalar GetRadius() const override
	{
		return Radius;
	}

	virtual void SetRadius(lmScalar radius) override
	{
		Radius = radius;
	}

	lSphereTree(const lmVector3D &position,lmScalar radius)
		:Position(position),Radius(radius)
	{}

	virtual ~lSphereTree() override
	{}
};

template<unsigned int N_CHILDREN>
class lSphereTreeFactory : public liBoundingVolumeFactory
{
public:

	virtual liBoundingVolume *CreateBoundingVolume(const std::vector<lmVector3D> &vertices,const std::list<liMesh::liTriangle *> triangle_list,const lmVector3D &position) override
	{
		lmScalar SphereRadius = 0.0;

		for(liMesh::liTriangle *T : triangle_list)
		{
			for(unsigned int i=0;i < 3;i++)
			{
				lmVector3D V = vertices[T->GetVertex(i)] - position;
				lmScalar Tmp = V.LengthSquared();

				if(SphereRadius < Tmp)
				{SphereRadius = Tmp;}
			}
		}

		SphereRadius = std::sqrt(SphereRadius);

		liBoundingVolume *NewSphereNode = new lSphereTree<N_CHILDREN>(position,SphereRadius);
		return NewSphereNode;
	}

	lSphereTreeFactory()
	{}

	virtual ~lSphereTreeFactory()
	{}
};

template<unsigned int N_CHILDREN>
class lAABBTree : public lBoundingVolume<N_CHILDREN>,public liAABB
{
private:
	lmVector3D Min;
	lmVector3D Max;
	/*
	std::vector<lpTriangle> Data;

	lAABBTree *Parent;
	std::list<lAABBTree *> Children;
	*/
public:

	virtual liBoundingVolume::lpBvType GetType() override {return liBoundingVolume::LP_AABB_TREE;}

	virtual liSphere *ToSphere() override
	{
		return nullptr;
	}

	virtual liAABB *ToAABB() override
	{
		return this;
	}

	virtual const lmVector3D &GetMin() const override
	{
		return Min;
	}

	virtual const lmVector3D &GetMax() const override
	{
		return Max;
	}

	virtual void SetMin(const lmVector3D &aabb_min) override
	{
		Min = aabb_min;
	}

	virtual void SetMax(const lmVector3D &aabb_max) override
	{
		Max = aabb_max;
	}

	virtual lmVector3D GetCenter() override
	{
		return (Min + Max) * (1.0/2.0);
	}

	lAABBTree(const lmVector3D &aabb_min,const lmVector3D &aabb_max)
		:Min(aabb_min),Max(aabb_max)
	{}

	virtual ~lAABBTree() override
	{}
};

template<unsigned int N_CHILDREN>
class lAABBTreeFactory : public liBoundingVolumeFactory
{
public:

	virtual liBoundingVolume *CreateBoundingVolume(const std::vector<lmVector3D> &vertices,const std::list<liMesh::liTriangle *> triangle_list,const lmVector3D &position) override
	{
		lmVector3D Min = vertices[triangle_list.back()->GetVertex(0)];
		lmVector3D Max = vertices[triangle_list.back()->GetVertex(0)];

		for(liMesh::liTriangle *T : triangle_list)
		{
			for(int i=0;i < 3;i++)
			{
				if(vertices[T->GetVertex(i)][0] > Max[0]){Max[0] = vertices[T->GetVertex(i)][0];}
				if(vertices[T->GetVertex(i)][1] > Max[1]){Max[1] = vertices[T->GetVertex(i)][1];}
				if(vertices[T->GetVertex(i)][3] > Max[3]){Max[3] = vertices[T->GetVertex(i)][3];}

				if(vertices[T->GetVertex(i)][0] < Min[0]){Min[0] = vertices[T->GetVertex(i)][0];}
				if(vertices[T->GetVertex(i)][1] < Min[1]){Min[1] = vertices[T->GetVertex(i)][1];}
				if(vertices[T->GetVertex(i)][3] < Min[3]){Min[3] = vertices[T->GetVertex(i)][3];}
			}
		}

		liBoundingVolume *NewAABBNode = new lAABBTree<N_CHILDREN>(Min,Max);
		return NewAABBNode;
	}

	lAABBTreeFactory()
	{}

	virtual ~lAABBTreeFactory()
	{}
};


/** \brief Egy mesht modellező objektum. Az lpRigidBodyból származtatott
 *
 * \todo Csináld meg rendesen a mesh tehetetlensgi tenzorát!
 *
 * \author Seres Lóránt Gábor
 *
 */
class lpMesh : public lpRigidBody,public liMesh
{
protected:
    std::vector<lmVector3D> Vertices;
    std::vector<lmVector3D> Normals;

    std::vector<lpTriangle> Triangles;

    liBoundingVolume *BoundingVolume = nullptr;
    //lmScalar BoundingSphereRadius;
    //lSphereTree *BoundingSphere = nullptr;

    lpMesh(bool ghost,lmScalar mass,liRigidBody::liState *state,lpType type)
		:lpRigidBody(ghost,mass,state,type)
    {

    }

private:

    /** \brief Kiszámolja a háromszögháló által határolt mesh tehetetlenségi tenzorát. Bővebb infó a függvénytörzs kommentjeiben van.
     *
     * \return void
     *
     */
    void CalculateBsInertiaTensor();

    static lSphereTreeFactory<8>	SphereOctreeFactory;
    static lAABBTreeFactory<8>		AABBOctreeFactory;
    static lAABBTreeFactory<4>		AABBQuadtreeFactory;

    liBoundingVolume *BV_QuadtreePartitioning(unsigned int elements_size,std::list<liTriangle *> &triangle_list,/*const lmVector3D &parent_center,*/const lmVector3D &position,const lmVector3D &sp_min,const lmVector3D &sp_max,liBoundingVolumeFactory *bv_factory)
    {
		liBoundingVolume *NewBoundingVolume = bv_factory->CreateBoundingVolume(Vertices,triangle_list,position);
    	if((triangle_list.size() <= elements_size) || ((sp_max - sp_min).LengthSquared() < 1e-4))
		{
			NewBoundingVolume->ResizeData(triangle_list.size());
			unsigned int i=0;
			for(liTriangle *T : triangle_list)
			{
				NewBoundingVolume->InsertData(*T,i);
				i++;
			}
		}
		else
		{
			lmVector3D PartitionMin[4];
			lmVector3D PartitionMax[4];

			// Back_Left
			PartitionMin[0] =  sp_min;
			PartitionMax[0] = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[0][1] = sp_max[1];

			// Back_Right
			PartitionMin[1]   =  sp_min;
			PartitionMin[1][0] = (sp_max[0] + sp_min[0]) * (1.0/2.0);
			PartitionMax[1]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[1][0] =  sp_max[0];
			PartitionMax[1][1] = sp_max[1];

			// Front_Left
			PartitionMin[2]   =  sp_min;
			PartitionMin[2][3] = (sp_max[3] + sp_min[3]) * (1.0/2.0);
			PartitionMax[2]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[2][3] =  sp_max[3];
			PartitionMax[2][1] = sp_max[1];

			// Front_Right_Up
			PartitionMin[3] = (sp_max + sp_min) * (1.0/2.0);
			PartitionMin[3][1] = sp_min[1];
			PartitionMax[3] =  sp_max;

			std::list<liTriangle *> NewTriangleList[4];
			lmVector3D TriangleListCenter[4];

			for(int i=0;i < 4;i++)
			{
				//std::list<lpTriangle *> NewTriangleList;
				//lmVector3D TriangleListCenter;
				//for(lpTriangle *T : triangle_list)

				for(auto I = triangle_list.begin();I != triangle_list.end();)
				{
					liTriangle *T = *I;

					bool Added = false;
					for(int j=0;j <3;j++)
					{
						if(!Added)
						{
							if(lmPointAABBCollision(Vertices[T->GetVertex(j)],PartitionMin[i],PartitionMax[i]))
							{
								NewTriangleList[i].push_back(T);
								Added = true;
							}
						}
					}

					if(Added)
					{
						TriangleListCenter[i] += (Vertices[T->GetVertex(0)] + Vertices[T->GetVertex(1)] + Vertices[T->GetVertex(2)]);

						auto J = std::next(I);
						triangle_list.erase(I);
						I = J;
					}
					else
					{
						I++;
					}

				}
			}

			for(int i=0;i < 4;i++)
			{
				if(NewTriangleList[i].size() != 0)
				{
					TriangleListCenter[i] *= 1.0/(3.0*NewTriangleList[i].size());
					liBoundingVolume *Child = BV_QuadtreePartitioning(elements_size,NewTriangleList[i]/*,center*/,TriangleListCenter[i],PartitionMin[i],PartitionMax[i],bv_factory);
					NewBoundingVolume->AddChild(Child,i);
				}
			}
		}

		return NewBoundingVolume;
    }

    liBoundingVolume *BV_OctreePartitioning(unsigned int elements_size,std::list<liTriangle *> &triangle_list,/*const lmVector3D &parent_center,*/const lmVector3D &position,const lmVector3D &sp_min,const lmVector3D &sp_max,liBoundingVolumeFactory *bv_factory)
    {
    	/*
		lmVector3D Min = Vertices[triangle_list.back()->V[0]];
		lmVector3D Max = Vertices[triangle_list.back()->V[0]];

		for(lpTriangle *T : triangle_list)
		{
			for(int i=0;i < 3;i++)
			{
				if(Vertices[T->V[i]].X > Max.X){Max.X = Vertices[T->V[i]].X;}
				if(Vertices[T->V[i]].Y > Max.Y){Max.Y = Vertices[T->V[i]].Y;}
				if(Vertices[T->V[i]].Z > Max.Z){Max.Z = Vertices[T->V[i]].Z;}

				if(Vertices[T->V[i]].X < Min.X){Min.X = Vertices[T->V[i]].X;}
				if(Vertices[T->V[i]].Y < Min.Y){Min.Y = Vertices[T->V[i]].Y;}
				if(Vertices[T->V[i]].Z < Min.Z){Min.Z = Vertices[T->V[i]].Z;}
			}
		}

		lAABBTree *NewAABBNode = new lAABBTree(Min,Max);
		*/
		liBoundingVolume *NewBoundingVolume = bv_factory->CreateBoundingVolume(Vertices,triangle_list,position);
    	if((triangle_list.size() <= elements_size) || ((sp_max - sp_min).LengthSquared() < 1e-4))
		{
			NewBoundingVolume->ResizeData(triangle_list.size());
			unsigned int i=0;
			for(liTriangle *T : triangle_list)
			{
				NewBoundingVolume->InsertData(*T,i);
				i++;
			}
		}
		else
		{
			lmVector3D PartitionMin[8];
			lmVector3D PartitionMax[8];

			// Back_Left_Down
			PartitionMin[0] =  sp_min;
			PartitionMax[0] = (sp_max + sp_min) * (1.0/2.0);

			// Back_Left_Up
			PartitionMin[1]   =  sp_min;
			PartitionMin[1][1] = (sp_max[1] + sp_min[1]) * (1.0/2.0);
			PartitionMax[1]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[1][1] =  sp_max[1];

			// Back_Right_Down
			PartitionMin[2]   =  sp_min;
			PartitionMin[2][0] = (sp_max[0] + sp_min[0]) * (1.0/2.0);
			PartitionMax[2]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[2][0] =  sp_max[0];

			// Front_Left_Down
			PartitionMin[3]   =  sp_min;
			PartitionMin[3][3] = (sp_max[3] + sp_min[3]) * (1.0/2.0);
			PartitionMax[3]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[3][3] =  sp_max[3];

			// Front_Right_Up
			PartitionMin[4] = (sp_max + sp_min) * (1.0/2.0);
			PartitionMax[4] =  sp_max;

			// Front_Right_Down
			PartitionMin[5]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMin[5][1] =  sp_min[1];
			PartitionMax[5]   =  sp_max;
			PartitionMax[5][1] = (sp_max[1] + sp_min[1]) * (1.0/2.0);

			// Front_Left_Up
			PartitionMin[6]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMin[6][0] =  sp_min[0];
			PartitionMax[6]   =  sp_max;
			PartitionMax[6][0] = (sp_max[0] + sp_min[0]) * (1.0/2.0);

			// Back_Right_Up
			PartitionMin[7]   = (sp_max + sp_min) * (1.0/2.0);
			PartitionMin[7][3] =  sp_min[3];
			PartitionMax[7]   =  sp_max;
			PartitionMax[7][3] = (sp_max[3] + sp_min[3]) * (1.0/2.0);

			std::list<liTriangle *> NewTriangleList[8];
			lmVector3D TriangleListCenter[8];

			for(int i=0;i < 8;i++)
			{
				//std::list<lpTriangle *> NewTriangleList;
				//lmVector3D TriangleListCenter;
				//for(lpTriangle *T : triangle_list)

				for(auto I = triangle_list.begin();I != triangle_list.end();)
				{
					liTriangle *T = *I;

					bool Added = false;
					for(int j=0;j <3;j++)
					{
						if(!Added)
						{
							if(lmPointAABBCollision(Vertices[T->GetVertex(j)],PartitionMin[i],PartitionMax[i]))
							{
								NewTriangleList[i].push_back(T);
								Added = true;
							}
						}
					}

					if(Added)
					{
						TriangleListCenter[i] += (Vertices[T->GetVertex(0)] + Vertices[T->GetVertex(1)] + Vertices[T->GetVertex(2)]);

						auto J = std::next(I);
						triangle_list.erase(I);
						I = J;
					}
					else
					{
						I++;
					}

				}
			}

			for(int i=0;i < 8;i++)
			{
				if(NewTriangleList[i].size() != 0)
				{
					TriangleListCenter[i] *= 1.0/(3.0*NewTriangleList[i].size());
					liBoundingVolume *Child = BV_OctreePartitioning(elements_size,NewTriangleList[i]/*,center*/,TriangleListCenter[i],PartitionMin[i],PartitionMax[i],bv_factory);
					NewBoundingVolume->AddChild(Child,i);
				}
			}
		}

		return NewBoundingVolume;
    }

public:

    //Arra kell, hogy letiltsuk a copy konstruktort és az egyenlőség operátort.
	lpMesh(const lpMesh &m) = delete;
    lpMesh &operator=(const lpMesh &m) = delete;

    /** \brief Meshnek létrehozott konstruktor - a geometriai típust automatikusan LP_MESH-re állítja
     *
     * \param mass lmScalar:                    A tömeg új étéke. Ha nulla, akkor a testnek végtelen lesz a tömege.
     * \param stiffness lmScalar:               A rugóállandó új értéke
     * \param damping_coefficient lmScalar:     A rugóerő csillapításának új értéke
     * \param state lpRigidBody::lpState *:     A test új állapota. (pozíció,impulzus,szögelfordulás,impulzusmomentum) Dinamikusan allokált kell, hogy legyen, felszabadításáról az lpRigidBody class gondoskodik.
     * \param PhMesh const lrmPhMesh *:       A mesh háromszöghálóját tartalmazó adatstruktúrára mutató pointer.
     *
     */
    lpMesh(bool ghost,lmScalar mass,liRigidBody::liState *state,const liPhMesh *PhMesh);

    /** \brief Destruktor
     *
     * Nem csinál semmit.
     *
     */
    virtual ~lpMesh() override;

    /** \brief Getter függvény, visszatér a csúcspontok számával.
     *
     * \return unsigned int - a csúcspontok száma.
     *
     */
    virtual unsigned int GetVerticesSize() const override;

    /** \brief Getter függvény, visszatér az i-edik csúcsponttal.
     *
     * \param i unsigned int: A csúcspont indexe;
     * \return const lmVector3D & - a csúcspontra mutató referencia.
     *
     */
    virtual const lmVector3D &GetVertex(unsigned int i) const override;

    /** \brief Getter függvény, visszatér a normálvektorok számával.
     *
     * \return unsigned int - a normálvektorok száma.
     *
     */
    virtual unsigned int GetNormalsSize() const override;

    /** \brief Getter függvény, visszatér az i-edik normálvektorral.
     *
     * \param i unsigned int: A normálvektor indexe;
     * \return const lmVector3D & - a normálvektorra mutató referencia.
     *
     */
    virtual const lmVector3D &GetNormal(unsigned int i) const override;

    /** \brief Getter függvény, visszatér a háromszögek számával.
     *
     * \return unsigned int - a háromszögek száma.
     *
     */
    virtual unsigned int GetTrianglesSize() const override;

    /** \brief Getter függvény, visszatér az i-edik háromszöggel.
     *
     * \param i unsigned int: A háromszög indexe;
     * \return const lpTriangle & - a háromszögre mutató referencia.
     *
     */
    virtual const lpTriangle &GetTriangle(unsigned int i) const override;

	virtual const lmVector3D &GetPosition() const override
	{
		return State->GetPosition();
	}

	virtual void SetPosition(const lmVector3D &position) override
	{
		State->SetPosition(position);
	}

	virtual const lmVector3D &GetOrientation() const override
	{
		return State->GetOrientation();
	}

	virtual void SetOrientation(const lmVector3D &orientation) override
	{
		State->SetOrientation(orientation);
	}

    bool HasBoundingVolume()
    {
    	return BoundingVolume != nullptr;
    }

    void GenerateBoundingSphere()
    {
		lmScalar BoundingSphereRadius = 0.0;
		for(unsigned int i=0;i < Vertices.size();i++)
		{
			lmScalar Tmp = Vertices[i].LengthSquared();

			if(BoundingSphereRadius < Tmp)
			{
				BoundingSphereRadius = Tmp;
			}
		}

		BoundingSphereRadius = sqrt(BoundingSphereRadius);
		liBoundingVolume *BoundingSphere = new lSphereTree<0>({0.0,0.0,0.0},BoundingSphereRadius);

		BoundingSphere->ResizeData(Triangles.size());
		for(unsigned int i=0;i < Triangles.size();i++)
		{
			BoundingSphere->InsertData(Triangles[i],i);
		}

		if(HasBoundingVolume())
		{
			delete BoundingVolume;
		}

		BoundingVolume = BoundingSphere;
    }

    void GenerateBoundingVolumeHierarchy(unsigned int elements_size,liBoundingVolume::lpBvType type,bool Quadtree)
    {
    	if(Triangles.size() != 0)
		{
			liBoundingVolumeFactory *BV_Factory;

			if(type == liBoundingVolume::LP_SPHERE_TREE)
			{
				BV_Factory = &SphereOctreeFactory;
			}
			else if(type == liBoundingVolume::LP_AABB_TREE)
			{
				if(Quadtree)
				{
					BV_Factory = &AABBQuadtreeFactory;
				}
				else
				{
					BV_Factory = &AABBOctreeFactory;
				}
			}
			else
			{
				return;
			}

			lmVector3D Min = Vertices[Triangles[0].V[0]];
			lmVector3D Max = Vertices[Triangles[0].V[0]];

			std::list<liTriangle *> TriangleList;
			for(lpTriangle &I : Triangles)
			{
				for(int i=0;i < 3;i++)
				{
					if(Vertices[I.V[i]][0] > Max[0]){Max[0] = Vertices[I.V[i]][0];}
					if(Vertices[I.V[i]][1] > Max[1]){Max[1] = Vertices[I.V[i]][1];}
					if(Vertices[I.V[i]][3] > Max[3]){Max[3] = Vertices[I.V[i]][3];}

					if(Vertices[I.V[i]][0] < Min[0]){Min[0] = Vertices[I.V[i]][0];}
					if(Vertices[I.V[i]][1] < Min[1]){Min[1] = Vertices[I.V[i]][1];}
					if(Vertices[I.V[i]][3] < Min[3]){Min[3] = Vertices[I.V[i]][3];}
				}

				TriangleList.push_back(&I);
			}

			if(HasBoundingVolume())
			{
				delete BoundingVolume;
			}

			if(Quadtree)
			{
				BoundingVolume = BV_QuadtreePartitioning(elements_size,TriangleList,{0.0,0.0,0.0},Min,Max,BV_Factory);
			}
			else
			{
				BoundingVolume = BV_OctreePartitioning(elements_size,TriangleList,{0.0,0.0,0.0},Min,Max,BV_Factory);
			}
		}
    }
/*
    void GenerateAABBHierarchy(unsigned int elements_size)
    {
    	if(Triangles.size() != 0)
		{
			lmVector3D Min = Vertices[Triangles[0].V[0]];
			lmVector3D Max = Vertices[Triangles[0].V[0]];

			std::list<lpTriangle *> TriangleList;
			for(lpTriangle &I : Triangles)
			{
				for(int i=0;i < 3;i++)
				{
					if(Vertices[I.V[i]].X > Max.X){Max.X = Vertices[I.V[i]].X;}
					if(Vertices[I.V[i]].Y > Max.Y){Max.Y = Vertices[I.V[i]].Y;}
					if(Vertices[I.V[i]].Z > Max.Z){Max.Z = Vertices[I.V[i]].Z;}

					if(Vertices[I.V[i]].X < Min.X){Min.X = Vertices[I.V[i]].X;}
					if(Vertices[I.V[i]].Y < Min.Y){Min.Y = Vertices[I.V[i]].Y;}
					if(Vertices[I.V[i]].Z < Min.Z){Min.Z = Vertices[I.V[i]].Z;}
				}

				TriangleList.push_back(&I);
			}

			if(HasBoundingVolume())
			{
				delete BoundingVolume;
			}

			BoundingVolume = AABBEqualPartitioning(elements_size,TriangleList,lmVector3D::NULL_VECTOR,lmVector3D::NULL_VECTOR,Min,Max);
		}
    }
*/
    liBoundingVolume *GetBoundingVolume()
    {
    	return BoundingVolume;
    }
    /*
    lSphereTree *GetBoundingSphere()
    {
    	return BoundingSphere;
    }
    */
};

#endif // LP_MESH_H
