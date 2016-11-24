#ifndef LRM_TMP_STATIC_MESH
#define LRM_TMP_STATIC_MESH

#include "../lInterfaces/lResourceManager/liGPUFactory.h"

#include <vector>
#include <string>
#include <fstream>

/** \brief Egy static mesh fájlból betöltött adatai.
 *
 * \author Seres Lóránt Gábor
 *
 */
struct lrmStaticMesh : public liStaticMeshResource
{

	virtual unsigned int GetVertexBufferLength() const override {return Vertices.size();}

	virtual const lmVector3D *GetVertexArray() const override
	{return &Vertices[0];}

	virtual const lmVector3D *GetNormalArray() const override
	{return &Normals[0];}

	virtual const lmVector3D *GetTangentArray() const override
	{return &Tangents[0];}

	virtual const lmVector3D *GetBitangentArray() const override
	{return &Bitangents[0];}

	virtual const lmVector2D *GetTexCoordArray() const override
	{return &TexCoords[0];}

	virtual unsigned int GetMatGroupsSize() const override
	{return MatGroups.size();}

	virtual const liMtlGroup *GetMtlGroup(unsigned int i) const override
	{return MatGroups[i];}

    std::vector<lmVector3D> Vertices;   /**< A vertexeket tartalmazó tömb */
    std::vector<lmVector3D> Normals;    /**< A normálvektorokat tartalmazó tömb */
    std::vector<lmVector3D> Tangents;   /**< A tangent vektorokat tartalmazó tömb */
    std::vector<lmVector3D> Bitangents; /**< A bitangent vektorokat tartalmazó tömb */
    std::vector<lmVector2D> TexCoords;  /**< A textúra koordinátákat tartalmazó tömb */

    /** \brief Az egy materialt használó "részmeshek" materialját és index bufferjét tartalmazó adatstruktúra
     */
    struct lrmMtlGroup : public liMtlGroup
    {
		virtual const std::string &GetMaterial() const override {return Material;}
		virtual unsigned int GetIndexBufferLength() const override {return IndexBuffer.size();}
		virtual const unsigned int *GetIndexArray() const override {return &IndexBuffer[0];}

        std::string Material;                   /**< A material kulcsa */
        std::vector<unsigned int> IndexBuffer;  /**< Az index buffer */
    };

    std::vector<lrmMtlGroup *> MatGroups; /**< A material groupokat tartalmazó tömb. */

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
    lrmStaticMesh();

    /** \brief Destruktor, felszabadítja az összes material groupot.
     *
     */
    virtual ~lrmStaticMesh() override;
};

std::ostream &operator<<(std::ostream &out,const lrmStaticMesh &StaticMesh);
std::istream &operator<<(std::istream &in,lrmStaticMesh &StaticMesh);

#endif
