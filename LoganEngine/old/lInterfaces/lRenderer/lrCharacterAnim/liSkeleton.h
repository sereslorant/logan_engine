#ifndef LI_SKELETON_H
#define LI_SKELETON_H

#include "../../../lMath/lMath.h"

#include <string>
#include <vector>
#include <map>

class liSkeleton
{
public:
    /** \brief Egy darab csont pozícióját és orientációját tartalmazó adatstruktúra.
     *
     */
    struct liBone
    {
	public:
        //virtual void operator=(const liBone &b) = 0;
        virtual const lmVector3D &GetPosition() const = 0;
        virtual const lmQuaternion &GetOrientation() const = 0;
        virtual int GetParentIndex() const = 0;

        virtual void SetPosition(const lmVector3D &position) = 0;
        virtual void SetOrientation(const lmQuaternion &orientation) = 0;
        virtual void SetParentIndex(int parent_index) = 0;

        void operator=(const liBone &bone) = delete;
        liBone(const liBone &bone) = delete;

        /** \brief Default konstruktor - a parent indexet (-1)-re állítja.
         *
         */
        liBone(){};

        virtual ~liBone() {}
    };

    /** \brief A csontokat tároló tömb méretét visszaadó függvény
     *
     * \return unsigned int - a csontokat tároló tömb mérete.
     *
     */
    virtual unsigned int Size() const = 0;

    /** \brief Az i-edik indexű csontot elkérő operátor. Konstans tagfüggvény.
     *
     * \param i unsigned int : a csont indexe
     * \return const lrBone & - a csontra mutató referencia
     *
     */
    virtual const liBone *operator[](unsigned int i) const = 0;

    /** \brief Az i-edik indexű csontot elkérő operátor.
     *
     * \param i unsigned int: a csont indexe
     * \return lrBone & - a csontra mutató referencia
     *
     */
    virtual liBone *operator[](unsigned int i) = 0;

    /** \brief Az i nevű csontot elkérő operátor. Konstans tagfüggvény.
     *
     * \param i const std::string &: a csont neve
     * \return const lrBone & - a csontra mutató referencia
     *
     */
    virtual const liBone *operator[](const std::string &i) const = 0;

    /** \brief Az i nevű csontot elkérő operátor.
     *
     * \param i const std::string &: a csont neve
     * \return lrBone & - a csontra mutató referencia
     *
     */
    virtual liBone *operator[](const std::string &i) = 0;

    /** \brief Megadja az Index-edik csont transzformációját.
     *
     * \param Index unsigned int: a csont indexe
     * \return const lmMatrix4x4 & - a csont transzformációja
     *
     */

    virtual const lmMatrix4x4 &GetTransformation(unsigned int Index) = 0;

    virtual const lmMatrix4x4 &GetInverseTransformation(unsigned int Index) = 0;

    virtual void SetBoneNames(std::vector<std::pair<std::string,unsigned int> >bone_names) = 0;
    virtual void SetBoneNames(std::map<std::string,unsigned int> bone_names) = 0;

    virtual std::map<std::string,unsigned int> &GetBoneNames() = 0;
    virtual const std::map<std::string,unsigned int> &GetBoneNames() const = 0;

    /** \brief Másoló operátor
     *
     * \param s const lrSkeleton &: a másolni kívánt csontváz
     * \return void
     *
     */
    void operator=(const liSkeleton &s) = delete;

    virtual void Copy(const liSkeleton &s) = 0;

    /** \brief Konstruktor, mely a skeleton minden tagváltozóját inicializálja.
     *
     * \param bones std::vector<lrBone> &:                       a csontokat tároló tömb.
     * \param boneNames std::map<std::string,unsigned int> &:    a ("név","index") kombókat tároló asszociatív tömb.
     *
     */
    liSkeleton(){};

    virtual ~liSkeleton(){}
};

#endif // LI_SKELETON_H
