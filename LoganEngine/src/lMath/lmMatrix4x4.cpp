
#include "lmMatrix4x4.h"

const lmMatrix4x4 NULL_MATRIX_4X4;
const lmMatrix4x4 IDENTITY_MATRIX_4X4 = lmMatrix4x4(lmMatrix4x4::IDENTITY);

template<>
lmScalar lmMatrixNxN<lmScalar,4>::Determinant() const
{
	return lmMatrix4x4DeterminantT(*this);
}

template<>
double lmMatrixNxN<double,4>::Determinant() const
{
	return lmMatrix4x4DeterminantT(*this);
}

/*
Ez úgy szar ahogy van.
template<>
void lmMatrixNxN<4>::Invert(lmMatrix4x4 &Target) const
{
	lmScalar detA = this->Determinant();

	static lmScalar ChessTable[4][4];
	ChessTable[0][0] =  1.0;ChessTable[1][0] = -1.0;ChessTable[2][0] =  1.0;ChessTable[3][0] = -1.0;
	ChessTable[0][1] = -1.0;ChessTable[1][1] =  1.0;ChessTable[2][1] = -1.0;ChessTable[3][1] =  1.0;
	ChessTable[0][2] =  1.0;ChessTable[1][2] = -1.0;ChessTable[2][2] =  1.0;ChessTable[3][2] = -1.0;
	ChessTable[0][3] = -1.0;ChessTable[1][3] =  1.0;ChessTable[2][3] = -1.0;ChessTable[3][3] =  1.0;


	if(std::abs(detA) > 1e-16)
	{
		for(int i=0;i < 4;i++)
		{
			for(int j=0;j < 4;j++)
			{
				Target[i][j] = this->GetSubMatrix(i,j).Determinant()*ChessTable[i][j];
				Target[i][j] /= detA;
			}
		}
	}
    else
    {
        Target = IDENTITY_MATRIX_4X4;
    }
}
*/
void lmOrtho4x4(lmMatrix4x4 &Target,lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near)
{
    lmMatrix4x4 A;

    A[0][0] = 2/(Right-Left);A[1][0] = 0;             A[2][0] =             0;A[3][0] = -(Right+Left)/(Right-Left);
    A[0][1] = 0;             A[1][1] = 2/(Top-Bottom);A[2][1] =             0;A[3][1] = -(Top+Bottom)/(Top-Bottom);
    A[0][2] = 0;             A[1][2] = 0;             A[2][2] = -2/(Far-Near);A[3][2] =  (Far+Near)/(Far-Near);
    A[0][3] = 0;             A[1][3] = 0;             A[2][3] =             0;A[3][3] =  1;

    Target *= A;
}

void lmFrustum4x4(lmMatrix4x4 &Target,lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near)
{
    lmMatrix4x4 A;

    A[0][0]=(2*Near)/(Right-Left);	A[1][0]=0;						A[2][0]=(Right+Left)/(Right-Left);	A[3][0]=0;
	A[0][1]=0;						A[1][1]=(2*Near)/(Top-Bottom);	A[2][1]=(Top+Bottom)/(Top-Bottom);	A[3][1]=0;
	A[0][2]=0;						A[1][2]=0;						A[2][2]=-(Far+Near)/(Far-Near);     A[3][2]=(-2*Far*Near)/(Far-Near);
	A[0][3]=0;						A[1][3]=0;						A[2][3]=-1;						    A[3][3]=0;

    Target *= A;
}

void lmTranslate4x4(lmMatrix4x4 &Target,lmScalar X,lmScalar Y,lmScalar Z)
{
    lmMatrix4x4 A;

    A[0][0] = 1;A[1][0] = 0;A[2][0] = 0;A[3][0] = X;
    A[0][1] = 0;A[1][1] = 1;A[2][1] = 0;A[3][1] = Y;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;A[3][2] = Z;
    A[0][3] = 0;A[1][3] = 0;A[2][3] = 0;A[3][3] = 1;

    Target *= A;
}

void lmTranslate4x4(lmMatrix4x4 &Target,const lmVector3D &V)
{
    lmMatrix4x4 A;

    A[0][0] = 1;A[1][0] = 0;A[2][0] = 0;A[3][0] = V[0];
    A[0][1] = 0;A[1][1] = 1;A[2][1] = 0;A[3][1] = V[1];
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;A[3][2] = V[2];
    A[0][3] = 0;A[1][3] = 0;A[2][3] = 0;A[3][3] =   1;

    Target *= A;
}

void lmRotate_X4x4(lmMatrix4x4 &Target,lmScalar Alpha)
{
    lmMatrix4x4 A;

    A[0][0] = 1;A[1][0] =          0;A[2][0] =           0;A[3][0] = 0;
    A[0][1] = 0;A[1][1] = cos(Alpha);A[2][1] = -sin(Alpha);A[3][1] = 0;
    A[0][2] = 0;A[1][2] = sin(Alpha);A[2][2] =  cos(Alpha);A[3][2] = 0;
    A[0][3] = 0;A[1][3] =          0;A[2][3] =           0;A[3][3] = 1;

    Target *= A;
}

