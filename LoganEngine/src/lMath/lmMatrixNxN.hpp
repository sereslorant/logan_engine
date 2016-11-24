#ifndef LM_MATRIX_NXN_H
#define LM_MATRIX_NXN_H

#include "lmVectorND.hpp"

/** \brief Egy NxN-es mátrixot definiáló osztály
 *
 * \todo KURVÁRA ÁT KELL NÉZNI!!!
 *
 * \warning A műveletek úgy vannak definiálva, hogy az első index oszlopot, a második index sort indexel
 * Kurvahülyén van ez így megcsinálva, át kéne írni.
 *
 * \author Seres Lóránt Gábor
 *
 */
template<class T,unsigned int N>
class lmMatrixNxN
{
private:
    T M[N][N];/**< A mátrix cellái */
public:

    //static const lmMatrixNxN NULL_MATRIX;
    //static const lmMatrixNxN IDENTITY_MATRIX;

    enum ConstructorMode
    {
    	NO_INIT,
        NULLMAT,
        IDENTITY,
    };

    /** \brief Default konstruktor, ami nullmátrixnak vagy egységmátrixnak inicializálja.
     *
     * \param Mode ConstructorMode - Ha NULLMAT, akkor nullmátrixnak, ha IDENTITY, akkor egységmátrixnak inicializálja a mátrixot.
     *
     */
    lmMatrixNxN(ConstructorMode Mode = NO_INIT)
    {
    	if(Mode != NO_INIT)
		{
			for(int i=0;i < N;i++)
			{
				for(int j=0;j < N;j++)
				{
					if((Mode == IDENTITY) && (i==j))
					{
							M[i][j] = 1;
					}
					else
					{
						M[i][j] = 0;
					}
				}
			}
		}
	}

