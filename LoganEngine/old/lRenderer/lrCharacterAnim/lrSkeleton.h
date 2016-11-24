#ifndef LR_SKELETON
#define LR_SKELETON

#include "../../lInterfaces/lRenderer/lrCharacterAnim/liSkeleton.h"

/** \brief Ez egy virtuális csontvázat modellező class. A virtuális csontváz adja meg,
 * hogy egy skeletal mesh milyen pozíciót vegyen föl kirajzoláskor.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrSkeleton : public liSkeleton
{
public:
    /** \brief Egy darab csont pozícióját és orientációját tartalmazó adatstruktúra.
     *
     */
    struct lrBone : public liBone
    {
        lmVector3D      Position;   /**< A bone pozíció */
        lmQuaternion    Orientation;/**< A bone orientációja */

        int ParentIndex;/**< A bone szülőjének az indexe. Ha ő a gyökér, akkor -1. */

        virtual const lmVector3D &GetPosition() const override {return Position;}
        virtual const lmQuaternion &GetOrientation() const override {return Orientation;}
        virtual int GetParentIndex() const override {return ParentIndex;}

        virtual void SetPosition(const lmVector3D &position) override {Position = position;}
        virtual void SetOrientation(const lmQuaternion &orientation) override {Orientation = orientation;}
        virtual void SetParentIndex(int parent_index) override {ParentIndex = parent_index;}


        void operator=(const lrBone &b)
        {
        	Position	= b.Position;
        	Orientation	= b.Orientation;

        	ParentIndex	= b.ParentIndex;
        }


        /** \brief Default konstruktor - a parent indexet (-1)-re állítja.
         *
         */
        lrBone();

        /** \brief Copy konstruktor
         *
         * \param b const lrBone &: a lemásolni kívánt bone-ra mutató referencia
         *
         */
        lrBone(const lrBone &b);

        /** \brief Konstruktor, amely az összes tagváltozót inicializálja.
         *
         * \param position const lmVector3D &:       A bone pozíciója
         * \param orientation const lmQuaternion &:  A bone orientációja
         * \param parentIndex int:                  A bone szülőjének az indexe
         *
         */
        lrBone(const lmVector3D &position,const lmQuaternion &orientation,int parentIndex);

        virtual ~lrBone() override
        {}
    };

    /** \brief Egy csont transzformációját tartalmazó adatstruktúra, hogy ne
     * kelljen minden lekérdezés esetén újra kiszámolni a mátrixot.
     *
     */
    struct lrBoneTransform
    {
        bool MatrixReady; /**< True, ha a mátrixot már kiszámoltuk, false egyébként. */
        lmMatrix4x4 Matrix; /**< A transzformáció mátrixa */

        /** \brief Default konstruktor, ami a MatrixReady-t false-ra állítja.
         *
         */
        lrBoneTransform():MatrixReady(false)
        {

        }
    };
private:
    std::vector<lrBone> Bones;                      /**< A csontokat tároló tömb. */
    std::vector<lrBoneTransform> BoneTransform;     /**< Az egyes csontokhoz tartozó transzformációk mátrixait tartalmazó tömb. */
    std::vector<lrBoneTransform> InverseTransform;     /**< Az egyes csontokhoz tartozó inverz transzformációk mátrixait tartalmazó tömb. */
    std::map<std::string,unsigned int> BoneNames;   /**< A fájlból beolvasható ("név","index") párosításokat tartalmazó asszociatív tömb. */

