#ifndef LM_MATRIX_4X4_H
#define LM_MATRIX_4X4_H

#include "lmMatrixNxN.hpp"
#include "lmMatrix3x3.hpp"

typedef lmVectorND<lmScalar,4> lmVector4D;

/** \brief Egy 4x4-es mátrixot definiáló osztály
 *
 * \todo A 3d-s transzformációk nincsenek dokumentálva.
 *
 * \author Seres Lóránt Gábor
 *
 */
typedef lmMatrixNxN<lmScalar,4> lmMatrix4x4;

/*
 * TODO gyorsabb determinánsszámítás kéne.
 */
template<class T>
T lmDeterminant(const lmMatrixNxN<T,4> &This)
{
	T Det;

	//lmMatrix3x3 M00;
	lmMatrixNxN<T,3> M00;
	M00[0][0] = This[1][1];M00[0][1] = This[1][2];M00[0][2] = This[1][3];
	M00[1][0] = This[2][1];M00[1][1] = This[2][2];M00[1][2] = This[2][3];
	M00[2][0] = This[3][1];M00[2][1] = This[3][2];M00[2][2] = This[3][3];

	//lmMatrix3x3 M10;
	lmMatrixNxN<T,3> M10;
	M10[0][0] = This[0][1];M10[0][1] = This[0][2];M10[0][2] = This[0][3];
	M10[1][0] = This[2][1];M10[1][1] = This[2][2];M10[1][2] = This[2][3];
	M10[2][0] = This[3][1];M10[2][1] = This[3][2];M10[2][2] = This[3][3];

	//lmMatrix3x3 M20;
	lmMatrixNxN<T,3> M20;
	M20[0][0] = This[0][1];M20[0][1] = This[0][2];M20[0][2] = This[0][3];
	M20[1][0] = This[1][1];M20[1][1] = This[1][2];M20[1][2] = This[1][3];
	M20[2][0] = This[3][1];M20[2][1] = This[3][2];M20[2][2] = This[3][3];

	//lmMatrix3x3 M30;
	lmMatrixNxN<T,3> M30;
	M30[0][0] = This[0][1];M30[0][1] = This[0][2];M30[0][2] = This[0][3];
	M30[1][0] = This[1][1];M30[1][1] = This[1][2];M30[1][2] = This[1][3];
	M30[2][0] = This[2][1];M30[2][1] = This[2][2];M30[2][2] = This[2][3];

	Det = (This[0][0] * M00.Determinant()) - (This[1][0] * M10.Determinant()) + (This[2][0] * M20.Determinant()) - (This[3][0] * M30.Determinant());

	return Det;
}
/*
template<>
lmScalar lmMatrixNxN<lmScalar,4>::Determinant() const;

template<>
double lmMatrixNxN<double,4>::Determinant() const;
*/
#include "lmQuaternion.hpp"

//Fixed function OpenGL stílusú függvények

template<class T>
void lmOrtho4x4(lmMatrixNxN<T,4> &Target,T Top,T Bottom,T Left,T Right,T Far,T Near)
{
    lmMatrixNxN<T,4> A;

    A[0][0] = 2/(Right-Left);A[1][0] = 0;             A[2][0] =             0;A[3][0] = -(Right+Left)/(Right-Left);
    A[0][1] = 0;             A[1][1] = 2/(Top-Bottom);A[2][1] =             0;A[3][1] = -(Top+Bottom)/(Top-Bottom);
    A[0][2] = 0;             A[1][2] = 0;             A[2][2] = -2/(Far-Near);A[3][2] =  (Far+Near)/(Far-Near);
    A[0][3] = 0;             A[1][3] = 0;             A[2][3] =             0;A[3][3] =  1;

    Target *= A;
}

template<class T>
void lmFrustum4x4(lmMatrixNxN<T,4> &Target,T Top,T Bottom,T Left,T Right,T Far,T Near)
{
    lmMatrixNxN<T,4> A;

    A[0][0]=(2*Near)/(Right-Left);	A[1][0]=0;						A[2][0]=(Right+Left)/(Right-Left);	A[3][0]=0;
	A[0][1]=0;						A[1][1]=(2*Near)/(Top-Bottom);	A[2][1]=(Top+Bottom)/(Top-Bottom);	A[3][1]=0;
	A[0][2]=0;						A[1][2]=0;						A[2][2]=-(Far+Near)/(Far-Near);     A[3][2]=(-2*Far*Near)/(Far-Near);
	A[0][3]=0;						A[1][3]=0;						A[2][3]=-1;						    A[3][3]=0;

    Target *= A;
}

template<class T>
void lmTranslate4x4(lmMatrixNxN<T,4> &Target,T X,T Y,T Z)
{
    lmMatrixNxN<T,4> A;

    A[0][0] = 1;A[1][0] = 0;A[2][0] = 0;A[3][0] = X;
    A[0][1] = 0;A[1][1] = 1;A[2][1] = 0;A[3][1] = Y;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;A[3][2] = Z;
    A[0][3] = 0;A[1][3] = 0;A[2][3] = 0;A[3][3] = 1;

    Target *= A;
}

template<class T>
void lmTranslate4x4(lmMatrixNxN<T,4> &Target,const lmVectorND<T,3> &V)
{
    lmMatrixNxN<T,4> A;

    A[0][0] = 1;A[1][0] = 0;A[2][0] = 0;A[3][0] = V[0];
    A[0][1] = 0;A[1][1] = 1;A[2][1] = 0;A[3][1] = V[1];
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;A[3][2] = V[2];
    A[0][3] = 0;A[1][3] = 0;A[2][3] = 0;A[3][3] =   1;

    Target *= A;
}

