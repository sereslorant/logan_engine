
#include "Csirguru.h"

const lmVector3D CR_InitialVelocity = CR_Speed*lmVector3D({0.0,1.0 /sqrt(2.0),-1.0/sqrt(2.0)});
const lmVector3D CR_FinalVelocity = CR_Speed*lmVector3D({0.0,0.0,-1.0});

lmVector3D LinearCombination(const lmVector3D &u,const lmVector3D &v1,const lmVector3D &v2,const lmVector3D &v3)
{
	return u[0] * v1 + u[1] * v2 + u[2] * v3;
}

void RotateX(lmVector3D &v0,lmVector3D &v1,lmVector3D &v2,float angle)
{
	v0 = lmVector3D({1.0,0.0,0.0});
	v1 = lmVector3D({0.0, cos(angle),sin(angle)});
	v2 = lmVector3D({0.0,-sin(angle),cos(angle)});
}

void RotateY(lmVector3D &v0,lmVector3D &v1,lmVector3D &v2,float angle)
{
	v0 = lmVector3D({ cos(angle),0.0,sin(angle)});
	v1 = lmVector3D({0.0,1.0,0.0});
	v2 = lmVector3D({-sin(angle),0.0,cos(angle)});
}

void RotateZ(lmVector3D &v0,lmVector3D &v1,lmVector3D &v2,float angle)
{
	v0 = lmVector3D({ cos(angle),sin(angle),0.0});
	v1 = lmVector3D({-sin(angle),cos(angle),0.0});
	v2 = lmVector3D({0.0,0.0,1.0});
}

lmVector3D BezierCtrlPoints[NUM_BEZIER_CURVES][BEZIER_SIZE] = {
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})},
	{lmVector3D({0.0,-186.6667,0}),lmVector3D({295,-310,0}),lmVector3D({440,148.333,0}),lmVector3D({0.0,453.333,0}),lmVector3D({-413.333,95,0}),lmVector3D({-270,-335,0}),lmVector3D({0.0,-186.6667,0})}
	};

//float BezierDiv[NUM_BEZIER_CURVES] = {2200,1200,1800,1750,1000,500.0,400.0,500.0,1000.0};
float BezierDiv[NUM_BEZIER_CURVES] = {220,120,180,175,100,50,40,50,100};

TransfiniteInterpolation *InitCsirguruTest()
{
	//float GerincDivisor = 400.0;
	float GerincDivisor = 40.0;

	lmVector3D Gerinc[] = {lmVector3D({-493.333,-175,0}),lmVector3D({-400,-241.667,0}),lmVector3D({-305,-263.333,0}),lmVector3D({-250,-190,0}),lmVector3D({-225,-113.333,0}),lmVector3D({-175,-73.3333,0}),lmVector3D({-78.3333,-70,0}),lmVector3D({41.6667,-76.6667,0}),lmVector3D({140,-85,0}),lmVector3D({230,-111.667,0}),lmVector3D({321.667,-155,0})};
	const unsigned int GERINC_SIZE = sizeof(Gerinc) / sizeof(lmVector3D);

	CatmullRomCsirguru GerincCurve;

	for(unsigned int i=0;i < GERINC_SIZE;i++)
	{
		Gerinc[i][2] = -Gerinc[i][0];
		Gerinc[i][1] = -Gerinc[i][1];
		Gerinc[i][0] = 0.0;

		Gerinc[i] = Gerinc[i] / GerincDivisor;

		GerincCurve.AddControlPoint(Gerinc[i]);
	}

	lmVector3D GerincDerivative[GERINC_SIZE];

	GerincDerivative[0].Normalize();

	for(unsigned int i=1;i < GERINC_SIZE -1;i++)
	{
		GerincDerivative[i] = GerincCurve.r_derivative(i*CR_DELTA_T);
		GerincDerivative[i].Normalize();
	}

	GerincDerivative[GERINC_SIZE -1].Normalize();

	BezierCurve BezierCurves[NUM_BEZIER_CURVES];

	for(unsigned int i=0;i < NUM_BEZIER_CURVES;i++)
	{
		float Angle = acos(lmDot(GerincDerivative[i+1],lmVector3D({0.0,0.0,-1.0})));
		float Sgn = lmDot(GerincDerivative[i+1],lmVector3D({0.0,1.0,0.0}));
		Sgn /= fabs(Sgn);

		for(unsigned int j=0;j < BEZIER_SIZE;j++)
		{
			BezierCtrlPoints[i][j] = BezierCtrlPoints[i][j] / BezierDiv[i];
			lmVector3D v0,v1,v2;
			RotateX(v0,v1,v2,Sgn * Angle);

			BezierCtrlPoints[i][j] = LinearCombination(BezierCtrlPoints[i][j],v0,v1,v2);

			BezierCtrlPoints[i][j] = BezierCtrlPoints[i][j] + Gerinc[1+i];
		}

		BezierCurves[i] = BezierCurve(BezierCtrlPoints[i]);
	}

	CatmullRomCsirguru CsirguruSpline[NUM_CATMULL_ROM_SPLINE];

	for(unsigned int i=0;i < NUM_CATMULL_ROM_SPLINE;i++)
	{
		for(unsigned int j=0;j < NUM_BEZIER_CURVES;j++)
		{
			CsirguruSpline[i].AddControlPoint(BezierCurves[j].GetPoint(i*BEZIER_DELTA_T));
		}
	}
	
	TransfiniteInterpolation *Csirguru = new TransfiniteInterpolation(BezierCurves,CsirguruSpline);

	Csirguru->Farok = GerincCurve.r((GERINC_SIZE - 1.0) * CR_DELTA_T);
	Csirguru->Csor = GerincCurve.r(0.2);

	return Csirguru;
}