public:

    /** \brief A csontokat tároló tömb méretét visszaadó függvény
     *
     * \return unsigned int - a csontokat tároló tömb mérete.
     *
     */
    virtual unsigned int Size() const override;

    void Resize(unsigned int size)
    {
    	Bones.resize(size);
    	BoneTransform.resize(size);
    	InverseTransform.resize(size);
    }

    /** \brief Az i-edik indexű csontot elkérő operátor. Konstans tagfüggvény.
     *
     * \param i unsigned int : a csont indexe
     * \return const lrBone & - a csontra mutató referencia
     *
     */
    virtual const liBone *operator[](unsigned int i) const override;

    /** \brief Az i-edik indexű csontot elkérő operátor.
     *
     * \param i unsigned int: a csont indexe
     * \return lrBone & - a csontra mutató referencia
     *
     */
    virtual liBone *operator[](unsigned int i) override;

    /** \brief Az i nevű csontot elkérő operátor. Konstans tagfüggvény.
     *
     * \param i const std::string &: a csont neve
     * \return const lrBone & - a csontra mutató referencia
     *
     */
    virtual const liBone *operator[](const std::string &i) const override;

    /** \brief Az i nevű csontot elkérő operátor.
     *
     * \param i const std::string &: a csont neve
     * \return lrBone & - a csontra mutató referencia
     *
     */
    virtual liBone *operator[](const std::string &i) override;

    /** \brief Megadja az Index-edik csont transzformációját.
     *
     * \param Index unsigned int: a csont indexe
     * \return const lmMatrix4x4 & - a csont transzformációja
     *
     */

    virtual const lmMatrix4x4 &GetTransformation(unsigned int Index) override;

    virtual const lmMatrix4x4 &GetInverseTransformation(unsigned int Index) override
    {
    	if(InverseTransform[Index].MatrixReady == false)
		{
			lmMatrix4x4 Transformation = lmRotate4x4(Bones[Index].Orientation.GetInverse()) * lmTranslate4x4(-1.0*Bones[Index].Position);

			if(Bones[Index].ParentIndex == -1)
			{InverseTransform[Index].Matrix = Transformation;}
			else
			{InverseTransform[Index].Matrix =  Transformation * GetInverseTransformation(Bones[Index].ParentIndex);}

			InverseTransform[Index].MatrixReady = true;
		}

		return InverseTransform[Index].Matrix;
    }

    virtual void SetBoneNames(std::vector<std::pair<std::string,unsigned int> >bone_names) override
    {
    	BoneNames.clear();

    	for(auto I : bone_names)
		{
			BoneNames[I.first] = I.second;
		}
    }
    virtual void SetBoneNames(std::map<std::string,unsigned int> bone_names) override
    {
    	BoneNames = bone_names;
    }

    virtual std::map<std::string,unsigned int> &GetBoneNames() override
    {
    	return BoneNames;
    }

    virtual const std::map<std::string,unsigned int> &GetBoneNames() const override
    {
    	return BoneNames;
    }

    /** \brief Másoló operátor
     *
     * \param s const lrSkeleton &: a másolni kívánt csontváz
     * \return void
     *
     */
    void operator=(const liSkeleton &s) = delete;

    virtual void Copy(const liSkeleton &s) override
	{
		Bones.resize(s.Size());

		for(unsigned int i=0;i < Bones.size();i++)
		{
			Bones[i].SetPosition(s[i]->GetPosition());
			Bones[i].SetOrientation(s[i]->GetOrientation());
			Bones[i].SetParentIndex(s[i]->GetParentIndex());
		}

		BoneNames = s.GetBoneNames();

		BoneTransform.resize(Bones.size());
		InverseTransform.resize(Bones.size());

		for(unsigned int i=0;i < BoneTransform.size();i++)
		{
			BoneTransform[i].MatrixReady = false;
		}

		for(unsigned int i=0;i < InverseTransform.size();i++)
		{
			InverseTransform[i].MatrixReady = false;
		}
	}

    /** \brief Default konstruktor
     *
     */
    lrSkeleton();

    /** \brief Copy konstruktor
     *
     * \param s const lrSkeleton &: a másolni kívánt skeleton
     *
     */
    lrSkeleton(const lrSkeleton &s);

    /** \brief Konstruktor, mely a skeleton minden tagváltozóját inicializálja.
     *
     * \param bones std::vector<lrBone> &:                       a csontokat tároló tömb.
     * \param boneNames std::map<std::string,unsigned int> &:    a ("név","index") kombókat tároló asszociatív tömb.
     *
     */
    lrSkeleton(std::vector<lrBone> &bones,std::map<std::string,unsigned int> &boneNames);

    virtual ~lrSkeleton() override
    {}
};

void lrLinearInterpolation(const liSkeleton *skeleton_0,const liSkeleton *skeleton_1,double weight,liSkeleton *dest);

#endif
