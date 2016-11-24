#ifndef LRM_WAVEFRONT_OBJ
#define LRM_WAVEFRONT_OBJ

#include "../lMath/lmVector3D.hpp"

#include "../lRenderer/lrMultiMaterial.h"

#include <string>
#include <vector>
#include <iostream>

#include "lrmStaticMesh.h"

/** \class lrmWfObj
 * \brief Egy Wavefront .obj fájlt importáló osztály.
 *
 * \todo Dokumentálni kéne
 *
 * \author Seres Lóránt
 *
 */
class lrmWavefrontLoader
{
private:
    struct lrmWfFile
    {
        struct lrmWfTriangle
        {
            int V1,V2,V3;
            int Tx1,Tx2,Tx3;
            int Normal;
        };

        struct lrmWfMatGroup
        {
            std::string Material;
            std::vector<lrmWfTriangle> Triangles;
        };

        std::vector<lrmWfMatGroup *> MatGroups;

        std::vector<lmVector3D> Vertices;
        std::vector<lmVector2D> TexCoords;
        std::vector<lmVector3D> Normals;

        std::vector<lmVector3D> smoothNormals;

        lrmWfFile()
        {

        }
        ~lrmWfFile()
        {
            for(auto i = MatGroups.begin();i != MatGroups.end();i++)
            {
                delete *i;
            }
        }
    };

    int LoadWavefrontObj(const std::string &materialPath,const std::string &staticMeshPath,const std::string &filename);
    int LoadWavefrontMtl(const std::string &filename);

    void SmoothNormals();

    /** \brief
     *
     * \todo az egymástól különálló mesheket szélességi kereséssel fel kéne térképezni, és mindegyikre a saját centrumára véve kéne kiszámolni a vektorokat.
     *
     * \param Target lrmStaticMesh&
     * \return void
     *
     */
    void CalculateTBVectors(lrmStaticMesh &Target);

    lrmWfFile File;
    std::string MaterialFileName;
    lrMultiMaterial *MultiMaterial;

public:
    const std::string &GetMaterialFileName() const;
    lrMultiMaterial *GetMultiMaterial();

    void Extract(lrmStaticMesh **Target);

    lrmWavefrontLoader(const std::string &materialPath,const std::string &staticMeshPath,const std::string &filename);
    ~lrmWavefrontLoader();
};

#endif
