#ifndef CSIRGURU_H
#define CSIRGURU_H

#include <GL/gl.h>

#include <lCore/lMath/lMath.h>

template<class Type>
class DynamicArray
{
private:
	static const unsigned int DEFAULT_ALLOCATED_MEMORY = 10;
	unsigned int ArraySize,AllocatedMemory;
	Type *Array,Default;

public:
	DynamicArray()
	{
		Array = new Type[DEFAULT_ALLOCATED_MEMORY];

		AllocatedMemory = DEFAULT_ALLOCATED_MEMORY;
		ArraySize = 0;
	}

	DynamicArray(const DynamicArray &d)
	{
		Array = new Type[d.ArraySize];

		AllocatedMemory = d.ArraySize;
		ArraySize = d.ArraySize;

		for(int i=0;i < ArraySize;i++)
		{
			Array[i] = d.Array[i];
		}
	}

	~DynamicArray()
	{
		delete[] Array;
	}

	void operator=(const DynamicArray &d)
	{
		delete[] Array;

		Array = new Type[d.ArraySize];

		AllocatedMemory = d.ArraySize;
		ArraySize = d.ArraySize;

		for(unsigned int i=0;i < ArraySize;i++)
		{
			Array[i] = d.Array[i];
		}
	}

	void push_back(Type Var)
	{
		if (ArraySize == AllocatedMemory)
		{
			Resize(AllocatedMemory + DEFAULT_ALLOCATED_MEMORY);
		}

		Array[ArraySize] = Var;
		ArraySize++;
	}

	void pop_back()
	{
		ArraySize--;
	}

	bool Resize(unsigned int NewSize)
	{
		if (NewSize < ArraySize)
		{
			return false;
		}

		Type *Tmp = Array;

		Array = new Type[NewSize];
		AllocatedMemory = NewSize;

		for (unsigned int i=0; i< size();i++)
		{
			Array[i] = Tmp[i];
		}

		delete[] Tmp;

		return true;
	}

	Type &back()
	{
		return Array[ArraySize-1];
	}

	unsigned int size()
	{
		return ArraySize;
	}

	Type &operator [] (const unsigned int Index)
	{
		if(Index > ArraySize)
		{
			Default = Type();

			return Default;
		}
		else
		{
			return Array[Index];
		}
	}
};

constexpr unsigned int BEZIER_SIZE = 7;

struct BezierCurve
{
	lmVector3D ControlPoint[BEZIER_SIZE];

	float GetBinomialisMutyur(unsigned int n,unsigned int i)
	{
		float BinomialisMutyur = 1.0;
		for(unsigned int j=1;j <= i;j++)
		{
			BinomialisMutyur *= (float)(n - i + j)/j;
		}

		return BinomialisMutyur;
	}

	lmVector3D GetPoint(float t)
	{
		lmVector3D Point;

		for(unsigned int i=0;i < BEZIER_SIZE;i++)
		{
			unsigned int n = BEZIER_SIZE - 1;

			float BinomialisMutyur = GetBinomialisMutyur(n,i);

			float Weight = BinomialisMutyur*(pow(t,i))*(pow(1.0 - t,n - i));

			Point = Point + Weight * ControlPoint[i];
		}

		return Point;
	}

	lmVector3D GetDerivative(float t)
	{
		lmVector3D Point;

		for(unsigned int i=0;i < BEZIER_SIZE;i++)
		{
			unsigned int n = BEZIER_SIZE - 1;

			float BinomialisMutyur = GetBinomialisMutyur(n,i);

			float Weight1;
			if(i == 0)
				{Weight1 = 0.0;}
			else
				{Weight1 = i*pow(t,i - 1)*pow(1.0 - t,n - i);}

			float Weight2;
			if(i == n)
				{Weight2 = 0.0;}
			else
				{Weight2 = (n - i)*pow(t,i)*pow(1.0 - t,n - i - 1);}

			float Weight = BinomialisMutyur * (Weight1 - Weight2);

			Point = Point + Weight * ControlPoint[i];
		}

		return Point;
	}

	BezierCurve()
	{}

	BezierCurve(lmVector3D control_point[])
	{
		for(unsigned int i=0;i < BEZIER_SIZE;i++)
		{
			ControlPoint[i] = control_point[i];
		}
	}
};

constexpr unsigned int NUM_BEZIER_CURVES = 9;
constexpr unsigned int NUM_CATMULL_ROM_SPLINE = 10;


