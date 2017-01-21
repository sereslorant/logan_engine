#ifndef LM_MATRIX_3X3_H
#define LM_MATRIX_3X3_H

#include "lmMatrixNxN.hpp"

/** \brief Egy 3x3-as mátrixot definiáló osztály
 *
 * \todo Néhány transzformáció nincs dokumentálva.
 *
 * \author Seres Lóránt Gábor
 *
 */
typedef lmMatrixNxN<lmScalar,3> lmMatrix3x3;

extern const lmMatrix3x3 NULL_MATRIX_3X3;
extern const lmMatrix3x3 IDENTITY_MATRIX_3X3;

template<class T>
T lmMatrix3x3DeterminantT(const lmMatrixNxN<T,3> &A)
{
    T detA =	A[0][0] *  (A[1][1] * A[2][2] - A[1][2] * A[2][1]) -
				A[1][0] *  (A[0][1] * A[2][2] - A[0][2] * A[2][1]) +
				A[2][0] *  (A[0][1] * A[1][2] - A[0][2] * A[1][1]);

    return detA;
}

template<class T>
void lmMatrix3x3InverseT(const lmMatrixNxN<T,3> &A,lmMatrixNxN<T,3> &Target)
{
	T detA = lmMatrix3x3DeterminantT(A);

    if(std::abs(detA) > 1e-16)
    {
        Target[0][0] =  (A[1][1] * A[2][2] - A[2][1] * A[1][2]);
        Target[1][0] = -(A[1][0] * A[2][2] - A[2][0] * A[1][2]);
        Target[2][0] =  (A[1][0] * A[2][1] - A[2][0] * A[1][1]);

        Target[0][1] = -(A[0][1] * A[2][2] - A[2][1] * A[1][2]);
        Target[1][1] =  (A[0][0] * A[2][2] - A[2][0] * A[0][2]);
        Target[2][1] = -(A[0][0] * A[2][1] - A[2][0] * A[0][1]);

        Target[0][2] =  (A[0][1] * A[1][2] - A[1][1] * A[0][2]);
        Target[1][2] = -(A[0][0] * A[1][2] - A[1][0] * A[0][2]);
        Target[2][2] =  (A[0][0] * A[1][1] - A[1][0] * A[0][1]);

        for(int i=0;i < 3;i++)
        {
            for(int j=0;j < 3;j++)
            {
                Target[i][j] /= detA;
            }
        }
    }
}

template<>
lmScalar lmMatrixNxN<lmScalar,3>::Determinant() const;

template<>
void lmMatrixNxN<lmScalar,3>::Invert(lmMatrixNxN<lmScalar,3> &Target) const;

template<>
double lmMatrixNxN<double,3>::Determinant() const;

template<>
void lmMatrixNxN<double,3>::Invert(lmMatrixNxN<double,3> &Target) const;

#include "lmQuaternion.h"

//Fixed function OpenGL stílusú függvények

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s merőleges vetítést végző mátrixszal jobbról.
 *
 *  \todo a paramétereket dokumentálni kéne. Ha meglesz a számítógépes grafika tárgy, vissza kell térni rá.
 *
 * \param Target lmMatrix3x3 &:  A beszorzandó mátrixra mutató referencia.
 * \param Top lmScalar:
 * \param Bottom lmScalar:
 * \param Left lmScalar:
 * \param Right lmScalar:
 *
 * \return lmMatrix3x3 - a vetítőmátrix.
 *
 */
void lmOrtho3x3(lmMatrix3x3 &Target,lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right);

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s eltolás mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param X lmScalar:           Az elmozdulás X komponense
 * \param Y lmScalar:           Az elmozdulás Y komponense
 * \return lmMatrix3x3 - az eltolás mátrix.
 *
 */
void lmTranslate3x3(lmMatrix3x3 &Target,lmScalar X,lmScalar Y);

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s eltolás mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param X lmScalar:           Az elmozdulás X komponense
 * \param Y lmScalar:           Az elmozdulás Y komponense
 * \return lmMatrix3x3 - az eltolás mátrix.
 *
 */
void lmTranslate3x3(lmMatrix3x3 &Target,const lmVector2D &V);

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s forgás mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param Alpha lmScalar:       Az elfordulás radiánban
 * \return lmMatrix3x3 - a forgás mátrix.
 *
 */
void lmRotate3x3(lmMatrix3x3 &Target,lmScalar Alpha);

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s skálázó mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param X lmScalar:           Az X menti skálázás
 * \param Y lmScalar:           Az Y menti skálázás
 * \return lmMatrix3x3 - a skálázó mátrix.
 *
 */
void lmScale3x3(lmMatrix3x3 &Target,lmScalar X,lmScalar Y);

void lmRotate3x3(lmMatrix3x3 &Target,const lmQuaternion &q);


/** \brief Visszatér egy 2D-s merőleges vetítést végző mátrixszal.
 *
 *  \todo a paramétereket dokumentálni kéne. Ha meglesz a számítógépes grafika tárgy, vissza kell térni rá.
 *
 * \param Top lmScalar:
 * \param Bottom lmScalar:
 * \param Left lmScalar:
 * \param Right lmScalar:
 *
 * \return lmMatrix3x3 - a vetítőmátrix.
 *
 */
lmMatrix3x3 lmOrtho3x3(lmScalar Top,lmScalar Bottom,lmScalar Left,lmScalar Right);

/** \brief Visszatér egy 2D-s eltolás mátrixszal.
 *
 * \param X lmScalar:   Az elmozdulás X komponense
 * \param Y lmScalar:   Az elmozdulás Y komponense
 * \return lmMatrix3x3 - az eltolás mátrix.
 *
 */
lmMatrix3x3 lmTranslate3x3(lmScalar X,lmScalar Y);

lmMatrix3x3 lmTranslate3x3(const lmVector2D &V);

/** \brief Visszatér egy 2D-s forgás mátrixszal.
 *
 * \param Alpha lmScalar:   Az elfordulás radiánban
 * \return lmMatrix3x3 - a forgás mátrix.
 *
 */
lmMatrix3x3 lmRotate3x3(lmScalar Alpha);

/** \brief Visszatér egy 2D-s skálázó mátrixszal.
 *
 * \param X lmScalar:   Az X menti skálázás
 * \param Y lmScalar:   Az Y menti skálázás
 * \return lmMatrix3x3 - a skálázó mátrix.
 *
 */
lmMatrix3x3 lmScale3x3(lmScalar X,lmScalar Y);

lmMatrix3x3 lmScale3x3(const lmVector2D &V);

/** \brief Visszatér egy 3D-s forgás mátrixszal.
 *
 * \param q const lmQuaternion &:   Az elfordulás tengelyét és szögét megadó kavternió
 * \return lmMatrix3x3 - a forgás mátrix.
 *
 */
lmMatrix3x3 lmRotate3x3(const lmQuaternion &q);

#endif // LM_MATRIX_3X3_H
