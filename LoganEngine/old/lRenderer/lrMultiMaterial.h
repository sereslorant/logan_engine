#ifndef LR_MULTI_MATERIAL_H
#define LR_MULTI_MATERIAL_H

#include "../lInterfaces/lRenderer/liMultiMaterial.h"

/** \brief A modellek fényvisszaveréssel kapcsolatos anyagi tulajdonságai
 *
 * \todo Dokumentálni kéne.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrMaterial : public liMaterial
{
private:
    float Shininess;

    float Diffuse[3];
    float Specular[3];

    bool HasDiffuseMap	= false;	/**< True, ha a materialnak van diffuse mapja, false, ha nincs.  */
    bool HasNormalMap	= false;	/**< True, ha a materialnak van normal mapja, false, ha nincs.   */
    bool HasSpecularMap	= false;	/**< True, ha a materialnak van specular mapja, false, ha nincs. */
    bool HasEmissiveMap	= false;	/**< True, ha a materialnak van emissive mapja, false, ha nincs. */

    std::string DiffuseMap;     /**< A diffuse map kulcsa.  */
    std::string NormalMap;      /**< A normal map kulcsa.   */
    std::string SpecularMap;    /**< A specular map kulcsa. */
    std::string EmissiveMap;    /**< A emissive map kulcsa. */

public:

    virtual float GetShininess() const override {return Shininess;}

    virtual const float *GetDiffuse()  const override {return Diffuse;}
    virtual const float *GetSpecular() const override {return Specular;}

    virtual void SetDiffuseMap(const std::string &diffuse_map) override
	{
		if(!HasDiffuseMap)
		{
			HasDiffuseMap = true;
			DiffuseMap = diffuse_map;
		}
	}

    virtual void SetNormalMap(const std::string &normal_map) override
	{
		if(!HasNormalMap)
		{
			HasNormalMap = true;
			NormalMap = normal_map;
		}
	}

    virtual void SetSpecularMap(const std::string &specular_map) override
	{
		if(!HasSpecularMap)
		{
			HasSpecularMap = true;
			SpecularMap = specular_map;
		}
	}

    virtual void SetEmissiveMap(const std::string &emissive_map) override
	{
		if(!HasEmissiveMap)
		{
			HasEmissiveMap = true;
			EmissiveMap = emissive_map;
		}
	}

    virtual const std::string &GetDiffuseMap()	const override {return DiffuseMap;}
    virtual const std::string &GetNormalMap()	const override {return NormalMap;}
    virtual const std::string &GetSpecularMap() const override {return SpecularMap;}
    virtual const std::string &GetEmissiveMap() const override {return EmissiveMap;}

    lrMaterial(float *diffuse,float *specular,float shininess)
		:Shininess(shininess)
    {
		for(int i=0;i < 3;i++)
		{
			Diffuse[i]  = diffuse[i];
			Specular[i] = specular[i];
		}
	}

	virtual ~lrMaterial() override
	{
		//Üres függvénytörzs
	}
};

#include <map>

/** \brief Több materialt összefogó osztály. Egy static meshhez vagy characterhez tartozik egy multi material.
 * Az egyes material groupokhoz és skeletal meshekhez tartozik a multi materialon belül 1-1 material.
 *
 * \author Seres Lóránt Gábor
 *
 */
class lrMultiMaterial : public liMultiMaterial
{
private:
    std::map<std::string,liMaterial *> Materials;   /**< Az egyes materialokat tároló asszociatív  tömb */
    static lrMaterial DefaultMaterial;             /**< Default material - ha egy material nem található, akkor ezt adja vissza */

public:

    /** \brief Beállítja a default material tulajdonságait. Ha még nincs létrehozva, akkor újat allokál.
     *
     * \todo Ha már van allokálva default material, nem állítja be a tulajdonságait.
     *
     * \param diffuse float*                a default material új diffuse értéke
     * \param specular float*               a default material új specular értéke
     * \param shininess float               a default material új shininess értéke
     * \param diffuseMap const std::string& a default material új diffuse mapja
     * \param normalMap const std::string&  a default material új specular mapja
     * \return void
     *
     */
    static void SetDefaultMaterial(float *diffuse,float *specular,float shininess);

    /** \brief megadja, hogy a megadott kulcsú material szerepel-e a multi materialban.
     *
     * \param Key const std::string&: a material kulcsa
     * \return bool - true, ha a keresett material benne van a multi materialban.
     *
     */
    virtual bool CheckMaterial(const std::string &Key);

    /** \brief Elkéri a multi materialból a megadott kulcsú materialt.
     *
     * \param Key const std::string&: a material kulcsa
     * \return const lrMaterial* - az elkért material. Ha nincs benne a multi materialban, akkor a default material.
     *
     */
    virtual const liMaterial *GetMaterial(const std::string &Key) const;

    /** \brief Material hozzáadása a multi materialhoz.
     *
     * \param Key const std::string&:   a material kulcsa
     * \param Mtl lrMaterial*:          a beillesztendő material
     * \return bool - true, ha a material beillesztése sikeres. Ha az adott key alatt szerepel material, akkor nem írja felül, és false-sal tér vissza.
     *
     */
    bool AddMaterial(const std::string &Key,liMaterial *Mtl);

    /** \brief Konstruktor, nem csinál semmit.
     *
     */
    lrMultiMaterial();

    /** \brief Destruktor, felszabadítja az összes materialt.
     *
     */
    virtual ~lrMultiMaterial() override;
};

#endif // LR_MULTI_MATERIAL_H
