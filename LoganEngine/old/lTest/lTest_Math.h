#ifndef L_TEST_MATH_H
#define L_TEST_MATH_H

#include <iostream>

#include "../lMath/lMath.h"

class lTest_Math
{
public:

	bool TestMatrixVectorMlt()
	{
		lmVector3D	Vector;
		lmMatrix3x3	Matrix;

		Vector[0] = 2;
		Vector[1] = 4;
		Vector[2] = 8;

		Matrix[0][0] = 1.0;Matrix[1][0] = 2.0;Matrix[2][0] = 3.0;
		Matrix[0][1] = 4.0;Matrix[1][1] = 5.0;Matrix[2][1] = 2.0;
		Matrix[0][2] = 6.0;Matrix[1][2] = 4.0;Matrix[2][2] = 5.0;

		const lmVector3D Res1({34.0,44.0,68.0});
		//const lmVector3D Res2({66.0,40.0,68.0});
		const lmVector3D Res2({66.0,56.0,54.0});

		lmVector3D Res;

		Res = Matrix*Vector;
		std::cout << "(" << Res[0] << ";" << Res[1] << ";" << Res[2] << ")" << std::endl;
		std::cout << std::endl;

		std::cout << "(" << Res1[0] << ";" << Res1[1] << ";" << Res1[2] << ")" << std::endl;
		std::cout << std::endl;

		Res = Vector*Matrix;
		std::cout << "(" << Res[0] << ";" << Res[1] << ";" << Res[2] << ")" << std::endl;
		std::cout << std::endl;

		std::cout << "(" << Res2[0] << ";" << Res2[1] << ";" << Res2[2] << ")" << std::endl;
		std::cout << std::endl;
	}

	bool TestMatrixMlt()
	{
		lmMatrix3x3	Matrix1;
		lmMatrix3x3	Matrix2;

		Matrix1[0][0] = 1.0;Matrix1[1][0] = 2.0;Matrix1[2][0] = 3.0;
		Matrix1[0][1] = 4.0;Matrix1[1][1] = 5.0;Matrix1[2][1] = 2.0;
		Matrix1[0][2] = 6.0;Matrix1[1][2] = 4.0;Matrix1[2][2] = 5.0;

		Matrix2[0][0] = 2.0;Matrix2[1][0] = 4.0;Matrix2[2][0] = 3.0;
		Matrix2[0][1] = 1.0;Matrix2[1][1] = 5.0;Matrix2[2][1] = 6.0;
		Matrix2[0][2] = 8.0;Matrix2[1][2] = 2.0;Matrix2[2][2] = 1.0;

		lmMatrix3x3	Res1;
		lmMatrix3x3	Res2;

		Res1[0][0] = 36.0;Res1[1][0] = 36.0;Res1[2][0] = 29.0;
		Res1[0][1] = 57.0;Res1[1][1] = 51.0;Res1[2][1] = 43.0;
		Res1[0][2] = 22.0;Res1[1][2] = 30.0;Res1[2][2] = 33.0;

		Res2[0][0] = 28.0;Res2[1][0] = 20.0;Res2[2][0] = 18.0;
		Res2[0][1] = 29.0;Res2[1][1] = 45.0;Res2[2][1] = 44.0;
		Res2[0][2] = 56.0;Res2[1][2] = 54.0;Res2[2][2] = 47.0;

		lmMatrix3x3 Res;

		Res = Matrix2*Matrix1;
		std::cout << Res[0][0] << ";" << Res[1][0] << ";" << Res[2][0] << std::endl;
		std::cout << Res[0][1] << ";" << Res[1][1] << ";" << Res[2][1] << std::endl;
		std::cout << Res[0][2] << ";" << Res[1][2] << ";" << Res[2][2] << std::endl;
		std::cout << std::endl;

		std::cout << Res1[0][0] << ";" << Res1[1][0] << ";" << Res1[2][0] << std::endl;
		std::cout << Res1[0][1] << ";" << Res1[1][1] << ";" << Res1[2][1] << std::endl;
		std::cout << Res1[0][2] << ";" << Res1[1][2] << ";" << Res1[2][2] << std::endl;
		std::cout << std::endl;

		std::cout << std::endl;

		Res = Matrix1*Matrix2;
		std::cout << Res[0][0] << ";" << Res[1][0] << ";" << Res[2][0] << std::endl;
		std::cout << Res[0][1] << ";" << Res[1][1] << ";" << Res[2][1] << std::endl;
		std::cout << Res[0][2] << ";" << Res[1][2] << ";" << Res[2][2] << std::endl;
		std::cout << std::endl;

		std::cout << Res2[0][0] << ";" << Res2[1][0] << ";" << Res2[2][0] << std::endl;
		std::cout << Res2[0][1] << ";" << Res2[1][1] << ";" << Res2[2][1] << std::endl;
		std::cout << Res2[0][2] << ";" << Res2[1][2] << ";" << Res2[2][2] << std::endl;
		std::cout << std::endl;
	}

	lTest_Math(){}
	~lTest_Math(){}
};

#endif // L_TEST_MATH_H
