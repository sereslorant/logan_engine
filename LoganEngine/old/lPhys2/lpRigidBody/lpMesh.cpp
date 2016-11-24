
#include "lpMesh.h"

lSphereTreeFactory<8>	lpMesh::SphereOctreeFactory;
lAABBTreeFactory<8>		lpMesh::AABBOctreeFactory;
lAABBTreeFactory<4>		lpMesh::AABBQuadtreeFactory;

void lpMesh::CalculateBsInertiaTensor()
{
    if(Mass != 0.0)
    {
        /*
         * Az algoritmus a következő:
         * A triangle meshünk előáll tetaréderek halmazaként.
         *    -Felbontjuk a meshünket tetraéderekre.
         *    -Kiszámoljuk egy egyszerűen számolható tetraéder tehetetlenségi tenzorát.
         *    -Minden tetraéderre lineáris transzformációval transzformáljuk az egyszerű tetraéder tehetetlenségi tenzorát.
         *    -Ezeket a tehetetlenségi tenzorokat összegezzük, így megkapjuk a test tehetetlenségi tenzorát.
         */

        /*
         * Definiáljuk így az egyszerű tetraéderünket:
         * v0(0.0,0.0,0.0),v1(1.0,0.0,0.0),v2(0.0,1.0,0.0),v3(0.0,0.0,1.0);
         * A tenzor főátlóbeli komponenseinek értéke 1.0/60.0
         * A többi 1.0/120.0
         */
        lmMatrix3x3 C;
        for(int i=0;i < 3;i++)
        {
            for(int j=0;j < 3;j++)
            {
                if(i==j)
                {
                    C[i][j] = 1.0/60.0;
                }
                else
                {
                    C[i][j] = 1.0/120.0;
                }
            }
        }

        lmMatrix3x3 Ctotal(lmMatrix3x3::NULLMAT);
        lmVector3D CenterOfMass;

        //Erre szükségünk lesz, hogy a tömegközéppontot ki tudjuk számolni.
        lmScalar detAtotal = 0.0;

        for(auto &I : Triangles)
        {
            /*
             * Ezután minden i. tetraéderünkre elvégzünk egy A transzformációt, amely az egyszerű tetraédert áttranszformálja
             * az i. tetraéderbe.
             * Levezethető, hogy Ci = A*C*t(A)*det(A); ahol t(A) az A transzponáltja.
             * Ha az i. tetraéder csúcsai a w0(0.0,0.0,0.0),w1(w1x,w1y,w1z),w2(w2x,w2y,w2z),w3(w3x,w3y,w3z)
             * akkor az A transzformációnk:
             * {w1x,w2x,w3x}
             * {w1y,w2y,w3y}
             * {w1z,w2z,w3z}
             * Szal minden háromszögre kreálunk egy tetraédert, aminek az egyik csúcsa a mesh tömegközéppontja, (Body space-ben vagyunk)
             * a többi meg a háromszög csúcspontjai, és minden háromszögre transzformáljuk C-t.
			 *
             * Az így kapott Ci-ket a tömegük szerint súlyozva összegezzük.
             */

            lmVector3D &V1 = Vertices[I.V[0]];
            lmVector3D &V2 = Vertices[I.V[1]];
            lmVector3D &V3 = Vertices[I.V[2]];

            lmMatrix3x3 A(lmMatrix3x3::NULLMAT);
            A[0][0] = V1[0]; A[1][0] = V2[0]; A[2][0] = V3[0];
            A[0][1] = V1[1]; A[1][1] = V2[1]; A[2][1] = V3[1];
            A[0][2] = V1[2]; A[1][2] = V2[2]; A[2][2] = V3[2];

            //TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //A = A.Transpose();
            //TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!

            lmScalar detA = A.Determinant();

            Ctotal         += A * C * A.Transpose() * detA;
            //Mivel a tömegközéppont számolásánál a tömeg és a térfogat egyes konstans szorzói kiesnek, ezért
            //az egyes tetraéderek tömegközéppontjaik a transzformációjuk determinánsa arányában adódnak hozzá
            //a tömegközépponthoz.
            CenterOfMass   += detA * (V1 + V2 + V3);

            detAtotal      += detA;
        }

        CenterOfMass *= 1.0/(4.0*detAtotal);

        Ctotal *= 6.0/(detAtotal);
        Ctotal = Ctotal - Mass * lmOuter(-1*CenterOfMass,-1*CenterOfMass);

        InertiaTensor = Mass*((Ctotal.Trace() * IDENTITY_MATRIX_3X3) - Ctotal);
        InvInertiaTensor = InertiaTensor.Invert();

        /*
         * Forrás: How to find the inertia tensor (or other mass properties) of a 3D solid body represented by a triangle mesh
         *         (Draft)
         *         Jonathan Blow, Atman J Binstock
         *         9 July 2004
         */
    }
}

lpMesh::lpMesh(bool ghost,lmScalar mass,liRigidBody::liState *state,const liPhMesh *PhMesh)
    :lpRigidBody(ghost,mass,state,LP_MESH),Vertices(PhMesh->VerticesSize()),Normals(PhMesh->NormalsSize()),Triangles(PhMesh->TrianglesSize())
{
    for(unsigned int i=0;i < Vertices.size();i++)
    {
        Vertices[i] = PhMesh->GetVertex(i);
        lmScalar Tmp = Vertices[i].LengthSquared();
    }

    for(unsigned int i=0;i < Normals.size();i++)
    {
        Normals[i] = PhMesh->GetNormal(i);
    }

    for(unsigned int i=0;i < Triangles.size();i++)
    {
        const liPhMesh::liPhTriangle &Ti = PhMesh->GetTriangle(i);

        Triangles[i].V[0] = Ti.GetVertexId(0);
        Triangles[i].V[1] = Ti.GetVertexId(1);
        Triangles[i].V[2] = Ti.GetVertexId(2);

		Triangles[i].Normal = Ti.GetNormalId();

		lmVector3D Cukcsy = lmCross(Vertices[Triangles[i].V[2]] - Vertices[Triangles[i].V[0]],Vertices[Triangles[i].V[1]] - Vertices[Triangles[i].V[0]]);

		if(lmDot(Normals[Triangles[i].Normal],Cukcsy) < 0.0)
		{
			unsigned int Tmp	= Triangles[i].V[1];
			Triangles[i].V[1]	= Triangles[i].V[2];
			Triangles[i].V[2]	= Tmp;
			/*
			std::cout << i << std::endl;

			Cukcsy = lmCross(Vertices[Triangles[i].V3] - Vertices[Triangles[i].V1],Vertices[Triangles[i].V2] - Vertices[Triangles[i].V1]);
			if(!(lmDot(Normals[Triangles[i].Normal],Cukcsy) < 0.0))
			{
				std::cout << "Győzelem!" << std::endl;
			}
			*/
		}
    }

    GenerateBoundingSphere();

    CalculateBsInertiaTensor();
}

lpMesh::~lpMesh()
{

}

unsigned int lpMesh::GetVerticesSize() const
{
    return Vertices.size();
}

const lmVector3D &lpMesh::GetVertex(unsigned int i) const
{
    return Vertices[i];
}

unsigned int lpMesh::GetNormalsSize() const
{
    return Normals.size();
}

const lmVector3D &lpMesh::GetNormal(unsigned int i) const
{
    return Normals[i];
}

unsigned int lpMesh::GetTrianglesSize() const
{
    return Triangles.size();
}

const lpTriangle &lpMesh::GetTriangle(unsigned int i) const
{
    return Triangles[i];
}
