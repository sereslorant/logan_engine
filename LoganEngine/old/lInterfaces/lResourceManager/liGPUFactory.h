#ifndef LI_GPU_FACTORY_H
#define LI_GPU_FACTORY_H

/*
 * Itt include-oljuk be azokat az interface-eket, amelyeken keresztül a resource manager adatokat közöl a
 * motor rendererével.
 *
 */
#include "liBitmap.h"

#include "../../lMath/lMath.h"
#include <string>

class liStaticMeshResource
{
public:
	virtual unsigned int GetVertexBufferLength() const = 0;

	virtual const lmVector3D *GetVertexArray() const = 0;
	virtual const lmVector3D *GetNormalArray() const = 0;
	virtual const lmVector3D *GetTangentArray() const = 0;
	virtual const lmVector3D *GetBitangentArray() const = 0;
	virtual const lmVector2D *GetTexCoordArray() const = 0;

	class liMtlGroup
	{
	public:
		virtual const std::string &GetMaterial() const = 0;
		virtual unsigned int GetIndexBufferLength() const = 0;
		virtual const unsigned int *GetIndexArray() const = 0;

		liMtlGroup(){}
		virtual ~liMtlGroup(){}
	};

	virtual unsigned int GetMatGroupsSize() const = 0;
	virtual const liMtlGroup *GetMtlGroup(unsigned int i) const = 0;

	liStaticMeshResource(){}
	virtual ~liStaticMeshResource(){}
};

class liSkeletalMeshResource
{
public:
	static const unsigned int MAX_WEIGHT_COUNT = 4;

	virtual unsigned int GetVertexBufferLength() const = 0;

	virtual const lmVector3D *GetVertexArray() const = 0;
	virtual const lmVector3D *GetNormalArray() const = 0;
	virtual const lmVector3D *GetTangentArray() const = 0;
	virtual const lmVector3D *GetBitangentArray() const = 0;
	virtual const lmVector2D *GetTexCoordArray() const = 0;
	virtual const int	*GetBoneIdArray() const = 0;
	virtual const float	*GetWeightArray() const = 0;

	virtual unsigned int GetBoneIdStructSize() const = 0;
	virtual unsigned int GetWeightStructSize() const = 0;

	virtual const std::string	&GetMaterial() const = 0;
	virtual unsigned int		GetIndexBufferLength() const = 0;
	virtual const unsigned int	*GetIndexArray() const = 0;

	liSkeletalMeshResource(){}
	virtual ~liSkeletalMeshResource(){}
};

#include "../lRenderer/liImg.h"
#include "../lRenderer/liStaticMesh.h"
#include "../lRenderer/liSkeletalMesh.h"

class liGPUFactory
{
public:

    /** \brief Ez a függvény feltölt egy bittérképet a videokártya memóriájába, és visszaad egy lrImg-et paraméterbe, amin keresztül el lehet érni.
     *
     * \todo A paraméterekhez fűzöttkomment nagyon parasztosan van megfogalmazva, át kell fogalmazni.
     *
     * \param Img lrImg **:  Arra az lrImg pointerre mutató pointer, amelyet rá akarunk állítani az újonnan létrehozott lrImg-re.
     * \param Bmp BMP *:     A betöltött bittérkép adatai
     * \return void
     *
     */
    virtual void UploadImage(liImg* &Img,liBitmap *Bmp) = 0;

    virtual void UploadCubemap(liImg* &Img,liBitmap *bitmaps[6]) = 0;

    /** \brief Ez a függvény feltölti a paraméterben kapott statikus mesht a videokártya memóriájába, és visszaad egy lrStaticMesht paraméterbe, amin keresztül el lehet érni.
     *
     * \todo A paraméterekhez fűzöttkomment nagyon parasztosan van megfogalmazva, át kell fogalmazni.
     *
     * \param Mesh lrStaticMesh **:  arra a static mesh pointerre mutató pointer, amelyet rá akarunk állítani az újonnan létrehozott statikus meshre.
     * \param Src lrmStaticMesh *:   a fájlból betöltött csúcspontokat, háromszögeket és material groupokat tartalmazó adatstruktúra.
     * \return void
     *
     */
    virtual void UploadStaticMesh(liStaticMesh* &Mesh,liStaticMeshResource *Src) = 0;

    /** \brief Ez a függvény feltölti a paraméterben kapott skeletal mesht a videokártya memóriájába, és visszaad egy lrSkeletalMesht paraméterbe, amin keresztül el lehet érni.
     *
     * \todo A paraméterekhez fűzöttkomment nagyon parasztosan van megfogalmazva, át kell fogalmazni.
     *
     * \param Mesh lrSkeletalMesh **:  arra a skeletal mesh pointerre mutató pointer, amelyet rá akarunk állítani az újonnan létrehozott skeletal meshre.
     * \param Src lrmSkeletalMesh *:   a fájlból betöltött csúcspontokat, háromszögeket és stb.-ket tartalmazó adatstruktúra.
     * \return void
     *
     */
    virtual void UploadSkeletalMesh(liSkeletalMesh* &Mesh,liSkeletalMeshResource *Src) = 0;

	liGPUFactory()
	{
		//Üres függvénytörzs
	}

	virtual ~liGPUFactory()
	{
		//Üres függvénytörzs
	}
};

#endif // LI_GPU_FACTORY_H
