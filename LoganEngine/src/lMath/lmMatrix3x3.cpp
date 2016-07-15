
#include "lmMatrix3x3.h"

const lmMatrix3x3 NULL_MATRIX_3X3;
const lmMatrix3x3 IDENTITY_MATRIX_3X3 = lmMatrix3x3(lmMatrix3x3::IDENTITY);

template<>
lmScalar lmMatrixNxN<lmScalar,3>::Determinant() const
{
    lmScalar detA =	lmMatrix3x3DeterminantT(*this);

    return detA;
}

template<>
void lmMatrixNxN<lmScalar,3>::Invert(lmMatrixNxN<lmScalar,3> &Target) const
{
	Target = lmMatrixNxN<lmScalar, 3>(lmMatrixNxN<lmScalar, 3>::IDENTITY);

    lmMatrix3x3InverseT(*this,Target);
}

template<>
double lmMatrixNxN<double,3>::Determinant() const
{
    double detA = lmMatrix3x3DeterminantT(*this);

    return detA;
}

template<>
void lmMatrixNxN<double,3>::Invert(lmMatrixNxN<double,3> &Target) const
{
	Target = lmMatrixNxN<double, 3>(lmMatrixNxN<double, 3>::IDENTITY);

    lmMatrix3x3InverseT(*this,Target);
}

void lmOrtho3x3(lmMatrix3x3 &Target,lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right)
{
    lmMatrix3x3 A;

    A[0][0] = 2/(Right-Left);A[1][0] = 0;             A[2][0] = -(Right+Left)/(Right-Left);
    A[0][1] = 0;             A[1][1] = 2/(Top-Bottom);A[2][1] = -(Top+Bottom)/(Top-Bottom);
    A[0][2] = 0;             A[1][2] = 0;             A[2][2] = 1;

    Target *= A;
}

void lmTranslate3x3(lmMatrix3x3 &Target,lmScalar X,lmScalar Y)
{
    lmMatrix3x3 A;

    A[0][0] = 1;A[1][0] = 0;A[2][0] = X;
    A[0][1] = 0;A[1][1] = 1;A[2][1] = Y;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;

    Target *= A;
}

void lmRotate3x3(lmMatrix3x3 &Target,lmScalar Alpha)
{
    lmMatrix3x3 A;

    A[0][0] = cos(Alpha);A[1][0] = -sin(Alpha);A[2][0] = 0;
    A[0][1] = sin(Alpha);A[1][1] =  cos(Alpha);A[2][1] = 0;
    A[0][2] =          0;A[1][2] =           0;A[2][2] = 1;

    Target *= A;
}

void lmScale3x3(lmMatrix3x3 &Target,lmScalar X,lmScalar Y)
{
    lmMatrix3x3 A;

    A[0][0] = X;A[1][0] = 0;A[2][0] = 0;
    A[0][1] = 0;A[1][1] = Y;A[2][1] = 0;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;

    Target *= A;
}

void lmRotate3x3(lmMatrix3x3 &Target,const lmQuaternion &q)
{
    lmQuaternion q_inv = q.GetInverse();

    lmQuaternion    i(1.0,0.0,0.0,0.0),
                    j(0.0,1.0,0.0,0.0),
                    k(0.0,0.0,1.0,0.0);

    lmQuaternion    t_i = q*i*q_inv;
    lmQuaternion    t_j = q*j*q_inv;
    lmQuaternion    t_k = q*k*q_inv;

    lmMatrix3x3 R(lmMatrix3x3::IDENTITY);

    R[0][0] =   t_i.X;R[1][0] =   t_j.X;R[2][0] =   t_k.X;
    R[0][1] =   t_i.Y;R[1][1] =   t_j.Y;R[2][1] =   t_k.Y;
    R[0][2] =   t_i.Z;R[1][2] =   t_j.Z;R[2][2] =   t_k.Z;

    Target *= R;
}

lmMatrix3x3 lmOrtho3x3(lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right)
{
    lmMatrix3x3 A(lmMatrix3x3::IDENTITY);

    lmOrtho3x3(A,Top,Bottom,Left,Right);

    return A;
}

lmMatrix3x3 lmTranslate3x3(lmScalar X,lmScalar Y)
{
    lmMatrix3x3 A(lmMatrix3x3::IDENTITY);

    lmTranslate3x3(A,X,Y);

    return A;
}

lmMatrix3x3 lmTranslate3x3(const lmVector2D &V)
{
    lmMatrix3x3 A(lmMatrix3x3::IDENTITY);

    lmTranslate3x3(A,V[0],V[1]);

    return A;
}

lmMatrix3x3 lmRotate3x3(lmScalar Alpha)
{
    lmMatrix3x3 A(lmMatrix3x3::IDENTITY);

    lmRotate3x3(A,Alpha);

    return A;
}

lmMatrix3x3 lmScale3x3(const lmVector2D &V)
{
    lmMatrix3x3 A(lmMatrix3x3::IDENTITY);

    lmScale3x3(A,V[0],V[1]);

    return A;
}

lmMatrix3x3 lmScale3x3(lmScalar X,lmScalar Y)
{
    lmMatrix3x3 A(lmMatrix3x3::IDENTITY);

    lmScale3x3(A,X,Y);

    return A;
}

lmMatrix3x3 lmRotate3x3(const lmQuaternion &q)
{
    lmMatrix3x3 R(lmMatrix3x3::IDENTITY);

    lmRotate3x3(R,q);

    return R;
}