constexpr float BEZIER_DELTA_T = 1.0/NUM_CATMULL_ROM_SPLINE;
constexpr float CR_DELTA_T = 2.0;

constexpr float CR_Speed = 0.125;

extern const lmVector3D CR_InitialVelocity;
extern const lmVector3D CR_FinalVelocity;

/*
 * Catmull-Rom spline
 * Forrás: a bmemodel.ppt grafika diák
 *
 */
class CatmullRomCsirguru
{
public:
	DynamicArray<lmVector3D> cps;	// control points
	DynamicArray<lmVector3D> Velocity; //A kontroll pontok sebességvektora
	DynamicArray<float>  ts;	// parameter (knot) values

	/*
	 * Hermite interpoláció
	 * Forrás: a bmemodel.ppt grafika diák
	 */
	static lmVector3D Hermite(const lmVector3D &p0,const lmVector3D &p1,const lmVector3D &v0,const lmVector3D &v1,float t0,float t1,float t)
	{
		lmVector3D a0 = p0;
		lmVector3D a1 = v0;
		lmVector3D a2 = (3.0*(p1-p0)/(pow(t1-t0,2))) - ((v1+2.0*v0)/(t1-t0));
		lmVector3D a3 = (2.0*(p0-p1)/(pow(t1-t0,3))) + ((v1+v0)/pow(t1-t0,2));

		return a0 + a1*(t-t0) + a2*pow(t-t0,2) + a3*pow(t-t0,3);
	}

	/*
	 * Hermite interpoláció
	 * Forrás: a bmemodel.ppt grafika diák
	 */
	static lmVector3D HermiteDerivative(const lmVector3D &p0,const lmVector3D &p1,const lmVector3D &v0,const lmVector3D &v1,float t0,float t1,float t)
	{
		lmVector3D a1 = v0;
		lmVector3D a2 = (3.0*(p1-p0)/(pow(t1-t0,2))) - ((v1+2.0*v0)/(t1-t0));
		lmVector3D a3 = (2.0*(p0-p1)/(pow(t1-t0,3))) + ((v1+v0)/pow(t1-t0,2));

		return a1 + 2.0*a2*(t-t0) + 3.0*a3*pow(t-t0,2);
	}

	static lmVector3D AverageVelocity(const lmVector3D &p0,const lmVector3D &p1,float t0,float t1)
	{
		return (p1-p0)/(t1-t0);
	}

	void AddControlPoint(lmVector3D cp)
	{
		cps.push_back(cp);
		ts.push_back(ts.size() * CR_DELTA_T);
		Velocity.push_back(CR_FinalVelocity);

		if(cps.size() == 1)
		{
			Velocity[0] = CR_InitialVelocity;
		}
		else if(cps.size() > 2)
		{
			const lmVector3D &Pprev2 = cps[cps.size() - 3]; float tprev2 = ts[ts.size() - 3];
			const lmVector3D &Pprev1 = cps[cps.size() - 2]; float tprev1 = ts[ts.size() - 2];
			const lmVector3D &Pfin   = cps[cps.size() - 1]; float tfin   = ts[ts.size() - 1];

			Velocity[Velocity.size()-2] = (AverageVelocity(Pprev2,Pprev1,tprev2,tprev1) + AverageVelocity(Pprev1,Pfin,tprev1,tfin)) / 2.0;
		}
	}

	lmVector3D r(float t) {
		for(unsigned int i = 0; i < cps.size() - 1; i++)
		{
			if (ts[i] <= t && t <= ts[i+1])
			{
				return Hermite(cps[i],cps[i+1],Velocity[i],Velocity[i+1],ts[i],ts[i+1],t);
			}
		}

		return lmVector3D({0.0,0.0,0.0});
	}

	lmVector3D r_derivative(float t) {
		for(unsigned int i = 0; i < cps.size() - 1; i++)
		{
			if (ts[i] <= t && t <= ts[i+1])
			{
				return HermiteDerivative(cps[i],cps[i+1],Velocity[i],Velocity[i+1],ts[i],ts[i+1],t);
			}
		}

		return lmVector3D({0.0,0.0,0.0});
	}
};

//constexpr unsigned int VERTICES_DIM_X = 50;
//constexpr unsigned int VERTICES_DIM_Y = 50;

//constexpr unsigned int VERTICES_DIM_X = 20;
//constexpr unsigned int VERTICES_DIM_Y = 20;