void lmRotate_Y4x4(lmMatrix4x4 &Target,lmScalar Alpha)
{
    lmMatrix4x4 A;

    A[0][0] = cos(Alpha);A[1][0] = 0;A[2][0] = sin(Alpha);A[3][0] = 0;
    A[0][1] =          0;A[1][1] = 1;A[2][1] =          0;A[3][1] = 0;
    A[0][2] =-sin(Alpha);A[1][2] = 0;A[2][2] = cos(Alpha);A[3][2] = 0;
    A[0][3] =          0;A[1][3] = 0;A[2][3] =          0;A[3][3] = 1;

    Target *= A;
}

void lmRotate_Z4x4(lmMatrix4x4 &Target,lmScalar Alpha)
{
    lmMatrix4x4 A;

    A[0][0] = cos(Alpha);A[1][0] = -sin(Alpha);A[2][0] = 0;A[3][0] = 0;
    A[0][1] = sin(Alpha);A[1][1] =  cos(Alpha);A[2][1] = 0;A[3][1] = 0;
    A[0][2] =          0;A[1][2] =           0;A[2][2] = 1;A[3][2] = 0;
    A[0][3] =          0;A[1][3] =           0;A[2][3] = 0;A[3][3] = 1;

    Target *= A;
}

void lmScale4x4(lmMatrix4x4 &Target,lmScalar X,lmScalar Y,lmScalar Z)
{
    lmMatrix4x4 A;

    A[0][0] = X;A[1][0] = 0;A[2][0] = 0;A[3][0] = 0;
    A[0][1] = 0;A[1][1] = Y;A[2][1] = 0;A[3][1] = 0;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = Z;A[3][2] = 0;
    A[0][3] = 0;A[1][3] = 0;A[2][3] = 0;A[3][3] = 1;

    Target *= A;
}

void lmScale4x4(lmMatrix4x4 &Target,const lmVector3D &V)
{
    lmScale4x4(Target,V[0],V[1],V[2]);
}

void lmRotate4x4(lmMatrix4x4 &Target,const lmQuaternion &q)
{
    lmQuaternion q_inv = q.GetInverse();

    lmQuaternion    i(1.0,0.0,0.0,0.0),
                    j(0.0,1.0,0.0,0.0),
                    k(0.0,0.0,1.0,0.0);

    lmQuaternion    t_i = q*i*q_inv;
    lmQuaternion    t_j = q*j*q_inv;
    lmQuaternion    t_k = q*k*q_inv;

    lmMatrix4x4 R;

    R[0][0] =   t_i.X;R[1][0] =   t_j.X;R[2][0] =   t_k.X;R[3][0] = 0.0;
    R[0][1] =   t_i.Y;R[1][1] =   t_j.Y;R[2][1] =   t_k.Y;R[3][1] = 0.0;
    R[0][2] =   t_i.Z;R[1][2] =   t_j.Z;R[2][2] =   t_k.Z;R[3][2] = 0.0;
    R[0][3] =     0.0;R[1][3] =     0.0;R[2][3] =     0.0;R[3][3] = 1.0;

    Target *= R;
}

lmMatrix4x4 lmOrtho4x4(lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmOrtho4x4(A,Top,Bottom,Left,Right,Far,Near);

    return A;
}

lmMatrix4x4 lmFrustum4x4(lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmFrustum4x4(A,Top,Bottom,Left,Right,Far,Near);

    return A;
}

lmMatrix4x4 lmTranslate4x4(lmScalar X,lmScalar Y,lmScalar Z)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmTranslate4x4(A,X,Y,Z);

    return A;
}

lmMatrix4x4 lmTranslate4x4(const lmVector3D &V)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmTranslate4x4(A,V);

    return A;
}

lmMatrix4x4 lmRotate_X4x4(lmScalar Alpha)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmRotate_X4x4(A,Alpha);

    return A;
}

lmMatrix4x4 lmRotate_Y4x4(lmScalar Alpha)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmRotate_Y4x4(A,Alpha);

    return A;
}

lmMatrix4x4 lmRotate_Z4x4(lmScalar Alpha)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmRotate_Z4x4(A,Alpha);

    return A;
}

lmMatrix4x4 lmScale4x4(lmScalar X,lmScalar Y,lmScalar Z)
{
    lmMatrix4x4 A(lmMatrix4x4::IDENTITY);

    lmScale4x4(A,X,Y,Z);

    return A;
}

lmMatrix4x4 lmRotate4x4(const lmQuaternion &q)
{
    lmMatrix4x4 R(lmMatrix4x4::IDENTITY);

    lmRotate4x4(R,q);

    return R;
}
