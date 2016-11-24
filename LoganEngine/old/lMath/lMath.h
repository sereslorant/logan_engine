#ifndef L_MATH_H
#define L_MATH_H

#include <cmath>

#define PI 3.14159265359


#include "lmVectorND.hpp"
#include "lmVector3D.hpp"

#include "lmMatrixNxN.hpp"
#include "lmMatrix3x3.h"
#include "lmMatrix4x4.h"

#include "lmQuaternion.h"

#include "lmGeometry.h"

#include "lmMindenszar.hpp"


/** \brief A paraméterként kapott szöget átkonvertálja fokból radiánba.
 *
 * \param Deg lmScalar: A kapott szög fokokban.
 * \return lmScalar - a radiánba konvertált szög.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmDegToRad(lmScalar Deg);

/** \brief A paraméterként kapott szöget átkonvertálja radiánból fokba.
 *
 * \param Rad lmScalar: A kapott szög radiánban.
 * \return lmScalar - a fokokba konvertált szög.
 *
 * \author Seres Lóránt Gábor
 *
 */
lmScalar lmRadToDeg(lmScalar Rad);

int lmSgn(lmScalar value,lmScalar epsilon = 1e-4);

#endif // L_MATH_H