constexpr unsigned int VERTICES_DIM_X = 10;
constexpr unsigned int VERTICES_DIM_Y = 10;

//constexpr unsigned int VERTICES_DIM_X = 5;
//constexpr unsigned int VERTICES_DIM_Y = 5;

class TransfiniteInterpolation
{
public:
	BezierCurve CrossSections[NUM_BEZIER_CURVES];
	CatmullRomCsirguru CsirguruFaroktolCsorig[NUM_CATMULL_ROM_SPLINE];

	float CRSize()
	{
		return CsirguruFaroktolCsorig[0].ts[CsirguruFaroktolCsorig[0].ts.size()-1];
	}

	lmVector3D GetPoint(float t_cr,float t_bezier)
	{
		unsigned int i = floor(t_cr / CR_DELTA_T);
		unsigned int j = floor(t_bezier / BEZIER_DELTA_T);

		if(i < NUM_BEZIER_CURVES-1 && j < NUM_CATMULL_ROM_SPLINE)
		{
			unsigned int I = i + 1;
			unsigned int J = (j + 1);

			J = J % NUM_CATMULL_ROM_SPLINE;

			lmVector3D Cj_t = CsirguruFaroktolCsorig[j].r(t_cr);
			lmVector3D CJ_t = CsirguruFaroktolCsorig[J].r(t_cr);

			lmVector3D Bi_t = CrossSections[i].GetPoint(t_bezier);
			lmVector3D BI_t = CrossSections[I].GetPoint(t_bezier);

			float t_cr_ratio = (t_cr - i*CR_DELTA_T)/CR_DELTA_T;
			float t_bezier_ratio = (t_bezier - j*BEZIER_DELTA_T)/BEZIER_DELTA_T;

			CatmullRomCsirguru &Cj = CsirguruFaroktolCsorig[j];
			CatmullRomCsirguru &CJ = CsirguruFaroktolCsorig[J];

			lmVector3D Pij = Cj.cps[i];lmVector3D PIj = Cj.cps[I];
			lmVector3D PiJ = CJ.cps[i];lmVector3D PIJ = CJ.cps[I];

			lmVector3D InterpCuccok = (1.0-t_bezier_ratio)*(Cj_t) + t_bezier_ratio*(CJ_t) + (1.0-t_cr_ratio)*(Bi_t) + t_cr_ratio*(BI_t);

			lmVector3D WeightedPts = (1.0-t_bezier_ratio)*(1.0-t_cr_ratio)*Pij + (1.0-t_bezier_ratio)*(t_cr_ratio)*PIj + (t_bezier_ratio)*(1.0-t_cr_ratio)*PiJ + (t_bezier_ratio)*(t_cr_ratio)*PIJ;

			return InterpCuccok - WeightedPts;
		}

		return lmVector3D({0.0,0.0,0.0});
	}

	lmVector3D GetDerivative_t_bezier(float t_cr,float t_bezier)
	{
		unsigned int i = floor(t_cr / CR_DELTA_T);
		unsigned int j = floor(t_bezier / BEZIER_DELTA_T);

		if(i < NUM_BEZIER_CURVES-1 && j < NUM_CATMULL_ROM_SPLINE)
		{
			int I = i + 1;
			int J = (j + 1);

			J = J % NUM_CATMULL_ROM_SPLINE;

			lmVector3D Cj_t = CsirguruFaroktolCsorig[j].r(t_cr);
			lmVector3D CJ_t = CsirguruFaroktolCsorig[J].r(t_cr);

			lmVector3D dBi_tdt = CrossSections[i].GetDerivative(t_bezier);
			lmVector3D dBI_tdt = CrossSections[I].GetDerivative(t_bezier);

			float t_cr_ratio = (t_cr - i*CR_DELTA_T)/CR_DELTA_T;
			//float t_bezier_ratio = (t_bezier - j*BEZIER_DELTA_T)/BEZIER_DELTA_T;

			CatmullRomCsirguru &Cj = CsirguruFaroktolCsorig[j];
			CatmullRomCsirguru &CJ = CsirguruFaroktolCsorig[J];

			lmVector3D Pij = Cj.cps[i];lmVector3D PIj = Cj.cps[I];
			lmVector3D PiJ = CJ.cps[i];lmVector3D PIJ = CJ.cps[I];

			lmVector3D InterpCuccok = (-1.0)*(Cj_t) + (CJ_t) + (1.0-t_cr_ratio)*(dBi_tdt) + t_cr_ratio*(dBI_tdt);

			lmVector3D WeightedPts = (-1.0)*(1.0-t_cr_ratio)*Pij + (-1.0)*(t_cr_ratio)*PIj + (1.0-t_cr_ratio)*PiJ + (t_cr_ratio)*PIJ;

			return InterpCuccok - WeightedPts;
		}

		return lmVector3D({0.0,0.0,0.0});
	}

