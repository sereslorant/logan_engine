#ifndef LM_VECTOR_3D
#define LM_VECTOR_3D

#include "lmVectorND.hpp"

/** \brief Két vektor vektoriális szorzata
 *
 * \param U const lmVector3D &: az egyik vektor
 * \param V const lmVector3D &: a másik vektor
 * \return lmVector3D - a művelet eredménye
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T>
lmVectorND<T,3> lmCross(const lmVectorND<T,3> &U,const lmVectorND<T,3> &V)
{
    lmVectorND<T,3> Cp;

    /*
    Cp.X = (U.Y * V.Z) - (U.Z * V.Y);
    Cp.Y = (U.Z * V.X) - (U.X * V.Z);
    Cp.Z = (U.X * V.Y) - (U.Y * V.X);
    */

    Cp[0] = (U[1] * V[2]) - (U[2] * V[1]);
    Cp[1] = (U[2] * V[0]) - (U[0] * V[2]);
    Cp[2] = (U[0] * V[1]) - (U[1] * V[0]);

    //std::cout << U[2] << ";" << V[0] << ";\n";

    //std::cout << Cp[0] << ";" << Cp[1] << ";" << Cp[2] << std::endl;

    return Cp;
}

#endif