    /** \brief Copy konstruktor
     *
     * \param A const lmMatrixNxN& a másolni kívánt mátrix
     *
     */
    lmMatrixNxN(const lmMatrixNxN &A)
    {
		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				M[i][j] = A[i][j];
			}
		}
	}

    /** \brief Destruktor - nem csinál semmit
     *
     */
    ~lmMatrixNxN()
    {

    }

    /** \brief  Mátrix transzponáló függvény
     *
     * \param Target lmMatrixNxN& ide adjuk vissza a mátrix transzponáltját.
     * \return void
     *
     */
    void Transpose(lmMatrixNxN &Target) const
    {
		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				Target.M[i][j] = M[j][i];
			}
		}
	}

	void GetSubMatrix(lmMatrixNxN<T,N-1> &Target,int I,int J) const
	{
		bool I_Passed = false;
		for(int i=0;i < N;i++)
		{
			if(i == I)
			{
				I_Passed = true;
			}
			else
			{
				bool J_Passed = false;

				for(int j=0;j < N;j++)
				{
					if(J_Passed)
					{
						if(I_Passed)
						{
							Target.M[i-1][j-1] = M[i-1][j-1];
						}
						else
						{
							Target.M[i][j-1] = M[i][j-1];
						}
					}
					else if(j == J)
					{
						J_Passed = true;
					}
					else
					{
						if(I_Passed)
						{
							Target.M[i-1][j] = M[i-1][j];
						}
						else
						{
							Target.M[i][j] = M[i][j];
						}
					}
				}
			}
		}
	}

	void Invert(lmMatrixNxN &Target) const
	{
		Target = lmMatrixNxN(IDENTITY);
	}

    /** \brief Transzponálja a mátrixot, és visszatér a mátrix transzponáltjával
     *
     * \return lmMatrixNxN - a transzponált mátrix
     *
     */
    lmMatrixNxN Transpose() const
    {
		lmMatrixNxN A(NULLMAT);

		Transpose(A);

		return A;
	}

    lmMatrixNxN<T,N-1> GetSubMatrix(int I, int J) const
    {
		lmMatrixNxN<T,N-1> A(lmMatrixNxN<T,N-1>::NULLMAT);

		GetSubMatrix(A,I,J);

		return A;
	}

	lmMatrixNxN Invert() const
	{
		lmMatrixNxN A(NULLMAT);

		Invert(A);

		return A;
	}

	T Trace() const
	{
		T Szercsy = 0.0;

		for(int i=0;i < N;i++)
		{
			Szercsy += (*this)[i][i];
		}

		return Szercsy;//A[0][0] + A[1][1] + A[2][2];
	}

    /** \brief
     *
     * \todo Meg kell csinálni az általános determináns számító függvényt!
     *
     * \return T
     *
     */
	T Determinant() const
    {
    	return 0.0;
    }

    //Operátorok

	/*
	 * TODO: Buffer overflow sérülékenység!
	 *
	 * Be kell csomagolni a visszaadott pointert, hogy bound checket hajtsunk végre.
	 */

    /** \brief Megindexeli a mátrix oszlopait
     *
     * \param i int: index
     * \return T * - a mátrix oszlopára mutató pointer, amit indexelve megkaphatjuk a mátrix egy celláját.
     *
     */
    T *operator[](unsigned int i)
    {
    	if(i < N)
		{
			return &(M[i][0]);
		}
		else
		{
			return nullptr;
		}
	}

	/*
	 * TODO: Buffer overflow sérülékenység!
	 *
	 * Be kell csomagolni a visszaadott pointert, hogy bound checket hajtsunk végre.
	 */

    /** \brief Megindexeli a mátrix oszlopait - konstans tagfüggvény
     *
     * \param i int: index
     * \return T * - a mátrix oszlopára mutató pointer, amit indexelve megkaphatjuk a mátrix egy celláját.
     *
     */
    const T *operator[](unsigned int i)const
    {
		if(i < N)
		{
			return &(M[i][0]);
		}
		else
		{
			return &(M[N-1][0]);
		}
	}

    /** \brief Két mátrix összeadása
     *
     * \param A const lmMatrixNxN &: a hozzáadandó mátrix
     * \return lmMatrixNxN - a művelet eredménye
     *
     */
    lmMatrixNxN operator+(const lmMatrixNxN &A) const
	{
		lmMatrixNxN B(NULLMAT);

		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				B[i][j] = (*this)[i][j] + A[i][j];
			}
		}

		return B;
	}

	lmMatrixNxN operator-(const lmMatrixNxN &A) const
	{
		lmMatrixNxN B(NULLMAT);

		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				B[i][j] = (*this)[i][j] - A[i][j];
			}
		}

		return B;
	}

    /** \brief Mátrix skalárral való szorzása
     *
     * \param l T: a skalár
     * \return lmMatrixNxN - a művelet eredménye
     *
     */
    lmMatrixNxN operator*(T l) const
    {
		lmMatrixNxN B(NULLMAT);

		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				B[i][j] = (*this)[i][j] * l;
			}
		}

		return B;
	}

    /** \brief Mátrix beszorzása jobbról oszlopvektorral
     *
     * \param V const lmVector4D &: Az oszlopvektor
     * \return lmVector4D a művelet eredménye
     *
     */
    /*
    lmVector4D  operator*(const lmVector4D &V) const;
    {
		lmVector4D U;
		const lmMatrix4x4 &This = *this;

		U.X = (This[0][0] * V.X) + (This[1][0] * V.Y) + (This[2][0] * V.Z) + (This[3][0] * V.W);
		U.Y = (This[0][1] * V.X) + (This[1][1] * V.Y) + (This[2][1] * V.Z) + (This[3][1] * V.W);
		U.Z = (This[0][2] * V.X) + (This[1][2] * V.Y) + (This[2][2] * V.Z) + (This[3][2] * V.W);
		U.W = (This[0][3] * V.X) + (This[1][3] * V.Y) + (This[2][3] * V.Z) + (This[3][3] * V.W);

		return U;
	}
    */

    /** \brief Mátrix beszorzása jobbról mátrixszal
     *
     * \param A const lmMatrixNxN &: A mátrix, amivel szorzunk
     * \return lmMatrixNxN a művelet eredménye
     *
     */
    lmMatrixNxN operator*(const lmMatrixNxN &A) const
    {
		lmMatrixNxN B(NULLMAT);

		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				for(int k=0;k < N;k++)
				{
					B[i][j] += (*this)[k][j]*A[i][k];
				}
			}
		}

		return B;
	}

    /** \brief Mátrix értékadás operátor
     *
     * \param A const lmMatrixNxN &: A mátrix, amelyet értékül adunk.
     * \return lmMatrixNxN - *this
     *
     */
    const lmMatrixNxN &operator=(const lmMatrixNxN &A)
    {
		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				M[i][j] = A.M[i][j];
			}
		}

		return *this;
	}

    /** \brief Két mátrix összeadása
     *
     * \param A const lmMatrixNxN &: a hozzáadandó mátrix
     * \return const lmMatrixNxN & - *this
     *
     */
    const lmMatrixNxN &operator+=(const lmMatrixNxN &A)
    {
		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				(*this)[i][j] += A[i][j];
			}
		}

		return *this;
	}

	const lmMatrixNxN &operator-=(const lmMatrixNxN &A)
	{
		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				(*this)[i][j] -= A[i][j];
			}
		}

		return *this;
	}

    /** \brief Mátrix beszorzása skalárral
     *
     * \param l T : a skalár
     * \return const lmMatrixNxN & - *this
     *
     */
    const lmMatrixNxN &operator*=(T l)
	{
		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				M[i][j] *= l;
			}
		}

		return *this;
	}

    /** \brief Mátrix beszorzása jobbról mátrixszal
     *
     * \param A const lmMatrixNxN &: A mátrix, amivel szorzunk
     * \return const lmMatrixNxN & - *this
     *
     */
    const lmMatrixNxN &operator*=(const lmMatrixNxN &A)
    {
		lmMatrixNxN Tmp(NULLMAT);

		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				for(int k=0;k < N;k++)
				{
					Tmp.M[i][j] += this->M[k][j]*A.M[i][k];
				}
			}
		}

		*this = Tmp;

		return *this;
	}

	inline friend lmMatrixNxN<T,N> operator*(T l,const lmMatrixNxN<T,N> &A)
	{
		lmMatrixNxN<T,N> M(lmMatrixNxN<T,N>::NULLMAT);

		for(int i=0;i < N;i++)
		{
			for(int j=0;j < N;j++)
			{
				M[i][j] = A[i][j] * l;
			}
		}

		return M;
	}

	inline friend lmVectorND<T,N>  operator*(const lmMatrixNxN<T,N> &A,const lmVectorND<T,N> &V)
	{
		lmVectorND<T,N> U;
		/*
		U.X = (A[0][0] * V.X) + (A[1][0] * V.Y) + (A[2][0] * V.Z);
		U.Y = (A[0][1] * V.X) + (A[1][1] * V.Y) + (A[2][1] * V.Z);
		U.Z = (A[0][2] * V.X) + (A[1][2] * V.Y) + (A[2][2] * V.Z);
		*/
		for(unsigned int i=0;i < N;i++)
		{
			for(unsigned int j=0;j < N;j++)
			{
				U[i] += A[j][i] * V[j];
			}
		}

		return U;
	}

	inline friend lmVectorND<T,N> operator*(const lmVectorND<T,N> &V,const lmMatrixNxN<T,N> &A)
	{
		lmVectorND<T,N> U;
		/*
		U.X = (A[0][0] * V.X) + (A[0][1] * V.Y) + (A[0][2] * V.Z);
		U.Y = (A[1][0] * V.X) + (A[1][1] * V.Y) + (A[1][2] * V.Z);
		U.Z = (A[2][0] * V.X) + (A[2][1] * V.Y) + (A[2][2] * V.Z);
		*/
		for(unsigned int i=0;i < N;i++)
		{
			for(unsigned int j=0;j < N;j++)
			{
				U[i] += A[i][j] * V[j];
			}
		}

		return U;
	}
};


template<class T,unsigned int N>
lmMatrixNxN<T,N> lmOuter(const lmVectorND<T,N> &U,const lmVectorND<T,N> &V)
{
	lmMatrixNxN<T,N> A;

	for(unsigned int i=0;i < N;i++)
	{
		for(unsigned int j=0;j < N;j++)
		{
			A[i][j] = U[j]*V[i];
		}
	}

	return A;
}

#endif // LM_MATRIX_NXN_H