template<class T>
void lmRotate_X4x4(lmMatrixNxN<T,4> &Target,T Alpha)
{
    lmMatrixNxN<T,4> A;
    /*
     * TODO A compiler rinyál amiatt, hogy a sin és a cos double-t ad vissza,
     * a mátrix meg floatot fogad, érdemes lecsekkolni.
     */
    A[0][0] = 1;A[1][0] =          0;A[2][0] =           0;A[3][0] = 0;
    A[0][1] = 0;A[1][1] = cos(Alpha);A[2][1] = -sin(Alpha);A[3][1] = 0;
    A[0][2] = 0;A[1][2] = sin(Alpha);A[2][2] =  cos(Alpha);A[3][2] = 0;
    A[0][3] = 0;A[1][3] =          0;A[2][3] =           0;A[3][3] = 1;

    Target *= A;
}

template<class T>
void lmRotate_Y4x4(lmMatrixNxN<T,4> &Target,T Alpha)
{
    lmMatrixNxN<T,4> A;
    /*
     * TODO A compiler rinyál amiatt, hogy a sin és a cos double-t ad vissza,
     * a mátrix meg floatot fogad, érdemes lecsekkolni.
     */
    A[0][0] = cos(Alpha);A[1][0] = 0;A[2][0] = sin(Alpha);A[3][0] = 0;
    A[0][1] =          0;A[1][1] = 1;A[2][1] =          0;A[3][1] = 0;
    A[0][2] =-sin(Alpha);A[1][2] = 0;A[2][2] = cos(Alpha);A[3][2] = 0;
    A[0][3] =          0;A[1][3] = 0;A[2][3] =          0;A[3][3] = 1;

    Target *= A;
}

template<class T>
void lmRotate_Z4x4(lmMatrixNxN<T,4> &Target,T Alpha)
{
    lmMatrixNxN<T,4> A;
    /*
     * TODO A compiler rinyál amiatt, hogy a sin és a cos double-t ad vissza,
     * a mátrix meg floatot fogad, érdemes lecsekkolni.
     */
    A[0][0] = cos(Alpha);A[1][0] = -sin(Alpha);A[2][0] = 0;A[3][0] = 0;
    A[0][1] = sin(Alpha);A[1][1] =  cos(Alpha);A[2][1] = 0;A[3][1] = 0;
    A[0][2] =          0;A[1][2] =           0;A[2][2] = 1;A[3][2] = 0;
    A[0][3] =          0;A[1][3] =           0;A[2][3] = 0;A[3][3] = 1;

    Target *= A;
}

template<class T>
void lmScale4x4(lmMatrixNxN<T,4> &Target,T X,T Y,T Z)
{
    lmMatrixNxN<T,4> A;

    A[0][0] = X;A[1][0] = 0;A[2][0] = 0;A[3][0] = 0;
    A[0][1] = 0;A[1][1] = Y;A[2][1] = 0;A[3][1] = 0;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = Z;A[3][2] = 0;
    A[0][3] = 0;A[1][3] = 0;A[2][3] = 0;A[3][3] = 1;

    Target *= A;
}

template<class T>
void lmScale4x4(lmMatrixNxN<T,4> &Target,const lmVectorND<T,3> &V)
{
    lmScale4x4(Target,V[0],V[1],V[2]);
}

template<class T>
void lmRotate4x4(lmMatrixNxN<T,4> &Target,const lmQuaternionT<T> &q)
{
    lmQuaternionT<T> q_inv = q.GetInverse();

    lmQuaternionT<T>    i(1.0,0.0,0.0,0.0),
                    j(0.0,1.0,0.0,0.0),
                    k(0.0,0.0,1.0,0.0);

    lmQuaternionT<T>    t_i = q*i*q_inv;
    lmQuaternionT<T>    t_j = q*j*q_inv;
    lmQuaternionT<T>    t_k = q*k*q_inv;

    lmMatrixNxN<T,4> R;

    R[0][0] =   t_i.X;R[1][0] =   t_j.X;R[2][0] =   t_k.X;R[3][0] = 0.0;
    R[0][1] =   t_i.Y;R[1][1] =   t_j.Y;R[2][1] =   t_k.Y;R[3][1] = 0.0;
    R[0][2] =   t_i.Z;R[1][2] =   t_j.Z;R[2][2] =   t_k.Z;R[3][2] = 0.0;
    R[0][3] =     0.0;R[1][3] =     0.0;R[2][3] =     0.0;R[3][3] = 1.0;

    Target *= R;
}

template<class T>
lmMatrixNxN<T,4> lmOrtho4x4(T Top,T Bottom,T Left,T Right,T Far,T Near)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmOrtho4x4(A,Top,Bottom,Left,Right,Far,Near);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmFrustum4x4(T Top,T Bottom,T Left,T Right,T Far,T Near)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmFrustum4x4(A,Top,Bottom,Left,Right,Far,Near);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmTranslate4x4(T X,T Y,T Z)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmTranslate4x4(A,X,Y,Z);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmTranslate4x4(const lmVectorND<T,3> &V)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmTranslate4x4(A,V);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmRotate_X4x4(T Alpha)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmRotate_X4x4(A,Alpha);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmRotate_Y4x4(T Alpha)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmRotate_Y4x4(A,Alpha);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmRotate_Z4x4(T Alpha)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmRotate_Z4x4(A,Alpha);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmScale4x4(T X,T Y,T Z)
{
    lmMatrixNxN<T,4> A(lmMatrixNxN<T,4>::IDENTITY);

    lmScale4x4(A,X,Y,Z);

    return A;
}

template<class T>
lmMatrixNxN<T,4> lmRotate4x4(const lmQuaternionT<T> &q)
{
    lmMatrixNxN<T,4> R(lmMatrixNxN<T,4>::IDENTITY);

    lmRotate4x4(R,q);

    return R;
}

#endif
