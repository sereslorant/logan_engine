#ifndef LRM_TMP_SKELETAL_MESH
#define LRM_TMP_SKELETAL_MESH

#include "../lInterfaces/lResourceManager/liGPUFactory.h"

#include <vector>
#include <fstream>

/** \brief Egy skeletal mesh fájlból betöltött adatai.
 *
 * \todo Alaposan át kell szervezni.
 *
 * \author Seres Lóránt Gábor
 *
 */
struct lrmSkeletalMesh : public liSkeletalMeshResource
{
    //static const unsigned int MAX_WEIGHT_COUNT = 4; /**< A maximális vertexre ható csontok száma */

	virtual unsigned int GetVertexBufferLength() const override {return Vertices.size();}

	virtual const lmVector3D *GetVertexArray() const override {return &Vertices[0];}
	virtual const lmVector3D *GetNormalArray() const override {return &Normals[0];}
	virtual const lmVector3D *GetTangentArray() const override {return &Tangents[0];}
	virtual const lmVector3D *GetBitangentArray() const override {return &Bitangents[0];}
	virtual const lmVector2D *GetTexCoordArray() const override {return &TexCoords[0];}
	virtual const int	*GetBoneIdArray() const override {return &(BoneId[0][0]);}
	virtual const float	*GetWeightArray() const override {return &(Weights[0][0]);}

	virtual unsigned int GetBoneIdStructSize() const override {return sizeof(lrmBoneId);}
	virtual unsigned int GetWeightStructSize() const override {return sizeof(lrmWeight);}

	virtual const std::string &GetMaterial() const override {return Material;}
	virtual unsigned int GetIndexBufferLength() const override {return IndexBuffer.size();}
	virtual const unsigned int *GetIndexArray() const override {return &IndexBuffer[0];}

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
        lrmWeight();

        /** \brief Operátor, mellyel el lehet érni a megadott indexű súlyt.
         *
         * \param i unsigned int: a súly indexe
         * \return float& - az i-edik súlyra mutató referencia.
         *
         */
        float &operator[](unsigned int i);

        /** \brief Operátor, mellyel el lehet érni a megadott indexű súlyt. Konstans tagfüggvény.
         *
         * \param i unsigned int: a súly indexe
         * \return float - az i-edik súly értéke.
         *
         */
        const float &operator[](unsigned int i) const;
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
        lrmBoneId();

        /** \brief Operátor, mellyel el lehet érni a megadott indexű bone id-t.
         *
         * \param i unsigned int: a bone id indexe
         * \return int& - az i-edik bone id-re mutató referencia.
         *
         */
        int &operator[](unsigned int i);

        /** \brief Operátor, mellyel el lehet érni a megadott indexű bone id-t. Konstans tagfüggvény.
         *
         * \param i unsigned int: a bone id indexe
         * \return int - az i-edik bone id értéke.
         *
         */
        const int &operator[](unsigned int i) const;
    };

    std::vector<lmVector3D>    Vertices;   /**< A vertexeket tartalmazó tömb */
    std::vector<lmVector2D>    TexCoords;  /**< A textúra koordinátákat tartalmazó tömb */
    std::vector<lmVector3D>    Normals;    /**< A normálvektorokat tartalmazó tömb */
    std::vector<lmVector3D>    Tangents;   /**< A tangent vektorokat tartalmazó tömb */
    std::vector<lmVector3D>    Bitangents; /**< A bitangent vektorokat tartalmazó tömb */
    std::vector<lrmBoneId>     BoneId;     /**< A bone id-ket tartalmazó tömb */
    std::vector<lrmWeight>     Weights;    /**< A súlyokat tartalmazó tömb */

    std::string Material; /**< A mesh materialjának kulcsa */
    std::vector<unsigned int> IndexBuffer; /**< A mesh index bufferje */

    /** \brief Kiprinteli az adatstruktúrát a paraméterben kapott ostream-re.
     *
     * \param out std::ostream& - az ostream, ahová kiírjuk az adatstruktúrát.
     * \return void
     *
     */
    void Print(std::ostream &out);

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
    lrmSkeletalMesh();

    /** \brief Destruktor, nem csinál semmit
     *
     */
    ~lrmSkeletalMesh();
};

#endif
