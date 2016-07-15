#ifndef LM_MATRIX_4X4_H
#define LM_MATRIX_4X4_H

#include "lmMatrixNxN.hpp"
#include "lmMatrix3x3.h"

typedef lmVectorND<lmScalar,4> lmVector4D;

/** \brief Egy 4x4-es mátrixot definiáló osztály
 *
 * \todo A 3d-s transzformációk nincsenek dokumentálva.
 *
 * \author Seres Lóránt Gábor
 *
 */
typedef lmMatrixNxN<lmScalar,4> lmMatrix4x4;

extern const lmMatrix4x4 NULL_MATRIX_4X4;
extern const lmMatrix4x4 IDENTITY_MATRIX_4X4;

template<class T>
T lmMatrix4x4DeterminantT(const lmMatrixNxN<T,4> &This)
{
	T Det;

	lmMatrix3x3 M00;
	M00[0][0] = This[1][1];M00[0][1] = This[1][2];M00[0][2] = This[1][3];
	M00[1][0] = This[2][1];M00[1][1] = This[2][2];M00[1][2] = This[2][3];
	M00[2][0] = This[3][1];M00[2][1] = This[3][2];M00[2][2] = This[3][3];

	lmMatrix3x3 M10;
	M10[0][0] = This[0][1];M10[0][1] = This[0][2];M10[0][2] = This[0][3];
	M10[1][0] = This[2][1];M10[1][1] = This[2][2];M10[1][2] = This[2][3];
	M10[2][0] = This[3][1];M10[2][1] = This[3][2];M10[2][2] = This[3][3];

	lmMatrix3x3 M20;
	M20[0][0] = This[0][1];M20[0][1] = This[0][2];M20[0][2] = This[0][3];
	M20[1][0] = This[1][1];M20[1][1] = This[1][2];M20[1][2] = This[1][3];
	M20[2][0] = This[3][1];M20[2][1] = This[3][2];M20[2][2] = This[3][3];

	lmMatrix3x3 M30;
	M30[0][0] = This[0][1];M30[0][1] = This[0][2];M30[0][2] = This[0][3];
	M30[1][0] = This[1][1];M30[1][1] = This[1][2];M30[1][2] = This[1][3];
	M30[2][0] = This[2][1];M30[2][1] = This[2][2];M30[2][2] = This[2][3];

	Det = (This[0][0] * M00.Determinant()) - (This[1][0] * M10.Determinant()) + (This[2][0] * M20.Determinant()) - (This[3][0] * M30.Determinant());

	return Det;
}

template<>
lmScalar lmMatrixNxN<lmScalar,4>::Determinant() const;

template<>
double lmMatrixNxN<double,4>::Determinant() const;

#include "lmQuaternion.h"

//Fixed function OpenGL stílusú függvények

void lmOrtho4x4(lmMatrix4x4 &Target,lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near);
void lmFrustum4x4(lmMatrix4x4 &Target,lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near);

void lmTranslate4x4(lmMatrix4x4 &Target,lmScalar X,lmScalar Y,lmScalar Z);
void lmTranslate4x4(lmMatrix4x4 &Target,const lmVector3D &V);
void lmRotate_X4x4(lmMatrix4x4 &Target,lmScalar Alpha);
void lmRotate_Y4x4(lmMatrix4x4 &Target,lmScalar Alpha);
void lmRotate_Z4x4(lmMatrix4x4 &Target,lmScalar Alpha);
void lmScale4x4(lmMatrix4x4 &Target,lmScalar X,lmScalar Y,lmScalar Z);
void lmScale4x4(lmMatrix4x4 &Target,const lmVector3D &V);

void lmRotate4x4(lmMatrix4x4 &Target,const lmQuaternion &q);

lmMatrix4x4 lmOrtho4x4(lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near);
lmMatrix4x4 lmFrustum4x4(lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right,lmScalar Far,lmScalar Near);

lmMatrix4x4 lmTranslate4x4(lmScalar X,lmScalar Y,lmScalar Z);
lmMatrix4x4 lmTranslate4x4(const lmVector3D &V);
lmMatrix4x4 lmRotate_X4x4(lmScalar Alpha);
lmMatrix4x4 lmRotate_Y4x4(lmScalar Alpha);
lmMatrix4x4 lmRotate_Z4x4(lmScalar Alpha);
lmMatrix4x4 lmScale4x4(lmScalar X,lmScalar Y,lmScalar Z);

lmMatrix4x4 lmRotate4x4(const lmQuaternion &q);

#endif
