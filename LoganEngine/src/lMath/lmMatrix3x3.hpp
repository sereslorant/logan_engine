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

template<class T>
T lmDeterminant(const lmMatrixNxN<T,3> &A)
{
    T detA =	A[0][0] *  (A[1][1] * A[2][2] - A[1][2] * A[2][1]) -
				A[1][0] *  (A[0][1] * A[2][2] - A[0][2] * A[2][1]) +
				A[2][0] *  (A[0][1] * A[1][2] - A[0][2] * A[1][1]);

    return detA;
}

template<class T>
void lmInverse(const lmMatrixNxN<T,3> &A,lmMatrixNxN<T,3> &Target)
{
	T detA = lmDeterminant(A);

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

#include "lmQuaternion.hpp"

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
template<class T>
void lmOrtho3x3(lmMatrixNxN<T,3> &Target,T Top,T Bottom,T Left,T Right)
{
    lmMatrixNxN<T,3> A;

    A[0][0] = 2/(Right-Left);A[1][0] = 0;             A[2][0] = -(Right+Left)/(Right-Left);
    A[0][1] = 0;             A[1][1] = 2/(Top-Bottom);A[2][1] = -(Top+Bottom)/(Top-Bottom);
    A[0][2] = 0;             A[1][2] = 0;             A[2][2] = 1;

    Target *= A;
}

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s eltolás mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param X lmScalar:           Az elmozdulás X komponense
 * \param Y lmScalar:           Az elmozdulás Y komponense
 * \return lmMatrix3x3 - az eltolás mátrix.
 *
 */
template<class T>
void lmTranslate3x3(lmMatrixNxN<T,3> &Target,T X,T Y)
{
    lmMatrixNxN<T,3> A;

    A[0][0] = 1;A[1][0] = 0;A[2][0] = X;
    A[0][1] = 0;A[1][1] = 1;A[2][1] = Y;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;

    Target *= A;
}

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s eltolás mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param X lmScalar:           Az elmozdulás X komponense
 * \param Y lmScalar:           Az elmozdulás Y komponense
 * \return lmMatrix3x3 - az eltolás mátrix.
 *
 */
template<class T>
void lmTranslate3x3(lmMatrixNxN<T,3> &Target,const lmVectorND<T,2> &V)
{
	lmTranslate3x3(Target,V[0],V[1]);
}

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s forgás mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param Alpha lmScalar:       Az elfordulás radiánban
 * \return lmMatrix3x3 - a forgás mátrix.
 *
 */
template<class T>
void lmRotate3x3(lmMatrixNxN<T,3> &Target,T Alpha)
{
    lmMatrixNxN<T,3> A;

    A[0][0] = cos(Alpha);A[1][0] = -sin(Alpha);A[2][0] = 0;
    A[0][1] = sin(Alpha);A[1][1] =  cos(Alpha);A[2][1] = 0;
    A[0][2] =          0;A[1][2] =           0;A[2][2] = 1;

    Target *= A;
}

/** \brief Beszorozza a Target-ben paraméterül kapott mátrixot egy 2D-s skálázó mátrixszal jobbról.
 *
 * \param Target lmMatrix3x3&:  A beszorzandó mátrix
 * \param X lmScalar:           Az X menti skálázás
 * \param Y lmScalar:           Az Y menti skálázás
 * \return lmMatrix3x3 - a skálázó mátrix.
 *
 */
template<class T>
void lmScale3x3(lmMatrixNxN<T,3> &Target,T X,T Y)
{
    lmMatrixNxN<T,3> A;

    A[0][0] = X;A[1][0] = 0;A[2][0] = 0;
    A[0][1] = 0;A[1][1] = Y;A[2][1] = 0;
    A[0][2] = 0;A[1][2] = 0;A[2][2] = 1;

    Target *= A;
}

template<class T>
void lmRotate3x3(lmMatrixNxN<T,3> &Target,const lmQuaternionT<T> &q)
{
    lmQuaternionT<T> q_inv = q.GetInverse();

    lmQuaternionT<T>    i(1.0,0.0,0.0,0.0),
                    	j(0.0,1.0,0.0,0.0),
						k(0.0,0.0,1.0,0.0);

    lmQuaternionT<T>    t_i = q*i*q_inv;
    lmQuaternionT<T>    t_j = q*j*q_inv;
    lmQuaternionT<T>    t_k = q*k*q_inv;

    lmMatrixNxN<T,3> R(lmMatrixNxN<T,3>::IDENTITY);

    R[0][0] =   t_i.X;R[1][0] =   t_j.X;R[2][0] =   t_k.X;
    R[0][1] =   t_i.Y;R[1][1] =   t_j.Y;R[2][1] =   t_k.Y;
    R[0][2] =   t_i.Z;R[1][2] =   t_j.Z;R[2][2] =   t_k.Z;

    Target *= R;
}

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
template<class T>
lmMatrixNxN<T,3> lmOrtho3x3(T Top,T Bottom,T Left,T Right)
{
    lmMatrixNxN<T,3> A(lmMatrixNxN<T,3>::IDENTITY);

    lmOrtho3x3(A,Top,Bottom,Left,Right);

    return A;
}

/** \brief Visszatér egy 2D-s eltolás mátrixszal.
 *
 * \param X lmScalar:   Az elmozdulás X komponense
 * \param Y lmScalar:   Az elmozdulás Y komponense
 * \return lmMatrix3x3 - az eltolás mátrix.
 *
 */
template<class T>
lmMatrixNxN<T,3> lmTranslate3x3(T X,T Y)
{
    lmMatrixNxN<T,3> A(lmMatrixNxN<T,3>::IDENTITY);

    lmTranslate3x3(A,X,Y);

    return A;
}

template<class T>
lmMatrixNxN<T,3> lmTranslate3x3(const lmVectorND<T,2> &V)
{
    lmMatrixNxN<T,3> A(lmMatrixNxN<T,3>::IDENTITY);

    lmTranslate3x3(A,V);

    return A;
}

/** \brief Visszatér egy 2D-s forgás mátrixszal.
 *
 * \param Alpha lmScalar:   Az elfordulás radiánban
 * \return lmMatrix3x3 - a forgás mátrix.
 *
 */
template<class T>
lmMatrixNxN<T,3> lmRotate3x3(T Alpha)
{
    lmMatrixNxN<T,3> A(lmMatrixNxN<T,3>::IDENTITY);

    lmRotate3x3(A,Alpha);

    return A;
}

/** \brief Visszatér egy 2D-s skálázó mátrixszal.
 *
 * \param X lmScalar:   Az X menti skálázás
 * \param Y lmScalar:   Az Y menti skálázás
 * \return lmMatrix3x3 - a skálázó mátrix.
 *
 */
template<class T>
lmMatrixNxN<T,3> lmScale3x3(T X,T Y)
{
    lmMatrixNxN<T,3> A(lmMatrixNxN<T,3>::IDENTITY);

    lmScale3x3(A,X,Y);

    return A;
}

template<class T>
lmMatrixNxN<T,3> lmScale3x3(const lmVectorND<T,2> &V)
{
    lmMatrixNxN<T,3> A(lmMatrixNxN<T,3>::IDENTITY);

    lmScale3x3(A,V[0],V[1]);

    return A;
}

/** \brief Visszatér egy 3D-s forgás mátrixszal.
 *
 * \param q const lmQuaternion &:   Az elfordulás tengelyét és szögét megadó kavternió
 * \return lmMatrix3x3 - a forgás mátrix.
 *
 */
template<class T>
lmMatrixNxN<T,3> lmRotate3x3(const lmQuaternionT<T> &q)
{
    lmMatrixNxN<T,3> R(lmMatrixNxN<T,3>::IDENTITY);

    lmRotate3x3(R,q);

    return R;
}

#endif // LM_MATRIX_3X3_H
