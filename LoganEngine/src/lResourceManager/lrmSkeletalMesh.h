#ifndef LRM_SKELETAL_MESH_H
#define LRM_SKELETAL_MESH_H

#include "lrmStaticMesh.h"

/** \brief Egy skeletal mesh fájlból betöltött adatai.
 *
 * \todo Alaposan át kell szervezni.
 *
 * \author Seres Lóránt Gábor
 *
 */
struct lrmSkeletalMesh
{
	static constexpr unsigned int MAX_WEIGHT_COUNT = 4;

    /** \brief Egy vertexre ható csontok súlyai.
     */
    struct lrmWeight
    {
    private:
        float Weight[MAX_WEIGHT_COUNT]; /**< A súlyokat tartalmazó fix méretű tömb. */
    public:

        /** \brief Konstruktor, amely minden súlyt 0-ra inicializál.
         *
         */
        lrmWeight()
		{
			for(unsigned int i=0;i < MAX_WEIGHT_COUNT;i++)
			{
				Weight[i] = 0.0;
			}
		}

        /** \brief Operátor, mellyel el lehet érni a megadott indexű súlyt.
         *
         * \param i unsigned int: a súly indexe
         * \return float& - az i-edik súlyra mutató referencia.
         *
         */
        float &operator[](unsigned int i)
        {
			return Weight[i];
		}

        /** \brief Operátor, mellyel el lehet érni a megadott indexű súlyt. Konstans tagfüggvény.
         *
         * \param i unsigned int: a súly indexe
         * \return float - az i-edik súly értéke.
         *
         */
        const float &operator[](unsigned int i) const
        {
			return Weight[i];
		}
    };

    /** \brief A vertexre ható csont indexe.
     */
    struct lrmBoneId
    {
    private:
        int Id[MAX_WEIGHT_COUNT];/**< A bone indexeket tartalmazó fix méretű tömb. */
    public:

        /** \brief Konstruktor, amely minden tömbelemet (-1)-re inicializál.
         *
         */
        lrmBoneId()
		{
			for(unsigned int i=0;i < MAX_WEIGHT_COUNT;i++)
			{
				Id[i] = -1;
			}
		}

        /** \brief Operátor, mellyel el lehet érni a megadott indexű bone id-t.
         *
         * \param i unsigned int: a bone id indexe
         * \return int& - az i-edik bone id-re mutató referencia.
         *
         */
        int &operator[](unsigned int i)
        {
			return Id[i];
		}

        /** \brief Operátor, mellyel el lehet érni a megadott indexű bone id-t. Konstans tagfüggvény.
         *
         * \param i unsigned int: a bone id indexe
         * \return int - az i-edik bone id értéke.
         *
         */
        const int &operator[](unsigned int i) const
        {
        	return Id[i];
        }
    };

    lrmStaticMesh BindPoseMesh;

    std::vector<lrmBoneId>     BoneId;     /**< A bone id-ket tartalmazó tömb */
    std::vector<lrmWeight>     Weights;    /**< A súlyokat tartalmazó tömb */

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
    lrmSkeletalMesh(){}

    /** \brief Destruktor, nem csinál semmit
     *
     */
    ~lrmSkeletalMesh(){}
};


#endif // LRM_SKELETAL_MESH_H