	lmVector3D GetPoint_t_cr(float t_cr,float t_bezier)
	{
		unsigned int i = floor(t_cr / CR_DELTA_T);
		unsigned int j = floor(t_bezier / BEZIER_DELTA_T);

		if(i < NUM_BEZIER_CURVES-1 && j < NUM_CATMULL_ROM_SPLINE)
		{
			unsigned int I = i + 1;
			unsigned int J = (j + 1);

			J = J % NUM_CATMULL_ROM_SPLINE;

			lmVector3D dCj_tdt = CsirguruFaroktolCsorig[j].r_derivative(t_cr);
			lmVector3D dCJ_tdt = CsirguruFaroktolCsorig[J].r_derivative(t_cr);

			lmVector3D Bi_t = CrossSections[i].GetPoint(t_bezier);
			lmVector3D BI_t = CrossSections[I].GetPoint(t_bezier);

			//float t_cr_ratio = (t_cr - i*CR_DELTA_T)/CR_DELTA_T;
			float t_bezier_ratio = (t_bezier - j*BEZIER_DELTA_T)/BEZIER_DELTA_T;

			CatmullRomCsirguru &Cj = CsirguruFaroktolCsorig[j];
			CatmullRomCsirguru &CJ = CsirguruFaroktolCsorig[J];

			lmVector3D Pij = Cj.cps[i];lmVector3D PIj = Cj.cps[I];
			lmVector3D PiJ = CJ.cps[i];lmVector3D PIJ = CJ.cps[I];

			lmVector3D InterpCuccok = (1.0-t_bezier_ratio)*(dCj_tdt) + t_bezier_ratio*(dCJ_tdt) - (Bi_t) + (BI_t);

			lmVector3D WeightedPts = (1.0-t_bezier_ratio)*(-1.0)*Pij + (1.0-t_bezier_ratio)*PIj + (t_bezier_ratio)*(-1.0)*PiJ + (t_bezier_ratio)*PIJ;

			return InterpCuccok - WeightedPts;
		}

		return lmVector3D({0.0,0.0,0.0});
	}

	lmVector3D Vertices[VERTICES_DIM_X][VERTICES_DIM_Y];
	lmVector3D Normals[VERTICES_DIM_X][VERTICES_DIM_Y];

	lmVector3D Csor;
	lmVector3D Farok;

	void Discretize()
	{
		const float CR_RESOLUTION = CRSize() / VERTICES_DIM_X;
		const float BEZIER_RESOLUTION = 1.0 / VERTICES_DIM_Y;

		for(unsigned int i = 0;i < VERTICES_DIM_X;i++)
		{
			float t_cr = i*CR_RESOLUTION;

			for(unsigned int j = 0;j < VERTICES_DIM_Y;j++)
			{
				float t_bezier = j*BEZIER_RESOLUTION;

				lmVector3D Vertex = GetPoint(t_cr,t_bezier);

				lmVector3D dCsirke_dBezier	= GetDerivative_t_bezier(t_cr,t_bezier);
				lmVector3D dCsirke_dCatmull	= GetPoint_t_cr(t_cr,t_bezier);

				lmVector3D Normal = lmCross(dCsirke_dCatmull,dCsirke_dBezier);
				Normal.Normalize();

				Vertices[i][j]	= Vertex;
				Normals[i][j]	= Normal;
			}
		}
	}

	TransfiniteInterpolation()
	{}

	TransfiniteInterpolation(BezierCurve bezier_curves[],CatmullRomCsirguru catmull_rom[])
	{
		for(unsigned int i=0;i < NUM_BEZIER_CURVES;i++)
		{
			CrossSections[i] = bezier_curves[i];
		}

		for(unsigned int i=0;i < NUM_CATMULL_ROM_SPLINE;i++)
		{
			CsirguruFaroktolCsorig[i] = catmull_rom[i];
		}

		Discretize();
	}
};

TransfiniteInterpolation *InitCsirguruTest();

#endif // CSIRGURU_H
