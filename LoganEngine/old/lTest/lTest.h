#ifndef L_TEST_H
#define L_TEST_H

#include "lTest_Math.h"

void Test()
{
	lTest_Math TestMath;
	TestMath.TestMatrixVectorMlt();
	TestMath.TestMatrixMlt();
}

#endif // L_TEST_H
